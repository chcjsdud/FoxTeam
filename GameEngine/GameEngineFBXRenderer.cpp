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
        GameEngineDebug::MsgBoxError("�������� �ʴ� fbx�Ž��� �����߽��ϴ�.");
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
                    // 15���� �����ϰ� �����ϴµ� �� ���
                    // ��? 1���� �ص� �Ǵ°� 15���� �ϰ� �ɰ��̴�.
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
        GameEngineDebug::MsgBoxError("�Ž��� �������� ���� �������� �ִϸ��̼��� ���� ���� �����ϴ�.");
        return;
    }

    if (Animations.end() != Animations.find(_AnimationName)) 
    {
        GameEngineDebug::MsgBoxError("���� �̸��� �ִϸ��̼��� �� ������� �����ϴ�.");
        return;
    }

    GameEngineFBXAnimation* Animation = GameEngineFBXAnimationManager::GetInst().Find(_AnimationFBXName);

    if (nullptr == Animation)
    {
        GameEngineDebug::MsgBoxError("���� �̸��� �ִϸ��̼��� �� ������� �����ϴ�.");
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
        GameEngineDebug::MsgBoxError("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
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
    // ��� ����

    // 0~24��������?
    CurFrameTime += _DeltaTime;
    //                      0.1
    if (CurFrameTime >= FrameTime)
    {
        // ������ �ð��� ���Ե���?
        // ������ �ð��� �߿��մϴ�.
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


    // �Ʊ �̾߱������� �̳༮�� 64���� ���� ������ �����ϱ�
    // ������ n��° �������� 64���� �ִϸ��̼� ���¸� �������� �մϴ�.

    for (int i = 0; i < ParentRenderer->BoneData.size(); i++)
    {
        Bone* BoneData = ParentRenderer->FBXMesh->FindBone(i);

        if (true == PixAniData->AniFrameData[i].BoneMatData.empty())
        {
            // ���� �����ӵ����Ͱ� �������� �ֽ��ϴ�.
            // ���� �׵������ �� �ִ� ��찡 �����ϴ�.

            // GameEngineDebug::MsgBoxError("������� ������!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            ParentRenderer->BoneData[i] = float4x4::Affine(BoneData->BonePos.GlobalScale, BoneData->BonePos.GlobalRotation, BoneData->BonePos.GlobalTranslation);
            return;
        }

        // ���������Ӱ� 
        FbxExBoneFrameData& CurData = PixAniData->AniFrameData[i].BoneMatData[CurFrame];
        // ������������ ������ �ʿ��ѵ�
        FbxExBoneFrameData& NextData = PixAniData->AniFrameData[i].BoneMatData[NextFrame];


        // ���� ������
        float4 LerpScale = float4::Lerp(CurData.S, NextData.S, CurFrameTime);
        // ���� ���ʹϿ�
        float4 SLerpQ = float4::SLerp(CurData.Q, NextData.Q, CurFrameTime);
        // ���� ������
        float4 LerpPos = float4::Lerp(CurData.T, NextData.T, CurFrameTime);

        // 


        size_t Size = sizeof(float4x4);
        // �ڽ��� ������İ� �����ش� ū �ǹ̴� ����.

        float4x4 Mat = float4x4::Affine(LerpScale, SLerpQ, LerpPos);

        // �л������� �� �����شٰ� 
        //ParentRenderer->BoneData[i].Pos = LerpPos;
        //ParentRenderer->BoneData[i].Q = SLerpQ;
        //ParentRenderer->BoneData[i].Scale = LerpScale;

        //FrameBoneData[i].AniScaleMat = float4x4::Affine(LerpScale, float4::QIden, float4::ZERO);
        //FrameBoneData[i].AniRotMat = float4x4::Affine(float4::ONE, SLerpQ, float4::NONE);
        //FrameBoneData[i].AniPosMat = float4x4::Affine(float4::ONE, float4::QIden, LerpPos);

        // FrameBoneData[i].AniWorldMat = float4x4::Affine(LerpScale, SLerpQ, LerpPos);


        // ���ÿ����� �ִϸ��̼ǵ� ���
        ParentRenderer->BoneData[i] = BoneData->BonePos.Offset * float4x4::Affine(LerpScale, SLerpQ, LerpPos);

        // ParentRenderer->BoneData[i].Transpose();
    }

    
}