cbuffer Camera : register(b0) {
  float4x4 Projection;
  float4x4 View;
}

struct VSInput {
  float3 Position : POSITION;
  float4 Colour : COLOR0;
};

struct VSOutput {
  float4 Position : SV_Position;
  float4 Colour : COLOR0;
};

VSOutput VSMain(VSInput input) {
  VSOutput output;
  output.Position = float4(input.Position, 1.0);
  output.Colour = input.Colour;
  return output;
}

float4 PSMain(VSOutput input) : SV_Target { return input.Colour; }