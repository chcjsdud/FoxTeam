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
    
    // ���������� �븻�� * ���εβ����Ͽ� ������ Ȯ��
    // ������Ʈ���������� ������ ��ġ + (������ �븻 * �β�)
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