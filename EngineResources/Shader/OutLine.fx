#include "CbufferHeader.fx"

struct VertexIn
{
    float4 Position : POSITION;
    float4 Texcoord : TEXTURECOORD;
    float4 Normal : NORMAL;
};

struct VertexOut
{
    float4 Position : SV_POSITION;
    float4 Texcoord : TEXTURECOORD;
    float4 Normal : NORMAL;
};

Texture2D DiffuseTex : register(t0);
SamplerState Smp : register(s0);

cbuffer OutLineData : register(b1)
{
    float4 LineColor;
    float LineThickness;
};

VertexOut OutLine_VS(VertexIn _in)
{
    VertexOut Out = (VertexOut)0;
    
    // 기존정점의 노말값 * 라인두께를하여 정점을 확장
    // 오브젝트공간에서의 정점의 위치 + (정점의 노말 * 두께)
    float4 ExpansionPos = (_in.Position.xyz + _in.Normal.xyz * LineThickness, 1);
    Out.Position = mul(ExpansionPos, WVP_);
    
    return Out;
}

float4 OutLine_PS(VertexOut _in) : SV_Target0
{
    float4 Color = (float4)0;
    Color = LineColor;
    return Color;
}