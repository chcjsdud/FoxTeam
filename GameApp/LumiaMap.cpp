#include "PreCompile.h"
#include "LumiaMap.h"

#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/EngineVertex.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineCore.h>

#include "NavMesh.h"
#include "PlayerInfoManager.h"


LumiaMap::LumiaMap()
	: navMeshRenderer_(nullptr)
	, downTownRenderer_(nullptr)
	, tileVertexBuffer_(nullptr)
	, tileIndexBuffer_(nullptr)
	, intervalX_(0.0f)
	, intervalZ_(0.0f)
	, gridStartX_(0.0f)
	, gridStartZ_(0.0f)
	, gridZCount_(0)
	, navMesh_(nullptr)
	, gridXCount_(0)
{

}

LumiaMap::~LumiaMap()
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

void LumiaMap::Start()
{
	navMeshRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	//navMeshRenderer_->SetFBXMesh("DowntownNavMesh.fbx", "TextureDeferredLight");
	navMeshRenderer_->SetFBXMesh("Bg_NaviMesh.fbx", "TextureDeferredLight");

	for (UINT i = 0; i < navMeshRenderer_->GetRenderSetCount(); i++)
	{
		navMeshRenderer_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Red.png");
		navMeshRenderer_->GetRenderSet(i).PipeLine_->SetRasterizer("EngineBaseRasterizerWireFrame");
		//navMeshRenderer_->GetRenderSet(i).PipeLine_->SetRasterizer("EngineBaseRasterizerNone");
	}

	navMeshRenderer_->GetTransform()->SetLocalScaling(100.0f);
	navMeshRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 0.0f });
	navMeshRenderer_->Off();

	navMesh_ = CreateComponent<NavMesh>();

	// 모든 메쉬를 네비메쉬로
	navMesh_->CreateNaviMesh(navMeshRenderer_);

	makeAStarNode(200.f, 200.f);
	checkASterNodeObstacle();
	updateAStarNodeVertexInfo();

	GameEngineDirectory tempDir;

	tempDir.MoveParent("FoxTeam");
	tempDir / "Resources" / "FBX" / "UserMesh" / "Map";

	std::vector<GameEngineFile> vecFile = tempDir.GetAllFile(".UserMesh");

	mapRenderers.resize(vecFile.size());

	for (size_t i = 0; i < mapRenderers.size(); i++)
	{
		mapRenderers[i] = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
		mapRenderers[i]->SetFBXMesh(vecFile[i].GetFileName(), "TextureDeferredLight");
		mapRenderers[i]->GetTransform()->SetLocalScaling(100.0f);
	}

	tempDir.MoveParent("UserMesh");
	tempDir.MoveChild("ItemBox");

	vecFile = tempDir.GetAllFile(".UserMesh");

	for (size_t i = 0; i < vecFile.size(); i++)
	{
		GameEngineFBXRenderer* FBX = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
		FBX->SetFBXMesh(vecFile[i].GetFileName(), "TextureDeferredLight");
		FBX->GetTransform()->SetLocalScaling(100.0f);

		mapRenderers.push_back(FBX);
	}

	int a = 0;
}

void LumiaMap::Update(float _deltaTime)
{
	if (nullptr != PlayerInfoManager::GetInstance()->GetMainCharacter())
	{
		float4 PlayerPos = PlayerInfoManager::GetInstance()->
			GetMainCharacter()->GetTransform()->GetWorldPosition();

		std::list<float> tmp;

		for (size_t i = 0; i < mapRenderers.size(); i++)
		{
			std::vector<RenderSet>& RenderSets = mapRenderers[i]->GetAllRenderSet();

			for (size_t j = 0; j < RenderSets.size(); j++)
			{
				//float Length = float4::Calc_Len3D(PlayerPos, RenderSets[j].LocalPos * mapRenderers[i]->GetTransform()->GetTransformData().WorldWorld_);
				float4 RenderSetPos = RenderSets[j].LocalPos * mapRenderers[i]->GetTransform()->GetTransformData().WorldWorld_;

				float Length = float4::Calc_Len2D(PlayerPos.x, PlayerPos.z, 
					RenderSetPos.x, RenderSetPos.z);

				tmp.push_back(RenderSets[j].Radius);
				
				if (RenderSets[j].Radius > 1.0f)
				{
					if (Length >= RENDER_RANGE * RenderSets[j].Radius)
					{
						RenderSets[j].isRender = false;
					}
					else
					{
						RenderSets[j].isRender = true;
					}
				}
				else
				{
					if (Length >= RENDER_RANGE)
					{
						RenderSets[j].isRender = false;
					}
					else
					{
						RenderSets[j].isRender = true;
					}
				}
			}
		}
		tmp;
		int a = 0;
	}
}

std::vector<float4> LumiaMap::FindPath(const float4& _startPosition, const float4& _endPosition)
{
	std::vector<float4> result;

	// 그리드의 스타트 포인트 인덱스와 엔드 포인트 인덱스를 구한다.
	int startIndexX = static_cast<int>((_startPosition.x - gridStartX_) / intervalX_ + 0.5f); // 반올림처리
	int startIndexZ = static_cast<int>((_startPosition.z - gridStartZ_) / intervalZ_ + 0.5f);

	int endIndexX = static_cast<int>((_endPosition.x - gridStartX_) / intervalX_ + 0.5f);
	int endIndexZ = static_cast<int>((_endPosition.z - gridStartZ_) / intervalZ_ + 0.5f);

	// 도착점이 그리드를 벗어나는 경우, 혹은 도착점이 이동 불가능한 곳일 경우 리턴 처리
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

	// 람다로 비교 함수를 작성. F값을 먼저 비교하고, F값이 같으면 H값을 비교한다.
	// 코스트가 가장 낮은 녀석은 벡터의 뒤로 보낼 생각이다. pop_back 을 하기 위해.
	auto cmp = [&](AStarNode* _left, AStarNode* _right)
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

	std::vector<AStarNode*> open;
	std::vector<AStarNode*> close;
	AStarNode* endNode = &allNodes_[endIndexZ][endIndexX];

	open.push_back(&allNodes_[startIndexZ][startIndexX]);

	while (true)
	{
		std::sort(open.begin(), open.end(), cmp);
		AStarNode* current = open.back();
		open.pop_back();
		close.push_back(current);

		// 현재 노드가 목적지 노드라면
		if (current == endNode)
		{
			return current->GetPath();
		}

		// 리턴값이 false 일 경우 장애물 판정으로 본다.
		// 람다 함수로 이웃 체크
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

			AStarNode* neighbour = &allNodes_[_zIndex][_xIndex];

			// 이웃 노드가 장애물 이라면
			if (neighbour->IsObstacle())
			{
				return false;
			}

			// 이웃노드가 CLOSED 리스트에 들어가있다면
			if (close.end() != std::find(close.begin(), close.end(), neighbour))
			{
				return true;
			}

			// // open list 에 노드가 없으면
			if (open.end() == std::find(open.begin(), open.end(), neighbour))
			{
				neighbour->EvaluateCost(*current, *endNode);
				neighbour->SetParent(current);
				open.push_back(neighbour);
			}
			else if (current->GetEvaluatedGCostToOhterNode(*neighbour) < neighbour->GetGCost()) // 현재 노드에서 이웃노드로 가는 경로가 이웃노드의 현재 경로보다 짧다면(부모를 고려했을 때, gCost가 작다면)
			{
				neighbour->EvaluateCost(*current, *endNode);
				neighbour->SetParent(current);
			}

			return true;
		};

		int currentXIndex = current->GetXIndex();
		int currentZIndex = current->GetZIndex();

		// 상하좌우부터 체크한다.
		bool bPassableUp = checkNeighbour(currentXIndex, currentZIndex + 1);
		bool bPassableDown = checkNeighbour(currentXIndex, currentZIndex - 1);
		bool bPassableLeft = checkNeighbour(currentXIndex - 1, currentZIndex);
		bool bPassableRight = checkNeighbour(currentXIndex + 1, currentZIndex);

		// 좌상, 우상, 좌하, 우하 순으로 체크한다.
		// 상하좌우가 막혀있으면 통행불가로 본다.
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


		// 경로가 없으면 브레이크 (즉, 도달할 수 없다.)
		if (open.empty())
		{
			break;
		}
	}

	return result;
}

void LumiaMap::makeAStarNode(float _intervalX, float _intervalZ)
{
	intervalX_ = _intervalX;
	intervalZ_ = _intervalZ;
	gridStartX_ = FLT_MAX;
	gridStartZ_ = FLT_MAX;
	float endX = -FLT_MAX;
	float endZ = -FLT_MAX;

	// 네비게이션 메시를 돌면서 최소값, 최대값 X, Z 를 구한다.
	for (NavFace n : navMesh_->GetAllNavi())
	{
		for (float4& f : n.GetInfo().Vertex)
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

	// 길이에 인터벌을 나눈 만큼 노드 갯수를 생성할 겁니다.
	int nodeCountX = static_cast<int>(XLength / intervalX_) + 2;
	int nodeCountZ = static_cast<int>(ZLength / intervalZ_) + 2;

	// 노드를 넣을 만큼의 배열 공간을 확보합니다.
	allNodes_.reserve(nodeCountZ);

	for (std::vector<AStarNode>& v : allNodes_)
	{
		v.reserve(nodeCountX);
	}

	// 새로운 노드를 만들어 삽입합니다.
	int index = 0;
	for (int z = 0; z < nodeCountZ; z++)
	{
		allNodes_.push_back(std::vector<AStarNode>());
		for (int x = 0; x < nodeCountX; x++)
		{
			float posX = gridStartX_ + x * intervalX_;
			float posZ = gridStartZ_ + z * intervalZ_;

			AStarNode newNode(index++, z, x, float4(posX, 0.0f, posZ));

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

void LumiaMap::checkASterNodeObstacle()
{
	GameEngineTime timer;
	timer.TimeCheckReset();
	timer.TimeCheck();

	for (std::vector<AStarNode>& v : allNodes_)
	{

		GameEngineCore::ThreadQueue.JobPost(
			[&]()
			{
				for (AStarNode& n : v)
				{
					float height = 0.0f;
					NavFace* face = navMesh_->GetNavFaceFromPositionXZ(n.GetPosition(), float4::DOWN, height);
					if (nullptr == face)
					{
						n.SetObstacle(true);
						tileVertices_[n.GetIndex()].COLOR = float4::RED;
					}
					else
					{
						tileVertices_[n.GetIndex()].POSITION.y = height;
					}
				}
			});
	}

	Sleep(50);

	while (0 < GameEngineCore::ThreadQueue.GetWorkingCount())
	{
		Sleep(100);
	}

	timer.TimeCheck();
	float elapsedTime = timer.GetDeltaTime();
	GameEngineDebug::OutPutDebugString("checkAStarNodeObstacle time elapsed : " + std::to_string(elapsedTime) + "\n");
}

void LumiaMap::updateAStarNodeVertexInfo()
{
	ID3D11DeviceContext* dc = GameEngineDevice::GetContext();

	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	dc->Map(tileVertexBuffer_->Buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	memcpy(subResource.pData, &tileVertices_[0], subResource.RowPitch);
	dc->Unmap(tileVertexBuffer_->Buffer_, 0);
}
