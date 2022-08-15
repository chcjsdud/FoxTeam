#include "PreCompile.h"
#include "GHMap.h"

#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/EngineVertex.h>


GHMap::GHMap()
	: navMeshRenderer_(nullptr)
{

}

GHMap::~GHMap()
{

}

void GHMap::Start()
{
	navMeshRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	navMeshRenderer_->SetFBXMesh("Bg_NaviMesh_Cobalt.fbx", "TextureDeferredLight");

	size_t count = navMeshRenderer_->GetRenderSetCount();
	for (size_t i = 0; i < count; i++)
	{
		navMeshRenderer_->GetRenderSet(static_cast<unsigned int>(i)).PipeLine_->SetRasterizer("EngineBaseRasterizerWireFrame");
	}

	navMeshRenderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	navMeshRenderer_->GetTransform()->SetLocalPosition({ 0.0f, -20.f });

	//navMeshRenderer_->Off();

	// 모든 메쉬를 네비메쉬로

	GameEngineFBXMesh* fbxMesh = navMeshRenderer_->GetMesh();

	std::vector<FbxMeshSet>& allMeshSet = fbxMesh->GetAllMeshMap();
	std::vector<FbxExMeshInfo>& allMeshInfo = fbxMesh->GetMeshInfos();
	size_t meshSetCount = allMeshSet.size();

	size_t triangleCount = 0;
	for (FbxExMeshInfo& info : allMeshInfo)
	{
		triangleCount += info.FaceNum;
	}

	navMeshes_.reserve(triangleCount);

	size_t navMeshCounter = 0;
	for (size_t i = 0; i < meshSetCount; ++i)
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

			newNavMesh.Index = static_cast<int>(navMeshCounter);

			navMeshes_.push_back(newNavMesh);

			++navMeshCounter;
		}
	}


	// 네비메쉬 인접성 검사

	size_t navMeshCount = navMeshes_.size();
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
				currentMesh.AdjacentMeshIndices.push_back(static_cast<int>(meshIndex));
			}
		}
	}


	//GameEngineVertexBuffer* NewVertex = new GameEngineVertexBuffer();
	//GameEngineIndexBuffer* NewIndex = new GameEngineIndexBuffer();

	//std::vector<GameEngineVertex> vertices;
	//std::vector<UINT> indices;
	//vertices.reserve(triangleCount);
	//indices.reserve(triangleCount * 3);

	//int counter = 0;
	//for (auto n : navMeshes_)
	//{
	//	for (size_t i = 0; i < 3; i++)
	//	{
	//		GameEngineVertex v;
	//		v.POSITION = n.Vertices[i];
	//		vertices.push_back(v);
	//		indices.push_back(counter++);
	//	}
	//}

	//NewVertex->Create(vertices, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	//NewIndex->Create(indices, D3D11_USAGE::D3D11_USAGE_DEFAULT);

	//GameEngineRenderer* Renderer = CreateTransformComponent<GameEngineRenderer>(GetTransform());
	//Renderer->SetRenderingPipeLine("Color");
	//Renderer->SetMesh(NewVertex, NewIndex);
	//Renderer->ShaderHelper.SettingConstantBufferLink("ResultColor", float4::BLUE);
	//Renderer->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });

	//Renderer->GetGameEngineRenderingPipeLine()->SetRasterizer("EngineBaseRasterizerWireFrame");
}

void GHMap::Update(float _deltaTime)
{
}

GHNavMesh* GHMap::GetCurrentNavMesh(const float4& _position)
{
	float4x4 matWorld = navMeshRenderer_->GetTransform()->GetTransformData().WorldWorld_;
	float distance = 0.f;
	for (GHNavMesh& nav : navMeshes_)
	{
		if (DirectX::TriangleTests::Intersects(_position.DirectVector, float4::DOWN.DirectVector,
			(nav.Vertices[0] * matWorld).DirectVector, (nav.Vertices[1] * matWorld).DirectVector, (nav.Vertices[2] * matWorld).DirectVector,
			distance))
		{
			return &nav;
		}
	}
	return nullptr;
}

bool GHMap::IsIntersectionMesh(const GHNavMesh& _mesh, const float4& _position)
{
	float4x4 matWorld = navMeshRenderer_->GetTransform()->GetTransformData().WorldWorld_;
	float distance = 0;
	return DirectX::TriangleTests::Intersects(_position.DirectVector, float4::DOWN.DirectVector,
		(_mesh.Vertices[0] * matWorld).DirectVector, (_mesh.Vertices[1] * matWorld).DirectVector, (_mesh.Vertices[2] * matWorld).DirectVector,
		distance);
}

GHNavMesh* GHMap::FindAdjacentMeshIntersect(const GHNavMesh& _currentMesh, const float4& _position)
{
	// 전체 탐색
	for (GHNavMesh& n : navMeshes_)
	{
		if (IsIntersectionMesh(n, _position))
		{
			return &n;
		}
	}

	//size_t adjacentMeshCount = _currentMesh.AdjacentMeshIndices.size();
	//for (size_t i = 0; i < adjacentMeshCount; i++)
	//{
	//	int index = _currentMesh.AdjacentMeshIndices[i];

	//	if (true == IsIntersectionMesh(navMeshes_[index], _position))
	//	{
	//		return &navMeshes_[index];
	//	}
	//}

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
