#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct PSMain_out
{
    float4 out_var_SV_Target [[color(0)]];
};

struct PSMain_in
{
    float4 in_var_COLOR0 [[user(locn0)]];
};

fragment PSMain_out PSMain(PSMain_in in [[stage_in]])
{
    PSMain_out out = {};
    out.out_var_SV_Target = in.in_var_COLOR0;
    return out;
}

