#include "PreCompile.h"
#include <GameEngine/CameraComponent.h>
#include "GameEngineFBXRenderer.h"
#include "GameEngineLevel.h"
#include "GameEngineRenderingPipeLineManager.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineTransform.h"
#include "GameEngineVertexShader.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"
#include "GameEnginePixelShader.h"
#include "CameraComponent.h"

#include "GameEngineFBXMesh.h"
#include "GameEngineFBXMeshManager.h"
#include "GameEngineFBXAnimation.h"
#include "GameEngineFBXAnimationManager.h"


GameEngineFBXRenderer::GameEngineFBXRenderer()
    : CurAnimation(nullptr)
    , FBXMesh(nullptr)
{
}

GameEngineFBXRenderer::~GameEngineFBXRenderer()
{
	for (size_t i = 0; i < RenderSets.size(); i++)
	{
		delete RenderSets[i].PipeLine_;
        delete RenderSets[i].ShaderHelper;
	}

    for (auto& Animation :  Animations)
    {
        delete Animation.second;
    }

    
}


void GameEngineFBXRenderer::Render(float _DeltaTime)
{

	for (size_t i = 0; i < RenderSets.size(); i++)
	{
		RenderSets[i].ShaderHelper->Setting();
		RenderSets[i].PipeLine_->Rendering();
		RenderSets[i].ShaderHelper->ReSet();
		RenderSets[i].PipeLine_->Reset();
	}
}

void GameEngineFBXRenderer::SetFBXMesh(const std::string& _Value, std::string _PipeLine)
{
    FBXMesh = GameEngineFBXMeshManager::GetInst().Find(_Value);

    if (nullptr == FBXMesh)
    {
        GameEngineDebug::MsgBoxError("존재하지 않는 fbx매쉬를 세팅했습니다.");
    }



    GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Find(_PipeLine);


    for (int MeshSetIndex = 0; MeshSetIndex < FBXMesh->GetMeshSet().size(); MeshSetIndex++)
    {
        for (int VertexBufferIndex = 0; VertexBufferIndex < FBXMesh->GetMeshSet()[MeshSetIndex].GameEngineVertexBuffers.size(); VertexBufferIndex++)
        {
            for (int IndexBufferIndex = 0; IndexBufferIndex < FBXMesh->GetMeshSet()[MeshSetIndex].GameEngineIndexBuffers[VertexBufferIndex].size(); IndexBufferIndex++)
            {

                GameEngineVertexBuffer* VertexBuffer = FBXMesh->GetMeshSet()[MeshSetIndex].GameEngineVertexBuffers[VertexBufferIndex];
                GameEngineIndexBuffer* IndexBuffer = FBXMesh->GetMeshSet()[MeshSetIndex].GameEngineIndexBuffers[VertexBufferIndex][IndexBufferIndex];

                RenderSet& RenderSetData = RenderSets.emplace_back();
                RenderSetData.PipeLine_ = Pipe->Clone();
                RenderSetData.ShaderHelper = new GameEngineShaderResHelper();

                RenderSetData.ShaderHelper->ShaderResourcesCheck(RenderSetData.PipeLine_->GetPixelShader());
                RenderSetData.ShaderHelper->ShaderResourcesCheck(RenderSetData.PipeLine_->GetVertexShader());

                if (true == RenderSetData.ShaderHelper->IsConstantBuffer("TransformData"))
                {
                    RenderSetData.ShaderHelper->SettingConstantBufferLink("TransformData", GetTransform()->GetTransformData());
                }

                if (true == RenderSetData.ShaderHelper->IsConstantBuffer("LightsData"))
                {
                    const LightsData& LightData = GetLevel()->GetMainCamera()->GetLightData();
                    RenderSetData.ShaderHelper->SettingConstantBufferLink("LightsData", LightData);
                }

                if (true == RenderSetData.ShaderHelper->IsStructuredBuffer("ArrAniMationMatrix"))
                {
                    FBXMesh->ImportBone();
                    if (0 == BoneData.size())
                    {
                        BoneData.resize(FBXMesh->GetBoneCount());
                    }

                    const LightsData& LightData = GetLevel()->GetMainCamera()->GetLightData();
                    // 15개가 동일하게 공유하는데 맵 언맵
                    // 음? 1번만 해도 되는걸 15번을 하게 될것이다.
                    RenderSetData.ShaderHelper->SettingStructuredBufferSetting("ArrAniMationMatrix", FBXMesh->GetAnimationBuffer());
                    RenderSetData.ShaderHelper->SettingStructuredBufferLink("ArrAniMationMatrix", &BoneData[0], sizeof(float4x4) * BoneData.size());
                }

                RenderSetData.PipeLine_->SetInputAssembler1VertexBufferSetting(VertexBuffer);
                RenderSetData.PipeLine_->SetInputAssembler2IndexBufferSetting(IndexBuffer);

                // Data.ShaderHelper

                // 


                //GameEngineRenderer* Renderer = NewActor->CreateTransformComponent<GameEngineRenderer>(NewActor->GetTransform());
                // Renderer->SetRenderingPipeLine("Color");
                //Renderer->SetMesh(VertexBuffer, IndexBuffer);
                //Renderer->GetTransform()->SetLocalScaling({ 10.0f, 10.0f, 10.0f });
                //Renderer->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
                //Renderer->ShaderHelper.SettingConstantBufferSet("ResultColor", float4(1.0f, 0.0f, 0.0f));
            }
        }
    }
}

void GameEngineFBXRenderer::Start()
{
	GetLevel()->GetMainCamera()->PushRenderer(GetOrder(), this);
}

void GameEngineFBXRenderer::Update(float _DeltaTime)
{
    if (nullptr != CurAnimation)
    {
        CurAnimation->Update(_DeltaTime);
    }
}


void GameEngineFBXRenderer::CreateFBXAnimation(const std::string& _AnimationName, const std::string& _AnimationFBXName)
{
    if (nullptr == FBXMesh)
    {
        GameEngineDebug::MsgBoxError("매쉬를 세팅하지 않은 랜더러에 애니메이션을 만들 수가 없습니다.");
        return;
    }

    if (Animations.end() != Animations.find(_AnimationName)) 
    {
        GameEngineDebug::MsgBoxError("같은 이름의 애니메이션을 또 만들수는 없습니다.");
        return;
    }

    GameEngineFBXAnimation* Animation = GameEngineFBXAnimationManager::GetInst().Find(_AnimationFBXName);

    if (nullptr == Animation)
    {
        GameEngineDebug::MsgBoxError("같은 이름의 애니메이션을 또 만들수는 없습니다.");
        return;
    }

    FBXAnimation* NewFBXAnimation = new FBXAnimation();

    NewFBXAnimation->Mesh = FBXMesh;
    NewFBXAnimation->Animation = Animation;
    NewFBXAnimation->ParentRenderer = this;


    NewFBXAnimation->Init();


    Animations.insert(std::make_pair(_AnimationName, NewFBXAnimation));

}

void GameEngineFBXRenderer::ChangeFBXAnimation(const std::string& _AnimationName) 
{
    std::map<std::string, FBXAnimation*>::iterator FindIter = Animations.find(_AnimationName);

    if (Animations.end() == FindIter)
    {
        GameEngineDebug::MsgBoxError("존재하지 않는 애니메이션으로 체인지 하려고 했습니다.");
        return;
    }

    CurAnimation = FindIter->second;
}

void FBXAnimation::Init()
{

    Animation->CalFbxExBoneFrameTransMatrix(Mesh);
    PixAniData = &Animation->AnimationDatas[0];
    Start = 0;
    End = PixAniData->AniFrameData[0].BoneMatData.size();
    FrameTime = 0.02f;

    //}

}

void FBXAnimation::Update(float _DeltaTime) 
{
    // 행렬 만들어서

    // 0~24진행이죠?
    CurFrameTime += _DeltaTime;
    //                      0.1
    if (CurFrameTime >= FrameTime)
    {
        // 여분의 시간이 남게되죠?
        // 여분의 시간이 중요합니다.
        CurFrameTime -= FrameTime;
        // 0.00011
        ++CurFrame;
    }

    if (CurFrame >= End)
    {
        CurFrame = Start;
    }

    int NextFrame = CurFrame;

    ++NextFrame;

    if (NextFrame >= End)
    {
        NextFrame = 0;
    }


    // 아까도 이야기했지만 이녀석이 64개의 본을 가지고 있으니까
    // 각본의 n번째 프레임의 64개의 애니메이션 상태를 만들어줘야 합니다.

    for (int i = 0; i < ParentRenderer->BoneData.size(); i++)
    {
        Bone* BoneData = ParentRenderer->FBXMesh->FindBone(i);

        if (true == PixAniData->AniFrameData[i].BoneMatData.empty())
        {
            // 본의 프레임데이터가 없을때가 있습니다.
            // 보통 항등행렬이 들어가 있는 경우가 많습니다.

            // GameEngineDebug::MsgBoxError("아핀행렬 만들어라!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            ParentRenderer->BoneData[i] = float4x4::Affine(BoneData->BonePos.GlobalScale, BoneData->BonePos.GlobalRotation, BoneData->BonePos.GlobalTranslation);
            return;
        }

        // 현재프레임과 
        FbxExBoneFrameData& CurData = PixAniData->AniFrameData[i].BoneMatData[CurFrame];
        // 다음프레임의 정보가 필요한데
        FbxExBoneFrameData& NextData = PixAniData->AniFrameData[i].BoneMatData[NextFrame];


        // 로컬 스케일
        float4 LerpScale = float4::Lerp(CurData.S, NextData.S, CurFrameTime);
        // 로컬 쿼터니온
        float4 SLerpQ = float4::SLerp(CurData.Q, NextData.Q, CurFrameTime);
        // 로컬 포지션
        float4 LerpPos = float4::Lerp(CurData.T, NextData.T, CurFrameTime);

        // 


        size_t Size = sizeof(float4x4);
        // 자신의 원본행렬과 곱해준다 큰 의미는 없다.

        float4x4 Mat = float4x4::Affine(LerpScale, SLerpQ, LerpPos);

        // 학생들한테 값 보여준다고 
        //ParentRenderer->BoneData[i].Pos = LerpPos;
        //ParentRenderer->BoneData[i].Q = SLerpQ;
        //ParentRenderer->BoneData[i].Scale = LerpScale;

        //FrameBoneData[i].AniScaleMat = float4x4::Affine(LerpScale, float4::QIden, float4::ZERO);
        //FrameBoneData[i].AniRotMat = float4x4::Affine(float4::ONE, SLerpQ, float4::NONE);
        //FrameBoneData[i].AniPosMat = float4x4::Affine(float4::ONE, float4::QIden, LerpPos);

        // FrameBoneData[i].AniWorldMat = float4x4::Affine(LerpScale, SLerpQ, LerpPos);


        // 로컬에서의 애니메이션된 행렬
        ParentRenderer->BoneData[i] = BoneData->BonePos.Offset * float4x4::Affine(LerpScale, SLerpQ, LerpPos);

        // ParentRenderer->BoneData[i].Transpose();
    }

    
}