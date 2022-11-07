#include "PreCompile.h"
#include "GameEngineShadowRenderer.h"

#include "GameEngineRenderingPipeLine.h"
#include "GameEngineFBXMesh.h"
#include "GameEngineShaderResHelper.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineVertexShader.h"
#include "GameEngineIndexBuffer.h"
#include "GameEnginePixelShader.h"

void GameEngineShadowRenderer::SetBaseRenderer(GameEngineFBXRenderer* _BaseRenderer)
{
	// ���̽������� ����
	BaseRenderer_ = _BaseRenderer;
	if (nullptr != BaseRenderer_)
	{
		FBXMesh_ = BaseRenderer_->GetMesh();
	}

	// ������ ����
	std::vector<FbxMeshSet>& AllMeshSet = FBXMesh_->GetAllMeshMap();
	int AllMeshCount = static_cast<int>(AllMeshSet.size());

	RenderSets_.resize(AllMeshCount);									// �޸�ħ�������� ���Ͽ� �̸� �޸𸮰��� �Ҵ�
	for (int MeshNum = 0; MeshNum < AllMeshCount; ++MeshNum)
	{
		SetRenderSet(MeshNum);
	}

	// ���̽��������� �������� �˰����� �̿��Ͽ� �׸������·� ��ȯ
	ParallelogramAlgorithm();
}

void GameEngineShadowRenderer::SetRenderSet(int _MeshIndex)
{
	// �����º� ������������ �ش� ������������ �̿��Ͽ� �����ϱ����Ͽ� ���������� ��ü�� �����͸� ����
	GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Find(PipeLineName_);

	// ���������� ���� �� ������� ���� or ��ũ
	std::vector<FbxMeshSet>& AllMeshSet = FBXMesh_->GetAllMeshMap();
	FbxMeshSet& StartMesh = AllMeshSet[_MeshIndex];

	std::vector<GameEngineVertexBuffer*>& VertexBufferVector = StartMesh.GameEngineVertexBuffers;
	for (int VertexBufferIndex = 0; VertexBufferIndex < static_cast<int>(VertexBufferVector.size()); ++VertexBufferIndex)
	{
		std::vector<GameEngineIndexBuffer*>& IndexBufferVector = StartMesh.GameEngineIndexBuffers[VertexBufferIndex];
		for (int IndexBufferIndex = 0; IndexBufferIndex < static_cast<int>(IndexBufferVector.size()); ++IndexBufferIndex)
		{
			FbxExRenderingPipeLineSettingData* MatData = &(StartMesh.MaterialData[VertexBufferIndex][IndexBufferIndex]);
			GameEngineVertexBuffer* VertexBuffer = VertexBufferVector[VertexBufferIndex];
			GameEngineIndexBuffer* IndexBuffer = IndexBufferVector[IndexBufferIndex];

			// �̸��Ҵ��س��� RenderSet ��Ͽ��� ���� �޽��� ���� ������ Get
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

			// Ʈ������������ ��ũ
			if (true == RenderSetData.ShaderHelper->IsConstantBuffer("TransformData"))
			{
				RenderSetData.ShaderHelper->SettingConstantBufferLink("TransformData", GetTransform()->GetTransformData());
			}

			// ���������� ��ũ
			if (true == RenderSetData.ShaderHelper->IsConstantBuffer("RendererData"))
			{
				RenderSetData.ShaderHelper->SettingConstantBufferLink("RendererData", RendererDataInst);
			}

			// �ִϸ��̼��� ���� �������� ��ũ
			if (true == RenderSetData.ShaderHelper->IsStructuredBuffer("ArrAniMationMatrix"))
			{
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

void GameEngineShadowRenderer::ParallelogramAlgorithm()
{
	// ���̽������� �������� ����Ÿ�ٿ� ������ �� �ش� ����Ÿ���� ȸ������ �׸��ڸ� �����
	switch (ShadowDir_)
	{
		case ShadowDir::NONE:			// �ٴ�
		{

			break;
		}
		case ShadowDir::TOP:			// ���
		{

			break;
		}
		case ShadowDir::RIGHTTOP:		// �����ʻ��
		{

			break;
		}
		case ShadowDir::RIGHT:			// ������
		{

			break;
		}
		case ShadowDir::RIGHTBOTTOM:	// �������ϴ�
		{

			break;
		}
		case ShadowDir::BOTTOM:			// �ϴ�
		{

			break;
		}
		case ShadowDir::LEFTBOTTOM:		// �����ϴ�
		{

			break;
		}
		case ShadowDir::LEFT:			// ����
		{

			break;
		}
		case ShadowDir::LEFTTOP:		// ���ʻ��
		{

			break;
		}
	}

	// ...



}

void GameEngineShadowRenderer::Start()
{
	PipeLineName_ = "LightShadow";
	SetRenderingPipeLine(PipeLineName_);
}

void GameEngineShadowRenderer::Update(float _DeltaTime)
{
	// ���̽��������� �������͸� ������ ���� �������� �������� ���� ����
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
	for (int MeshNum = 0; static_cast<int>(MeshNum < RenderSets_.size()); ++MeshNum)
	{
		if (false == RenderSets_[MeshNum].isRender)
		{
			continue;
		}

		RenderSets_[MeshNum].ShaderHelper->Setting();
		RenderSets_[MeshNum].PipeLine_->Rendering();
		RenderSets_[MeshNum].ShaderHelper->ReSet();
		RenderSets_[MeshNum].PipeLine_->Reset();
	}
}

GameEngineShadowRenderer::GameEngineShadowRenderer()
	: PipeLineName_{}
	, FBXMesh_(nullptr)
	, BaseRenderer_(nullptr)
	, ShadowDir_(ShadowDir::NONE)
{
}

GameEngineShadowRenderer::~GameEngineShadowRenderer()
{
}
