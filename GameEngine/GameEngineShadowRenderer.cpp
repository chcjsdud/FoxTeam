#include "PreCompile.h"
#include "GameEngineShadowRenderer.h"

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
#include "GameEngineTextureManager.h"

void GameEngineShadowRenderer::SetBaseRenderer(GameEngineFBXRenderer* _BaseRenderer, std::string _PipeLineName)
{
	// ���̽����Ǵ� ������ ����
	BaseRenderer_ = _BaseRenderer;
	FBXMesh_ = BaseRenderer_->GetMesh();
	if (nullptr != FBXMesh_)
	{
		std::vector<FbxMeshSet>& AllMeshSet = FBXMesh_->GetAllMeshMap();
		int AllMeshCount = static_cast<int>(AllMeshSet.size());
		RenderSets_.resize(AllMeshCount);
		for (int i = 0; i < AllMeshCount; ++i)
		{
			SetFBXMeshRender(_PipeLineName, i);
		}
	}

	// ���̽��������� Ʈ�����������͸� �̿��Ͽ� �ش� �������� ȸ���Ͽ� �׸�������ó�� ���� - ó����...
	float4 BaseForwardVector = BaseRenderer_->GetTransform()->GetLocalForwardVector();



	GetTransform()->SetLocalScaling(BaseRenderer_->GetTransform()->GetLocalScaling());			// ũ��
	//GetTransform()->SetLocalPosition(BaseRenderer_->GetTransform()->GetLocalPosition() + float4(200.0f, 0.0f, 0.0f));		// ��ġ
	GetTransform()->SetLocalPosition(BaseRenderer_->GetTransform()->GetLocalPosition());		// ��ġ

	GetTransform()->SetLocalRotationDegree(BaseRenderer_->GetTransform()->GetLocalRotation());	// ȸ��

	



	
	

	// ���� �������� ���� ī�޶��� ��Ͽ� �ش� ������ �߰�
	BaseRenderer_->SetShadowRenderer(this);
}

void GameEngineShadowRenderer::SetFBXMeshRender(std::string _PipeLineName, int _MeshIndex)
{
	GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Find(_PipeLineName);

	std::vector<FbxMeshSet>& AllMeshSet = FBXMesh_->GetAllMeshMap();
	FbxMeshSet& StartMesh = AllMeshSet[_MeshIndex];

	std::vector<GameEngineVertexBuffer*>& VertexBufferVector = StartMesh.GameEngineVertexBuffers;
	for (int VertexBufferIndex = 0; VertexBufferIndex < VertexBufferVector.size(); VertexBufferIndex++)
	{
		std::vector<GameEngineIndexBuffer*>& IndexBufferVector = StartMesh.GameEngineIndexBuffers[VertexBufferIndex];
		for (int IndexBufferIndex = 0; IndexBufferIndex < IndexBufferVector.size(); IndexBufferIndex++)
		{
			FbxExRenderingPipeLineSettingData* MatData = &(StartMesh.MaterialData[VertexBufferIndex][IndexBufferIndex]);
			GameEngineVertexBuffer* VertexBuffer = VertexBufferVector[VertexBufferIndex];
			GameEngineIndexBuffer* IndexBuffer = IndexBufferVector[IndexBufferIndex];

			RenderSet& RenderSetData = RenderSets_[_MeshIndex];

			float4 AllVtxPos = float4::ZERO;
			float4 MaxVtxValue = float4::ZERO;

			for (size_t i = 0; i < StartMesh.Vertexs.size(); i++)
			{
				float4 Vtx = StartMesh.Vertexs[i].POSITION;

				AllVtxPos += Vtx;

				if (abs(Vtx.x) > abs(MaxVtxValue.x))
				{
					MaxVtxValue.x = Vtx.x;
				}

				if (abs(Vtx.y) > abs(MaxVtxValue.y))
				{
					MaxVtxValue.y = Vtx.y;
				}

				if (abs(Vtx.z) > abs(MaxVtxValue.z))
				{
					MaxVtxValue.z = Vtx.z;
				}
			}

			RenderSetData.LocalPos = AllVtxPos / static_cast<float>(StartMesh.Vertexs.size());

			float4 fScale = MaxVtxValue - RenderSetData.LocalPos;

			RenderSetData.Radius = max(max(abs(fScale.x), abs(fScale.y)), abs(fScale.z));
			RenderSetData.Index = _MeshIndex;

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

			if (true == RenderSetData.ShaderHelper->IsConstantBuffer("RendererData"))
			{
				RenderSetData.ShaderHelper->SettingConstantBufferLink("RendererData", RendererDataInst);
			}

			if (true == RenderSetData.ShaderHelper->IsStructuredBuffer("ArrAniMationMatrix"))
			{
				// ���̽��������� �������͸� ��ũ
				RenderSetData.BoneData = BaseRenderer_->GetAllRenderSetValue()[_MeshIndex].BoneData;
				RenderSetData.ShaderHelper->SettingStructuredBufferSetting("ArrAniMationMatrix", FBXMesh_->GetAnimationBuffer(VertexBufferIndex));
				RenderSetData.ShaderHelper->SettingStructuredBufferLink("ArrAniMationMatrix", &RenderSetData.BoneData[0], sizeof(float4x4) * RenderSetData.BoneData.size());
				RendererDataInst.IsAni = 1;
			}

			RenderSetData.PipeLine_->SetInputAssembler1VertexBufferSetting(VertexBuffer);
			RenderSetData.PipeLine_->SetInputAssembler2IndexBufferSetting(IndexBuffer);
		}
	}
}

void GameEngineShadowRenderer::Start()
{
}

void GameEngineShadowRenderer::Update(float _DeltaTime)
{	
	// ���̽��� ������ �������ͷ� �� �������͸� ����
	if (nullptr != BaseRenderer_)
	{
		for (size_t i = 0; i < BaseRenderer_->GetAllRenderSet().size(); ++i)
		{
			RenderSets_[i].BoneData = BaseRenderer_->GetAllRenderSet()[i].BoneData;
		}
	}
}

void GameEngineShadowRenderer::Render(float _DeltaTime, bool _IsDeferred)
{
	for (size_t i = 0; i < RenderSets_.size(); i++)
	{
		if (false == RenderSets_[i].isRender)
		{
			continue;
		}

		RenderSets_[i].ShaderHelper->Setting();
		RenderSets_[i].PipeLine_->Rendering();
		RenderSets_[i].ShaderHelper->ReSet();
		RenderSets_[i].PipeLine_->Reset();
	}
}

GameEngineShadowRenderer::GameEngineShadowRenderer()
	: BaseRenderer_(nullptr)
	, FBXMesh_(nullptr)
{
}

GameEngineShadowRenderer::~GameEngineShadowRenderer()
{
	for (size_t i = 0; i < RenderSets_.size(); i++)
	{
		delete RenderSets_[i].PipeLine_;
		delete RenderSets_[i].ShaderHelper;
	}
}
