#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct type_Camera
{
    float4x4 Projection;
    float4x4 View;
};

struct spvDescriptorSetBuffer0
{
    constant type_Camera* Camera [[id(0)]];
};

struct VSMain_out
{
    float4 out_var_COLOR0 [[user(locn0)]];
    float4 gl_Position [[position]];
};

struct VSMain_in
{
    float3 in_var_POSITION [[attribute(0)]];
    float4 in_var_COLOR0 [[attribute(1)]];
};

vertex VSMain_out VSMain(VSMain_in in [[stage_in]], constant spvDescriptorSetBuffer0& spvDescriptorSet0 [[buffer(0)]])
{
    VSMain_out out = {};
    out.gl_Position = (float4(in.in_var_POSITION, 1.0) * (*spvDescriptorSet0.Camera).View) * (*spvDescriptorSet0.Camera).Projection;
    out.out_var_COLOR0 = in.in_var_COLOR0;
    return out;
}

