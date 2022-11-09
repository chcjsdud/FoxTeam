#include "CbufferHeader.fx"
#include "LightHeader.fx"

struct VertexIn
{
    float4 Position : POSITION;
    float4 Texcoord : TEXTURECOORD;
    float4 Normal : NORMAL;
    float4 BiNormal : BINORMAL;
    float4 Tangent : TANGENT;
};

struct VertexOut
{
    float4 Position : SV_POSITION;
    float4 ViewPosition : POSITION;
    float4 Texcoord : TEXTURECOORD;
    float4 ViewNormal : NORMAL;
    float4 ViewBiNormal : BINORMAL;
    float4 ViewTangent : TANGENT;
};

VertexOut DeferredLightMap_VS(VertexIn _In)
{
    VertexOut Out = (VertexOut) 0;
    
    _In.Position.w = 1.0f;
    Out.Position = mul(_In.Position, WVP_);
    Out.ViewPosition = mul(_In.Position, WV_);
    Out.Texcoord = _In.Texcoord;
    
    //_In.Normal.w = 0.0f;
    //Out.ViewNormal = normalize(mul(_In.Normal, WV_));
    //Out.ViewNormal.w = 0.0f;
    
    //_In.BiNormal.w = 0.0f;
    //Out.ViewBiNormal = normalize(mul(_In.BiNormal, WV_));
    //Out.ViewBiNormal.w = 0.0f;
    
    //_In.Tangent.w = 0.0f;
    //Out.ViewTangent = normalize(mul(_In.Tangent, WV_));
    //Out.ViewTangent.w = 0.0f;
    
    
    float4 Tangent = _In.Tangent;
    Tangent.x = 1.0f;
    Tangent.y = 0.0f;
    Tangent.z = 0.0f;
    Tangent.w = 0.0f;
    
    float4 Normal = _In.Normal;
    Normal.x = 0.0f;
    Normal.y = 1.0f;
    Normal.z = 0.0f;
    Normal.w = 0.0f;
    
    float4 BiNormal = _In.BiNormal;
    BiNormal.x = 0.0f;
    BiNormal.y = 0.0f;
    BiNormal.z = 1.0f;
    BiNormal.w = 0.0f;
    
    Out.ViewTangent = normalize(mul(Tangent, WV_));
    Out.ViewNormal = normalize(mul(Normal, WV_));
    Out.ViewBiNormal = normalize(mul(BiNormal, WV_));
    
    
    
    
    return Out;
}

struct DeferredOutPut
{
    float4 ViewDif : SV_Target0;
    float4 ViewPos : SV_Target1;
    float4 ViewNor : SV_Target2;
};

Texture2D DiffuseTex : register(t0);
Texture2D NormalTex : register(t1);
SamplerState WrapSmp : register(s0);

DeferredOutPut DeferredLightMap_PS(VertexOut _In)
{
    DeferredOutPut Out;

    Out.ViewDif = (DiffuseTex.Sample(WrapSmp, _In.Texcoord.xy));
    
    if (0.0f >= Out.ViewDif.w)
    {
        Out.ViewDif.w = 1.0f;
    }

    if (Out.ViewDif.r == 0.0f && Out.ViewDif.g == 0.0f && Out.ViewDif.b == 0.0f)
    {
        clip(-1);
    }
    
    Out.ViewPos = _In.ViewPosition;
    Out.ViewPos.w = 1.0f;
    Out.ViewNor = _In.ViewNormal;
    Out.ViewNor.w = 1.0f;
    
    if(0 != IsBump)
    {
        Out.ViewNor = BumpNormalCalculate(NormalTex, WrapSmp, _In.Texcoord, _In.ViewTangent, _In.ViewBiNormal, _In.ViewNormal);
        Out.ViewNor.w = 1.0f;
    }
    
    return Out;
}

