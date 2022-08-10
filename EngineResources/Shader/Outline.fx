#include "CbufferHeader.fx"
#include "LightHeader.fx"
#include "AniHeader.fx"

struct VertexIn
{
    float4 Pos : POSITION;
    float3 Normal : NORMAL;
};

struct VertexOut
{
    float4 Pos : POSITION;
    float4 Texcoord : TEXTURECOORD;
};



VertexOut Outline_VS(VertexIn _In)
{
    VertexOut Out;

    _In.Pos.xy = sign(_In.Pos.xy);
    Out.Pos = float4(_In.Pos.xy, 0.0, 1.0);
    
    // image_space
    Out.Texcoord.x = 0.5 * (1 + _In.Pos.x);
    Out.Texcoord.y = 0.5 * (1 - _In.Pos.y);
   
  
    return Out;
}

cbuffer view_proj_matrix : register(0)
{
    float4 vDiffuseColor;
};

struct DeferredOutPut
{
    float4 ViewDif : SV_Target0;
    float4 ViewPos : SV_Target1;
    float4 ViewNor : SV_Target2;
};

SamplerState OutlineSmp : register(s0);

float Mask[9] =
{
    -1, -1, -1,
    -1, 8, -1,
    -1, -1, -1
}; // 라플레시안 필터

float coord[3] = { -1, 0, +1 };
float divider = 1;
float MAP_CX = 512;
float MAP_CY = 512;

struct PS_INPUT
{
    float2 TexCoord : TEXCOORD0;
};


float Outline_PS(float2 Tex : TEXCOORD0) : COLOR
{
    float4 Color = 0;
    
    float4 Ret;
    
    float3 GrayScale = float3(0.3, 0.59, 0.11);
    
    for (int i = 0; i < 9; i++)
    {
        Color += Mask[i] * (tex2D(RT, Tex + float2(coord[i % 3] / MAP_CX, coord[i / 3] / MAP_CY)));
    }
    
  //  float gray = 1 - (Color.r * 0.3 + Color.g * 0.59 + Color.b * 0.11);
    float gray = 1 - dot(Color, GrayScale);
    Ret = float4(gray, gray, gray, 1) / divider;
    
    return Ret;
}

