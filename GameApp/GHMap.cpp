#include "PreCompile.h"
#include "GHMap.h"

#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/EngineVertex.h>
#include <GameEngine/GameEngineCollision.h>
#include <numeric>


GHMap::GHMap()
	: navMeshRenderer_(nullptr)
	, tileVertexBuffer_(nullptr)
	, tileIndexBuffer_(nullptr)
	, intervalX_(0.0f)
	, intervalZ_(0.0f)
	, gridStartX_(0.0f)
	, gridStartZ_(0.0f)
	, gridZCount_(0)
	, gridXCount_(0)
{

}

GHMap::~GHMap()
{
	if (nullptr != tileVertexBuffer_)
	{
		delete tileVertexBuffer_;
	}

	if (nullptr != tileIndexBuffer_)
	{
		delete tileIndexBuffer_;
	}
}

void GHMap::Start()
{
	navMeshRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	navMeshRenderer_->SetFBXMesh("Bg_NaviMesh_Cobalt.fbx", "TextureDeferredLight");
	//navMeshRenderer_->SetFBXMesh("Bg_NaviMesh.fbx", "TextureDeferredLight");

	size_t count = navMeshRenderer_->GetRenderSetCount();
	for (size_t i = 0; i < count; i++)
	{
		navMeshRenderer_->GetRenderSet(static_cast<unsigned int>(i)).PipeLine_->SetRasterizer("EngineBaseRasterizerWireFrame");
	}

	navMeshRenderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	navMeshRenderer_->GetTransform()->SetLocalPosition({ 0.0f, -50.f });

	//navMeshRenderer_->Off();

	// ��� �޽��� �׺�޽���

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

	// �׺�޽� ������ �˻�

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

	makeAStarNode(50.f, 50.f);
	checkASterNodeObstacle();
	updateAStarNodeVertexInfo();

	//FindPath(float4::ZERO, float4(350.f, 0.0f, 214.f));

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
	// ��ü Ž��
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

std::vector<float4> GHMap::FindPath(const float4& _startPosition, const float4& _endPosition)
{
	std::vector<float4> result;

	// �׸����� ��ŸƮ ����Ʈ �ε����� ���� ����Ʈ �ε����� ���Ѵ�.
	int startIndexX = static_cast<int>((_startPosition.x - gridStartX_) / intervalX_ + 0.5f); // �ݿø�ó��
	int startIndexZ = static_cast<int>((_startPosition.z - gridStartZ_) / intervalZ_ + 0.5f);

	int endIndexX = static_cast<int>((_endPosition.x - gridStartX_) / intervalX_ + 0.5f);
	int endIndexZ = static_cast<int>((_endPosition.z - gridStartZ_) / intervalZ_ + 0.5f);

	// �������� �׸��带 ����� ���, Ȥ�� �������� �̵� �Ұ����� ���� ��� ���� ó��
	if (endIndexX < 0 || endIndexZ < 0 || endIndexX >= gridXCount_ || endIndexZ >= gridZCount_)
	{
		result.push_back(_endPosition);
		return result;
	}
	else if (allNodes_[endIndexZ][endIndexX].IsObstacle())
	{
		result.push_back(_endPosition);
		return result;
	}

	// ���ٷ� �� �Լ��� �ۼ�. F���� ���� ���ϰ�, F���� ������ H���� ���Ѵ�.
	// �ڽ�Ʈ�� ���� ���� �༮�� ������ �ڷ� ���� �����̴�. pop_back �� �ϱ� ����.
	auto cmp = [&](GHNode* _left, GHNode* _right)
	{
		float leftFCost = _left->GetFCost();
		float rightFCost = _right->GetFCost();

		if (leftFCost > rightFCost)
		{
			return true;
		}
		if (leftFCost == rightFCost)
		{
			return _left->GetHCost() > _right->GetHCost();
		}

		return false;
	};

	std::vector<GHNode*> open;
	std::vector<GHNode*> close;
	GHNode* endNode = &allNodes_[endIndexZ][endIndexX];

	open.push_back(&allNodes_[startIndexZ][startIndexX]);

	while (true)
	{
		std::sort(open.begin(), open.end(), cmp);
		GHNode* current = open.back();
		open.pop_back();
		close.push_back(current);

		// ���� ��尡 ������ �����
		if (current == endNode)
		{
			return current->GetPath();
		}

		// ���ϰ��� false �� ��� ��ֹ� �������� ����.
		// ���� �Լ��� �̿� üũ
		auto checkNeighbour = [&](int _xIndex, int _zIndex)->bool
		{
			if (_zIndex < 0 || _zIndex >= gridZCount_)
			{
				return true;
			}

			if (_xIndex < 0 || _xIndex >= gridXCount_)
			{
				return true;
			}

			GHNode* neighbour = &allNodes_[_zIndex][_xIndex];

			// �̿� ��尡 ��ֹ� �̶��
			if (neighbour->IsObstacle())
			{
				return false;
			}

			// �̿���尡 CLOSED ����Ʈ�� ���ִٸ�
			if (close.end() != std::find(close.begin(), close.end(), neighbour))
			{
				return true;
			}

			// // open list �� ��尡 ������
			if (open.end() == std::find(open.begin(), open.end(), neighbour))
			{
				neighbour->EvaluateCost(*current, *endNode);
				neighbour->SetParent(current);
				open.push_back(neighbour);
			}
			else if (current->GetEvaluatedGCostToOhterNode(*neighbour) < neighbour->GetGCost()) // ���� ��忡�� �̿����� ���� ��ΰ� �̿������ ���� ��κ��� ª�ٸ�(�θ� ������� ��, gCost�� �۴ٸ�)
			{
				neighbour->EvaluateCost(*current, *endNode);
				neighbour->SetParent(current);
			}

			return true;
		};

		int currentXIndex = current->GetXIndex();
		int currentZIndex = current->GetZIndex();

		// �����¿���� üũ�Ѵ�.
		bool bPassableUp = checkNeighbour(currentXIndex, currentZIndex + 1);
		bool bPassableDown = checkNeighbour(currentXIndex, currentZIndex - 1);
		bool bPassableLeft = checkNeighbour(currentXIndex - 1, currentZIndex);
		bool bPassableRight = checkNeighbour(currentXIndex + 1, currentZIndex);

		// �»�, ���, ����, ���� ������ üũ�Ѵ�.
		// �����¿찡 ���������� ����Ұ��� ����.
		if (bPassableUp)
		{
			if (bPassableLeft)
			{
				checkNeighbour(currentXIndex - 1, currentZIndex + 1);
			}

			if (bPassableRight)
			{
				checkNeighbour(currentXIndex + 1, currentZIndex + 1);
			}
		}

		if (bPassableDown)
		{
			if (bPassableLeft)
			{
				checkNeighbour(currentXIndex - 1, currentZIndex - 1);
			}

			if (bPassableRight)
			{
				checkNeighbour(currentXIndex + 1, currentZIndex - 1);
			}
		}


		// ��ΰ� ������ �극��ũ (��, ������ �� ����.)
		if (open.empty())
		{
			break;
		}
	}

	return result;
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

void GHMap::makeAStarNode(float _intervalX, float _intervalZ)
{
	intervalX_ = _intervalX;
	intervalZ_ = _intervalZ;
	gridStartX_ = FLT_MAX;
	gridStartZ_ = FLT_MAX;
	float endX = -FLT_MAX;
	float endZ = -FLT_MAX;

	// �׺���̼� �޽ø� ���鼭 �ּҰ�, �ִ밪 X, Z �� ���Ѵ�.
	for (GHNavMesh& n : navMeshes_)
	{
		for (float4& f : n.Vertices)
		{
			if (f.x < gridStartX_)
			{
				gridStartX_ = f.x;
			}

			if (f.z < gridStartZ_)
			{
				gridStartZ_ = f.z;
			}

			if (f.x > endX)
			{
				endX = f.x;
			}

			if (f.z > endZ)
			{
				endZ = f.z;
			}
		}
	}

	gridStartX_ *= navMeshRenderer_->GetTransform()->GetLocalScaling().x;
	gridStartZ_ *= navMeshRenderer_->GetTransform()->GetLocalScaling().z;
	endX *= navMeshRenderer_->GetTransform()->GetLocalScaling().x;
	endZ *= navMeshRenderer_->GetTransform()->GetLocalScaling().z;

	float XLength = endX - gridStartX_;
	float ZLength = endZ - gridStartZ_;

	// ���̿� ���͹��� ���� ��ŭ ��� ������ ������ �̴ϴ�.
	int nodeCountX = static_cast<int>(XLength / intervalX_) + 2;
	int nodeCountZ = static_cast<int>(ZLength / intervalZ_) + 2;

	// ��带 ���� ��ŭ�� �迭 ������ Ȯ���մϴ�.
	allNodes_.reserve(nodeCountZ);

	for (std::vector<GHNode>& v : allNodes_)
	{
		v.reserve(nodeCountX);
	}

	// ���ο� ��带 ����� �����մϴ�.
	int index = 0;
	for (int z = 0; z < nodeCountZ; z++)
	{
		allNodes_.push_back(std::vector<GHNode>());
		for (int x = 0; x < nodeCountX; x++)
		{
			float posX = gridStartX_ + x * intervalX_;
			float posZ = gridStartZ_ + z * intervalZ_;

			GHNode newNode(index++, z, x, float4(posX, 0.0f, posZ));

			allNodes_[z].push_back(newNode);
		}
	}

	tileVertexBuffer_ = new GameEngineVertexBuffer();
	tileIndexBuffer_ = new GameEngineIndexBuffer();

	tileVertices_;
	tileIndices_;
	size_t allVertexCount = allNodes_.size() * allNodes_[0].size();
	tileVertices_.reserve(allVertexCount);
	tileIndices_.reserve(allVertexCount * 7);

	int counter = 0;
	gridZCount_ = static_cast<int>(allNodes_.size());
	gridXCount_ = static_cast<int>(allNodes_[0].size());

	for (auto n : allNodes_)
	{
		for (auto m : n)
		{
			GameEngineVertex v;
			v.POSITION = m.GetPosition();
			v.COLOR = float4::GREEN;
			tileVertices_.push_back(v);
		}
	}

	for (size_t z = 0; z < gridZCount_ - 1; z++)
	{
		for (size_t x = 0; x < gridXCount_ - 1; x++)
		{
			size_t currentVertexIndex = z * gridXCount_ + x;
			tileIndices_.push_back(static_cast<UINT>(currentVertexIndex));
			tileIndices_.push_back(static_cast<UINT>(currentVertexIndex + 1));
			tileIndices_.push_back(static_cast<UINT>(currentVertexIndex + gridXCount_));

			tileIndices_.push_back(static_cast<UINT>(currentVertexIndex + 1));
			tileIndices_.push_back(static_cast<UINT>(currentVertexIndex + gridXCount_ + 1));
			tileIndices_.push_back(static_cast<UINT>(currentVertexIndex + gridXCount_));
		}
	}


	tileVertexBuffer_->Create(tileVertices_, D3D11_USAGE::D3D11_USAGE_DYNAMIC);
	tileIndexBuffer_->Create(tileIndices_, D3D11_USAGE::D3D11_USAGE_DEFAULT);

	GameEngineRenderer* Renderer = CreateTransformComponent<GameEngineRenderer>(GetTransform());
	Renderer->SetRenderingPipeLine("DeferredNavTile");
	Renderer->SetMesh(tileVertexBuffer_, tileIndexBuffer_);
	Renderer->GetGameEngineRenderingPipeLine()->SetRasterizer("EngineBaseRasterizerWireFrame");
	//Renderer->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
}

void GHMap::checkASterNodeObstacle()
{
	GameEngineTime::GetInst().TimeCheck();


	for (std::vector<GHNode>& v : allNodes_)
	{
		for (GHNode& n : v)
		{
			if (nullptr == GetCurrentNavMesh(n.GetPosition()))
			{
				n.SetObstacle(true);
				tileVertices_[n.GetIndex()].COLOR = float4::RED;
			}
		}
	}

	//int vertexSize = tileVertices_.size();

	//for (int i = 0; i < vertexSize; i++)
	//{
	//	if (nullptr == GetCurrentNavMesh(tileVertices_[i].POSITION))
	//	{
	//		tileVertices_[i].COLOR = float4::RED;
	//	}
	//}


	GameEngineTime::GetInst().TimeCheck();
	GameEngineTime::GetInst().GetDeltaTime();
}

void GHMap::updateAStarNodeVertexInfo()
{
	ID3D11DeviceContext* dc = GameEngineDevice::GetContext();

	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	dc->Map(tileVertexBuffer_->Buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	memcpy(subResource.pData, &tileVertices_[0], subResource.RowPitch);
	dc->Unmap(tileVertexBuffer_->Buffer_, 0);
}
