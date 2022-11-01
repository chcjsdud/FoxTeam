#include "CbufferHeader.fx"
#include "AniHeader.fx"

struct VertexIn
{
    float4 Position : POSITION;
    float4 Weight : BLENDWEIGHT;
    int4 Index : BLENDINDICES;
};

struct VertexOut
{
    float4 Position : SV_POSITION;
};

VertexOut ShadowAni_VS(VertexIn _In)
{
    VertexOut Out = (VertexOut)0;
    
    Skinning(_In.Position, _In.Weight, _In.Index, ArrAniMationMatrix);
    _In.Position.w = 1.0f;
    Out.Position = mul(_In.Position, WVP_);
    
    return Out;
}

cbuffer ShadowColor : register(b0)
{
    float4 vColor;
};

float4 ShadowAni_PS(VertexOut _In) : SV_Target0
{
    return vColor;
}