#include "PreCompile.h"
#include "GameEnginePreprocessingRenderer.h"

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

RenderSet& GameEnginePreprocessingRenderer::GetRenderSet(unsigned int _Index)
{
	return RenderSets_[_Index];
}

void GameEnginePreprocessingRenderer::SetBaseRenderer(GameEngineFBXRenderer* _BaseRenderer, std::string _PipeLineName, bool _IsCharacter, bool _IsSilhouett)
{
	// ���̽����Ǵ� ������ ����
	BaseRenderer_ = _BaseRenderer;
	FBXMesh_ = BaseRenderer_->GetMesh();
	if (nullptr != FBXMesh_)
	{
		SetMesh(_PipeLineName, _IsCharacter);
	}

	// �Ƿ翧�϶� ��ó�������� 
	if (true == _IsSilhouett)
	{
		OutLineData_.LineColor = float4(0.06f, 0.4f, 0.57f, 1.0f);
		OutLineData_.LineThickness = 0.0f;
	}
	// �ܰ����϶� ��ó��������
	else
	{
		OutLineData_.LineColor = float4::RED;
		OutLineData_.LineThickness = 2.0f;
	}

	// ���̽��������� ȸ��/ũ��/��ġ ����
	float4 Thickness = float4::ONE * OutLineData_.LineThickness;
	GetTransform()->SetLocalScaling(BaseRenderer_->GetTransform()->GetLocalScaling() + Thickness);
	GetTransform()->SetLocalRotationDegree(BaseRenderer_->GetTransform()->GetLocalRotation());
	GetTransform()->SetLocalPosition(BaseRenderer_->GetTransform()->GetLocalPosition());

	// 
	BaseRenderer_->SetOutLineRenderer(this);
}

void GameEnginePreprocessingRenderer::SetOutLineData(const float4& _LineColor, float _LineThickness)
{
	OutLineData_.LineColor = _LineColor;
	OutLineData_.LineThickness = _LineThickness;
}

void GameEnginePreprocessingRenderer::SetMesh(std::string _PipeLineName, bool _IsCharacter)
{
	std::vector<FbxMeshSet>& AllMeshSet = FBXMesh_->GetAllMeshMap();
	int AllMeshCount = static_cast<int>(AllMeshSet.size());
	if (true == _IsCharacter)
	{
		RenderSets_.resize(AllMeshCount);
		for (int i = 0; i < AllMeshCount; ++i)
		{
			SetFBXMeshRenderSetCharacter(_PipeLineName, i);
		}
	}
	else
	{
		for (int i = 0; i < AllMeshCount; ++i)
		{
			SetFBXMeshRenderSet(_PipeLineName, i);
		}
	}
}

void GameEnginePreprocessingRenderer::SetFBXMeshRenderSet(std::string _PipeLineName, int _MeshIndex)
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

			RenderSet& RenderSetData = RenderSets_.emplace_back();

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

			if (true == RenderSetData.ShaderHelper->IsConstantBuffer("OutLineData"))
			{
				RenderSetData.ShaderHelper->SettingConstantBufferLink("OutLineData", OutLineData_);
			}

			if (true == RenderSetData.ShaderHelper->IsTextureSetting("DiffuseTex"))
			{
				GameEngineTexture* Tex = GameEngineTextureManager::GetInst().Find(GameEnginePath::GetFileName(MatData->DifTexturePath));

				if ("" != MatData->DifTexturePath)
				{
					if (Tex == nullptr && true == GameEnginePath::IsExist(MatData->DifTexturePath))
					{
						GameEngineTextureManager::GetInst().Load(MatData->DifTexturePath);
						Tex = GameEngineTextureManager::GetInst().Find(GameEnginePath::GetFileName(MatData->DifTexturePath));
					}

					if (Tex != nullptr)
					{
						RenderSetData.ShaderHelper->SettingTexture("DiffuseTex", Tex);
					}
				}
			}


			if (true == RenderSetData.ShaderHelper->IsTextureSetting("NormalTex"))
			{
				GameEngineTexture* Tex = GameEngineTextureManager::GetInst().Find(GameEnginePath::GetFileName(MatData->NorTexturePath));

				if ("" != MatData->NorTexturePath)
				{
					if (Tex == nullptr && true == GameEnginePath::IsExist(MatData->NorTexturePath))
					{
						GameEngineTextureManager::GetInst().Load(MatData->NorTexturePath);
						Tex = GameEngineTextureManager::GetInst().Find(GameEnginePath::GetFileName(MatData->NorTexturePath));
						RenderSetData.ShaderHelper->SettingTexture("NormalTex", Tex);
						RendererDataInst.IsBump = 1;
					}
				}
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

void GameEnginePreprocessingRenderer::SetFBXMeshRenderSetCharacter(std::string _PipeLineName, int _MeshIndex)
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

			if (true == RenderSetData.ShaderHelper->IsConstantBuffer("OutLineData"))
			{
				RenderSetData.ShaderHelper->SettingConstantBufferLink("OutLineData", OutLineData_);
			}

			if (true == RenderSetData.ShaderHelper->IsTextureSetting("DiffuseTex"))
			{
				GameEngineTexture* Tex = GameEngineTextureManager::GetInst().Find(GameEnginePath::GetFileName(MatData->DifTexturePath));

				if ("" != MatData->DifTexturePath)
				{
					if (Tex == nullptr && true == GameEnginePath::IsExist(MatData->DifTexturePath))
					{
						GameEngineTextureManager::GetInst().Load(MatData->DifTexturePath);
						Tex = GameEngineTextureManager::GetInst().Find(GameEnginePath::GetFileName(MatData->DifTexturePath));
					}

					if (Tex != nullptr)
					{
						RenderSetData.ShaderHelper->SettingTexture("DiffuseTex", Tex);
					}
				}
			}


			if (true == RenderSetData.ShaderHelper->IsTextureSetting("NormalTex"))
			{
				GameEngineTexture* Tex = GameEngineTextureManager::GetInst().Find(GameEnginePath::GetFileName(MatData->NorTexturePath));

				if ("" != MatData->NorTexturePath)
				{
					if (Tex == nullptr && true == GameEnginePath::IsExist(MatData->NorTexturePath))
					{
						GameEngineTextureManager::GetInst().Load(MatData->NorTexturePath);
						Tex = GameEngineTextureManager::GetInst().Find(GameEnginePath::GetFileName(MatData->NorTexturePath));
						RenderSetData.ShaderHelper->SettingTexture("NormalTex", Tex);
						RendererDataInst.IsBump = 1;
					}
				}
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

void GameEnginePreprocessingRenderer::Start()
{
	// ������� ��ũ���� �ʱ�ȭ
	OutLineData_.LineColor = float4(0.06f, 0.4f, 0.57f, 1.0f);
	OutLineData_.LineThickness = 0.0f;
}

void GameEnginePreprocessingRenderer::Update(float _DeltaTime)
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

void GameEnginePreprocessingRenderer::Render(float _DeltaTime, bool _IsDeferred)
{
	for (size_t i = 0; i < RenderSets_.size(); i++)
	{
		if (false == RenderSets_[i].isRender)
		{
			continue;
		}

		if (true == _IsDeferred && true == RenderSets_[i].PipeLine_->IsDeferred())
		{
			RenderSets_[i].ShaderHelper->Setting();
			RenderSets_[i].PipeLine_->Rendering();
			RenderSets_[i].ShaderHelper->ReSet();
			RenderSets_[i].PipeLine_->Reset();
		}
		else if (false == RenderSets_[i].PipeLine_->IsDeferred())
		{
			RenderSets_[i].ShaderHelper->Setting();
			RenderSets_[i].PipeLine_->Rendering();
			RenderSets_[i].ShaderHelper->ReSet();
			RenderSets_[i].PipeLine_->Reset();
		}
	}
}

GameEnginePreprocessingRenderer::GameEnginePreprocessingRenderer()
	: OutLineData_{}
	, BaseRenderer_(nullptr)
	, FBXMesh_(nullptr)
{
}

GameEnginePreprocessingRenderer::~GameEnginePreprocessingRenderer()
{
	for (size_t i = 0; i < RenderSets_.size(); i++)
	{
		delete RenderSets_[i].PipeLine_;
		delete RenderSets_[i].ShaderHelper;
	}
}
