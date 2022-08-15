#include "PreCompile.h"
#include "GHMap.h"

#include <GameEngine/GameEngineFBXRenderer.h>



GHMap::GHMap()
	: renderer_(nullptr)
{

}

GHMap::~GHMap()
{

}

void GHMap::Start()
{
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh("Bg_NaviMesh_Cobalt.fbx", "TextureDeferredLight");

	int count = renderer_->GetRenderSetCount();
	for (int i = 0; i < count; i++)
	{
		renderer_->GetRenderSet(i).PipeLine_->SetRasterizer("EngineBaseRasterizerWireFrame");
	}

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });


	GameEngineTime::GetInst().TimeCheck();

	// 모든 메쉬를 네비메쉬로
	{
		GameEngineFBXMesh* fbxMesh = renderer_->GetMesh();

		std::vector<FbxMeshSet>& allMeshSet = fbxMesh->GetAllMeshMap();
		std::vector<FbxExMeshInfo>& allMeshInfo = fbxMesh->GetMeshInfos();
		int meshSetCount = allMeshSet.size();

		size_t triangleCount = 0;
		for (FbxExMeshInfo& info : allMeshInfo)
		{
			triangleCount += info.FaceNum;
		}

		navMeshes_.reserve(triangleCount);

		int navMeshCounter = 0;
		for (int i = 0; i < meshSetCount; ++i)
		{
			FbxMeshSet* mesh = &allMeshSet[i];
			FbxExMeshInfo* meshInfo = &allMeshInfo[i];

			int numTriangle = meshInfo->FaceNum;
			for (int j = 0; j < numTriangle; ++j)
			{

				GHNavMesh newNavMesh;
				newNavMesh.Vertices[0] = mesh->Vertexs[mesh->Indexs[0][0][j * 3 + 0]].POSITION;
				newNavMesh.Vertices[1] = mesh->Vertexs[mesh->Indexs[0][0][j * 3 + 1]].POSITION;
				newNavMesh.Vertices[2] = mesh->Vertexs[mesh->Indexs[0][0][j * 3 + 2]].POSITION;

				newNavMesh.Index = navMeshCounter;

				navMeshes_.push_back(newNavMesh);

				++navMeshCounter;
			}
		}
	}

	// 네비메쉬 인접성 검사
	{
		int navMeshCount = navMeshes_.size();
		for (GHNavMesh& currentMesh : navMeshes_)
		{
			for (size_t meshIndex = 0; meshIndex < navMeshCount; meshIndex++)
			{
				if (meshIndex == currentMesh.Index)
				{
					continue;
				}

				if (true == checkNavMeshAdjacency(currentMesh, navMeshes_[meshIndex]))
				{
					currentMesh.AdjacentMeshIndex.push_back(meshIndex);
				}
			}
		}
	}

	GameEngineTime::GetInst().TimeCheck();

	float elapsedTime = GameEngineTime::GetInst().GetDeltaTime();
	int a = 0;
}

void GHMap::Update(float _deltaTime)
{
}

GHNavMesh* GHMap::GetCurrentNavMesh(const float4& _position)
{
	for (GHNavMesh& nav : navMeshes_)
	{
		float distance = 0;
		if (DirectX::TriangleTests::Intersects(_position.DirectVector, float4::DOWN.DirectVector,
			nav.Vertices[0].DirectVector, nav.Vertices[1].DirectVector, nav.Vertices[2].DirectVector,
			distance))
		{
			return &nav;
		}
	}
	return nullptr;
}

bool GHMap::checkNavMeshAdjacency(const GHNavMesh& _left, const GHNavMesh& _right)
{
	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			if (_left.Vertices[i] == _right.Vertices[j])
			{
				return true;
			}
		}
	}

	return false;
}
