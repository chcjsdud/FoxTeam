#include "CbufferHeader.fx"
#include "LightHeader.fx"

struct VertexIn
{
    float4 Position : POSITION;
    float4 Texcoord : TEXTURECOORD;
    float4 Normal : NORMAL;
};

struct VertexOut
{
    // 
    float4 Position : SV_POSITION;
    float4 ViewPosition : POSITION;
    float4 Texcoord : TEXTURECOORD;
    float4 ViewNormal : NORMAL;
};

VertexOut TextureDeferredLight_VS(VertexIn _In)
{
    VertexOut Out = (VertexOut) 0;
    
    // [][][][]
    // [][][][]
    // [][][][1]
    // [][][][]
    
    _In.Position.w = 1.0f;
    Out.Position = mul(_In.Position, WVP_);
    Out.ViewPosition = mul(_In.Position, WV_);
    Out.Texcoord = _In.Texcoord;

    // w에 z값 들어있잖아.
    // Out.Position.w = 1.0f;
    
    
    // 픽셀을 건져내기 위한 포지션
    
    _In.Normal.w = 0.0f;
    Out.ViewNormal = normalize(mul(_In.Normal, WV_));
    Out.ViewNormal.w = 0.0f;
    return Out;
}


struct DeferredOutPut
{
    float4 ViewDif : SV_Target0; // 텍스처 색깔
    float4 ViewPos : SV_Target1;
    float4 ViewNor : SV_Target2;
};

Texture2D DiffuseTex : register(t0);
SamplerState Smp : register(s0);

DeferredOutPut TextureDeferredLight_PS(VertexOut _In) 
{
    DeferredOutPut Out;

    Out.ViewDif = (DiffuseTex.Sample(Smp, _In.Texcoord.xy));
    Out.ViewPos = _In.ViewPosition;
    Out.ViewPos.w = 1.0f;
    Out.ViewNor = _In.ViewNormal;
    Out.ViewNor.w = 1.0f;
    
    return Out;
}


