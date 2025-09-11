cbuffer Camera
{
    float4x4 Projection;
    float4x4 View;
};

struct VSInput
{
    float3 Position : POSITION;
    float4 Colour   : COLOR;
};

struct VSOutput
{
    float4 Position : SV_POSITION;
    float4 Colour   : COLOR;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output;

    output.Position = mul(Projection, mul(View, float4(input.Position, 1.0f)));

    output.Colour = input.Colour;

    return output;
}

float4 PSMain(VSOutput input) : SV_TARGET
{
    return input.Colour;
}
