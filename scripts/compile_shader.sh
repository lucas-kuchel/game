#!/usr/bin/env bash

set -e

HLSL_FILE="$1"
ENTRY="$2"
STAGE="$3"
OUTPUT_DIR="${4:-shaders_out}"

if [[ ! -f "$HLSL_FILE" ]]; then
  echo "HLSL file not found: $HLSL_FILE"
  exit 1
fi

BASENAME=$(basename "$HLSL_FILE" .hlsl)
mkdir -p "$OUTPUT_DIR"

# Determine target profile
case "$STAGE" in
  vertex) TARGET="vs_6_0" ;;
  pixel)  TARGET="ps_6_0" ;;
  *) echo "Unknown stage: $STAGE"; exit 1 ;;
esac

# Output paths
SPV_OUT="$OUTPUT_DIR/${BASENAME}.${STAGE}.spv"
DXBC_OUT="$OUTPUT_DIR/${BASENAME}.${STAGE}.dxbc"
GLSL_OUT="$OUTPUT_DIR/${BASENAME}.${STAGE}.glsl"
MSL_OUT="$OUTPUT_DIR/${BASENAME}.${STAGE}.metal"

echo "Compiling HLSL -> SPIR-V..."
dxc -T $TARGET -E $ENTRY -Fo "$SPV_OUT" "$HLSL_FILE" -spirv -fspv-target-env=vulkan1.3

echo "Compiling HLSL -> DXBC..."
dxc -T $TARGET -E $ENTRY -Fo "$DXBC_OUT" "$HLSL_FILE"

echo "SPIR-V -> GLSL..."
spirv-cross "$SPV_OUT" --version 460 --output "$GLSL_OUT" 

echo "SPIR-V -> MSL..."
spirv-cross "$SPV_OUT" \
    --msl \
    --msl-version 20100 \
    --msl-argument-buffers \
    --msl-pad-fragment-output \
    --output "$MSL_OUT"

echo "Done:"
echo "  SPIR-V: $SPV_OUT"
echo "  DXBC:   $DXBC_OUT"
echo "  GLSL:   $GLSL_OUT"
echo "  MSL:    $MSL_OUT"
