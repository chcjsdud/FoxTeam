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
	// 베이스렌더러 저장
	BaseRenderer_ = _BaseRenderer;
	if (nullptr != BaseRenderer_)
	{
		FBXMesh_ = BaseRenderer_->GetMesh();
	}

	// 렌더셋 셋팅
	std::vector<FbxMeshSet>& AllMeshSet = FBXMesh_->GetAllMeshMap();
	int AllMeshCount = static_cast<int>(AllMeshSet.size());

	RenderSets_.resize(AllMeshCount);									// 메모리침범방지를 위하여 미리 메모리공간 할당
	for (int MeshNum = 0; MeshNum < AllMeshCount; ++MeshNum)
	{
		SetRenderSet(MeshNum);
	}

	// 베이스렌더러를 평행사면형 알고리즘을 이용하여 그림자형태로 변환
	ParallelogramAlgorithm();
}

void GameEngineShadowRenderer::SetRenderSet(int _MeshIndex)
{
	// 렌더셋별 파이프라인을 해당 파이프라인을 이용하여 복사하기위하여 파이프라인 객체의 포인터를 얻어옴
	GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Find(PipeLineName_);

	// 파이프라인 복제 및 상수버퍼 셋팅 or 링크
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

			// 미리할당해놓은 RenderSet 목록에서 현재 메쉬의 공간 포인턴 Get
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

			// 트랜스폼데이터 링크
			if (true == RenderSetData.ShaderHelper->IsConstantBuffer("TransformData"))
			{
				RenderSetData.ShaderHelper->SettingConstantBufferLink("TransformData", GetTransform()->GetTransformData());
			}

			// 렌더데이터 링크
			if (true == RenderSetData.ShaderHelper->IsConstantBuffer("RendererData"))
			{
				RenderSetData.ShaderHelper->SettingConstantBufferLink("RendererData", RendererDataInst);
			}

			// 애니메이션을 위한 본데이터 링크
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
	// 베이스렌더러 기준으로 렌더타겟에 렌더링 후 해당 렌더타겟을 회전시켜 그림자를 만든다
	switch (ShadowDir_)
	{
		case ShadowDir::NONE:			// 바닥
		{

			break;
		}
		case ShadowDir::TOP:			// 상단
		{

			break;
		}
		case ShadowDir::RIGHTTOP:		// 오른쪽상단
		{

			break;
		}
		case ShadowDir::RIGHT:			// 오른쪽
		{

			break;
		}
		case ShadowDir::RIGHTBOTTOM:	// 오른쪽하단
		{

			break;
		}
		case ShadowDir::BOTTOM:			// 하단
		{

			break;
		}
		case ShadowDir::LEFTBOTTOM:		// 왼쪽하단
		{

			break;
		}
		case ShadowDir::LEFT:			// 왼쪽
		{

			break;
		}
		case ShadowDir::LEFTTOP:		// 왼쪽상단
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
	// 베이스렌더러의 본데이터를 가져와 나의 렌더셋의 본데이터 정보 갱신
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
