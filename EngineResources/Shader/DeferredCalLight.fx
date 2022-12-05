#include "LightHeader.fx"

struct VertexIn
{
    float4 Position : POSITION;
    float4 Texcoord : TEXTURECOORD;
};

struct VertexOut
{
    float4 Position : SV_POSITION;
    float4 Texcoord : TEXTURECOORD;
};

VertexOut DeferredCalLight_VS(VertexIn _In)
{
    VertexOut Out = (VertexOut) 0;
    Out.Position = _In.Position;
    Out.Texcoord = _In.Texcoord;
    return Out;
}


Texture2D PositionTex : register(t1);
Texture2D NormalTex : register(t2);
Texture2DArray LightShadowTex : register(t3);                                        // 1장의 텍스쳐에 레벨에서 사용하는 모든 빛의 깊이데이터를 기록한 텍스쳐

SamplerState Smp : register(s0);

struct LightOutPut
{
    float4 DifLight : SV_Target0; // 텍스처 색깔
    float4 SpcLight : SV_Target1;
    float4 AmbLight : SV_Target2;
};

LightOutPut DeferredCalLight_PS(VertexOut _In)
{
    LightOutPut Out = (LightOutPut) 0.0f;
    
    float4 ViewPosition = (PositionTex.Sample(Smp, _In.Texcoord.xy));
    float4 ViewNormal = (NormalTex.Sample(Smp, _In.Texcoord.xy));
    
    if (0.0f >= ViewPosition.a)
    {
        clip(-1);
    }
    ViewNormal.w = 0.0f;
    
    Out.DifLight = (float4) 0.0f;
    Out.SpcLight = (float4) 0.0f;
    Out.AmbLight = (float4) 0.0f;
    
    for (int i = 0; i < LightCount; ++i)
    {
        Out.DifLight += CalculateDirectionDiffuseLight(ViewNormal, Lights[i]);
        Out.SpcLight += CalculateDirectionSpacularLight(ViewPosition, ViewNormal, Lights[i]);
        Out.AmbLight += CalculateDirectionAmbientLight(Lights[i]);
    }
    
    //============================================================= 그림자 계산
    int Count = 0;
    for (int LightNum = 0; LightNum < LightCount; ++LightNum)
    {
        if (Out.DifLight.x > 0.0f)
        {
            float4 WorldPosition = mul(float4(ViewPosition.xyz, 1.0f), Lights[LightNum].CameraViewInverse);
            float4 ShaodwPos = mul(WorldPosition, Lights[LightNum].LightVP);

            // 보간후 재균질화
            float fDepth = ShaodwPos.z / ShaodwPos.w;
            float fUvX = ShaodwPos.x / ShaodwPos.w;
            float fUvY = ShaodwPos.y / ShaodwPos.w;
            
            // 라이트클립공간(x,y는 -1 ~ 1 / z는 0 ~ 1 범위)의 좌표를 텍스쳐공간(x,y는 0 ~ 1범위)으로 변환
            float2 ShadowUv = float2(fUvX * 0.5f + 0.5f, fUvY * -0.5f + 0.5f);

            // 단, 해당 위치가 빛을 비추는 범위일때만 그림자 영역으로 판단
            if (0.001f < ShadowUv.x && 0.999f > ShadowUv.x && 0.001f < ShadowUv.y && 0.999f > ShadowUv.y)
            {
                // 그림자맵의 해당 좌표를 샘플링(그림자맵의 깊이 Get)
                float fShadowDepth = LightShadowTex.Sample(Smp, float3(ShadowUv.x, ShadowUv.y, LightNum)).r;
                
                // 클립 공간 z 값이 그림자 맵 값보다 크면 픽셀이 그림자로 판단
                if (0.0f < fShadowDepth && fDepth > fShadowDepth + 0.001f)
                {
                    Count++;
                }
            }
        }
    }

    if (0 != Count)
    {
        Out.DifLight *= 0.5f;
        Out.SpcLight *= 0.5f;
    }
    
    for (int ShadowNum = 0; ShadowNum < Count; ++ShadowNum)
    {
        Out.DifLight -= float4(0.5f, 0.5f, 0.5f, 0.5f);
    }
    
    //============================================================= 그림자 계산
    
    return Out;
}


