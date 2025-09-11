#!/usr/bin/env python3
import argparse
import subprocess
import sys
import os
import platform

def run(cmd):
    print(f"Running: {' '.join(cmd)}")
    subprocess.run(cmd, check=True)

def compile_stage(stage, is_macos):
    hlsl_file = stage["path"]
    entry = stage["fun"]
    stage_type = stage["type"]

    if not os.path.isfile(hlsl_file):
        print(f"HLSL file not found: {hlsl_file}")
        sys.exit(1)

    # Base output dir = input path minus extension
    base_outdir = os.path.splitext(hlsl_file)[0]
    os.makedirs(base_outdir, exist_ok=True)

    basename = os.path.splitext(os.path.basename(hlsl_file))[0]

    target = {"vertex": "vs_6_0", "pixel": "ps_6_0"}.get(stage_type)
    if not target:
        print(f"Unknown stage type: {stage_type}")
        sys.exit(1)

    # Backend subfolders
    spv_dir  = os.path.join(base_outdir, "spv")
    dxbc_dir = os.path.join(base_outdir, "dxbc")
    glsl_dir = os.path.join(base_outdir, "glsl")
    msl_dir  = os.path.join(base_outdir, "msl")

    for d in [spv_dir, dxbc_dir, glsl_dir, msl_dir]:
        os.makedirs(d, exist_ok=True)

    # Output paths
    spv_out  = os.path.join(spv_dir, f"{basename}.{stage_type}.spv")
    dxbc_out = os.path.join(dxbc_dir, f"{basename}.{stage_type}.dxbc")
    glsl_out = os.path.join(glsl_dir, f"{basename}.{stage_type}.glsl")
    msl_out  = os.path.join(msl_dir, f"{basename}.{stage_type}.metal")
    metal_object = os.path.join(msl_dir, f"{basename}.{stage_type}.air")

    print(f"Compiling {stage_type} stage: {hlsl_file} ({entry})")

    # HLSL -> SPIR-V
    run(["dxc", "-T", target, "-E", entry, "-Fo", spv_out, hlsl_file,
         "-spirv", "-fspv-target-env=vulkan1.3"])
    # HLSL -> DXBC
    run(["dxc", "-T", target, "-E", entry, "-Fo", dxbc_out, hlsl_file])
    # SPIR-V -> GLSL
    run(["spirv-cross", spv_out, "--version", "460", "--output", glsl_out])
    # SPIR-V -> MSL
    run(["spirv-cross", spv_out,
         "--msl", "--msl-version", "20100",
         "--msl-argument-buffers",
         "--msl-pad-fragment-output",
         "--output", msl_out])

    if is_macos:
        print(f"Compiling Metal shader to object: {msl_out} -> {metal_object}")
        run(["xcrun", "-sdk", "macosx", "metal", "-c", msl_out, "-o", metal_object])
        return metal_object

def main():
    parser = argparse.ArgumentParser(description="Compile HLSL shaders to multiple targets")
    parser.add_argument("--stage", action="append", nargs="+", required=True,
                        help='Stage specification: type=vertex path="file.hlsl" fun=EntryPoint')
    args = parser.parse_args()

    is_macos = platform.system() == "Darwin"
    metal_objects = []

    for stage_args in args.stage:
        stage = {}
        for kv in stage_args:
            key, val = kv.split("=", 1)
            stage[key] = val.strip('"')
        msl_object = compile_stage(stage, is_macos)
        if msl_object:
            metal_objects.append(msl_object)

    # Link Metal objects on macOS
    if is_macos and metal_objects:
        base_outdir = os.path.splitext(stage["path"])[0]  # last stage path used
        metallib = os.path.join(base_outdir, "msl", "shaders.metallib")
        print(f"Linking Metal objects into metallib: {metallib}")
        run(["xcrun", "-sdk", "macosx", "metallib"] + metal_objects + ["-o", metallib])
        print(f"Metal library created: {metallib}")

if __name__ == "__main__":
    main()