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
#include "GameEngineTextureManager.h"


GameEngineFBXRenderer::GameEngineFBXRenderer()
	: currentAnimation_(nullptr)
	, overrideAnimation_(nullptr)
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

	for (auto& Animation : Animations)
	{
		delete Animation.second;
	}


}


void GameEngineFBXRenderer::Render(float _DeltaTime, bool _IsDeferred)
{
	for (size_t i = 0; i < RenderSets.size(); i++)
	{
		if (false == RenderSets[i].isRender)
		{
			continue;
		}

		if (true == _IsDeferred
			&& true == RenderSets[i].PipeLine_->IsDeferred())
		{
			RenderSets[i].ShaderHelper->Setting();
			RenderSets[i].PipeLine_->Rendering();
			RenderSets[i].ShaderHelper->ReSet();
			RenderSets[i].PipeLine_->Reset();
		}
		else if (false == RenderSets[i].PipeLine_->IsDeferred())
		{
			RenderSets[i].ShaderHelper->Setting();
			RenderSets[i].PipeLine_->Rendering();
			RenderSets[i].ShaderHelper->ReSet();
			RenderSets[i].PipeLine_->Reset();
		}
	}
}

void GameEngineFBXRenderer::SetFBXMeshRenderSet(const std::string& _Value, std::string _PipeLine, int _MeshIndex)
{
	FBXMesh = GameEngineFBXMeshManager::GetInst().Find(_Value);

	if (nullptr == FBXMesh)
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� fbx�Ž��� �����߽��ϴ�.");
	}

	GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Find(_PipeLine);

	std::vector<FbxMeshSet>& AllMeshSet = FBXMesh->GetAllMeshMap();

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

			RenderSet& RenderSetData = RenderSets.emplace_back();

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
				FBXMesh->ImportBone();
				if (0 == RenderSetData.BoneData.size())
				{
					RenderSetData.BoneData.resize(FBXMesh->GetBoneCount(VertexBufferIndex));
				}

				const LightsData& LightData = GetLevel()->GetMainCamera()->GetLightData();
				// 15���� �����ϰ� �����ϴµ� �� ���
				// ��? 1���� �ص� �Ǵ°� 15���� �ϰ� �ɰ��̴�.
				RenderSetData.ShaderHelper->SettingStructuredBufferSetting("ArrAniMationMatrix", FBXMesh->GetAnimationBuffer(VertexBufferIndex));
				RenderSetData.ShaderHelper->SettingStructuredBufferLink("ArrAniMationMatrix", &RenderSetData.BoneData[0], sizeof(float4x4) * RenderSetData.BoneData.size());

				RendererDataInst.IsAni = 1;
			}

			RenderSetData.PipeLine_->SetInputAssembler1VertexBufferSetting(VertexBuffer);
			RenderSetData.PipeLine_->SetInputAssembler2IndexBufferSetting(IndexBuffer);
		}
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

	std::vector<FbxMeshSet>& AllMeshSet = FBXMesh->GetAllMeshMap();

	for (int i = 0; i < AllMeshSet.size(); ++i)
	{
		SetFBXMeshRenderSet(_Value, _PipeLine, i);

		// FbxMeshSet& StartMesh = AllMeshSet[i];

	}
}

void GameEngineFBXRenderer::Start()
{
	GetLevel()->GetMainCamera()->PushRenderer(GetOrder(), this);
}

void GameEngineFBXRenderer::Update(float _DeltaTime)
{
	if (nullptr != currentAnimation_)
	{
		currentAnimation_->Update(_DeltaTime);
	}
}


void GameEngineFBXRenderer::CreateFBXAnimation(const std::string& _AnimationName, const std::string& _AnimationFBXName, int _AnimationIndex, bool _isLoop, float _frameTime)
{   // 0805 �ִϸ��̼� �������� �߰�
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
		GameEngineDebug::MsgBoxError("�ε��Ϸ��� �ִϸ��̼� ������ �����ϴ�.");
		return;
	}

	FBXAnimation* NewFBXAnimation = new FBXAnimation();

	NewFBXAnimation->Mesh = FBXMesh;
	NewFBXAnimation->Animation = Animation;
	NewFBXAnimation->ParentRenderer = this;
	NewFBXAnimation->AnimationName_ = _AnimationName;
	NewFBXAnimation->Init(_AnimationIndex, _isLoop, _frameTime);


	Animations.insert(std::make_pair(_AnimationName, NewFBXAnimation));

}

void GameEngineFBXRenderer::ChangeFBXAnimation(const std::string& _AnimationName, bool _bForceChange)
{
	std::map<std::string, FBXAnimation*>::iterator FindIter = Animations.find(_AnimationName);

	if (Animations.end() == FindIter)
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
		return;
	}

	if (_bForceChange == false && currentAnimation_ == FindIter->second)
	{
		return;
	}

	currentAnimation_ = FindIter->second;
	currentAnimation_->ResetFrame();
}

void GameEngineFBXRenderer::OverrideFBXAnimation(const std::string& _AnimationName, const std::string& _boneNameToAffect, bool _bForceChange)
{
	if (currentAnimation_ == nullptr)
	{
		GameEngineDebug::MsgBoxError("currentAnimation_ is nullptr");
		return;
	}

	std::map<std::string, FBXAnimation*>::iterator FindIter = Animations.find(_AnimationName);

	if (Animations.end() == FindIter)
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
		return;
	}

	if (_bForceChange == false && overrideAnimation_ == FindIter->second)
	{
		return;
	}

	overrideAnimation_ = FindIter->second;
	overrideAnimation_->ResetFrame();
}

bool GameEngineFBXRenderer::IsCurrentAnimationEnd()
{
	if (nullptr == currentAnimation_)
	{
		return false;
	}

	return currentAnimation_->bEnd_;
}


bool GameEngineFBXRenderer::CheckIntersects(const float4& _Position,
	const float4& _Direction, float& _Distance)
{
	std::vector<FbxMeshSet>& vecMeshMap = FBXMesh->GetAllMeshMap();
	std::vector<FbxExMeshInfo>& vecMeshInfos = FBXMesh->GetMeshInfos();

	bool Check = false;

	float4 Dir = _Direction.NormalizeReturn3D();

	for (size_t i = 0; i < vecMeshMap.size(); i++)
	{
		for (size_t j = 0; j < vecMeshMap[i].Indexs[0].size(); ++j)
		{
			for (size_t k = 0; k < vecMeshInfos[i].FaceNum; k++)
			{
				float4 V0 = vecMeshMap[i].Vertexs[vecMeshMap[i].Indexs[0][j][k * 3 + 0]].POSITION * GetTransform()->GetTransformData().WorldWorld_;
				float4 V1 = vecMeshMap[i].Vertexs[vecMeshMap[i].Indexs[0][j][k * 3 + 1]].POSITION * GetTransform()->GetTransformData().WorldWorld_;
				float4 V2 = vecMeshMap[i].Vertexs[vecMeshMap[i].Indexs[0][j][k * 3 + 2]].POSITION * GetTransform()->GetTransformData().WorldWorld_;

				Check = DirectX::TriangleTests::Intersects(_Position.DirectVector,
					Dir.DirectVector,
					V0.DirectVector,
					V1.DirectVector,
					V2.DirectVector,
					_Distance);

				if (true == Check)
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool GameEngineFBXRenderer::CheckMeshToPointCollision(const float4& _Position,
	const float4& _Direction, const float _Range)
{
	float dist = 0.0f;

	if (false == CheckIntersects(_Position, _Direction, dist))
	{
		return false;
	}

	if (_Range >= dist
		&& 0.0f < dist)
	{
		return true;
	}

	return false;
}
