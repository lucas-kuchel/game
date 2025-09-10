#include <metal_stdlib>

using namespace metal;

struct Camera
{
    float4x4 Projection;
    float4x4 View;
};

struct VSInput
{
    float3 Position [[attribute(0)]];
    float4 Colour   [[attribute(1)]];
};

struct VSOutput
{
    float4 Position [[position]];
    float4 Colour;
};

vertex VSOutput VSMain(VSInput in                 [[stage_in]],
                       constant Camera& camera    [[buffer(0)]])
{
    VSOutput out;

    float4 pos = float4(in.Position, 1.0);

    pos = camera.View * pos;
    pos = camera.Projection * pos;

    out.Position = pos;
    out.Colour = in.Colour;
    return out;
}

fragment float4 PSMain(VSOutput in [[stage_in]])
{
    return in.Colour;
}