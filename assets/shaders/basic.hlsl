cbuffer Camera {
  float4x4 Projection;
  float4x4 View;
};

cbuffer Transform {
  float4x4 Model;
};

struct VSInput {
  float3 Position : POSITION;
  float4 Colour : COLOR;
};

struct VSOutput {
  float4 Position : SV_POSITION;
  float4 Colour : COLOR;
  float3 Normal : NORMAL;
};

VSOutput VSMain(VSInput input) {
  VSOutput output;

  output.Position = mul(Projection, mul(View, mul(Model, float4(input.Position, 1.0f))));
  output.Colour = input.Colour;
  output.Normal = input.Position;

  return output;
}

float4 PSMain(VSOutput input) : SV_TARGET {
  float3 lightDir = normalize(float3(0.0, 0.5, 0.5));
  float diffuse = max(dot(input.Normal, lightDir), 0.0);

  return input.Colour * diffuse; 
}
