#include "Precompile.h"
#include "SJH_PathFinder.h"

std::list<SJH_NaviCell*> SJH_PathFinder::AStarMovePath(SJH_NaviCell* _StartCell, SJH_NaviCell* _EndCell, int _Maximum)
{
	// 초기화
	PoolCount_ = 0;
	NodePool_.resize(_Maximum);
	OpenList_.clear();
	OpenKeys_.clear();
	CloseList_.clear();
	CloseKeys_.clear();

	// 시작노드 생성
	SJH_AStarNode* StartNode = CreateNode(_StartCell, _EndCell);
	OpenList_.insert(std::make_pair(StartNode->TotalLen_, StartNode));
	OpenKeys_.insert(StartNode->CellInfo_->GetCellInfomationIndex());

	// 열린노드 모두 소멸시까지 반복
	while (0 != OpenList_.size())
	{
		// 메모리 최대치 탐색이 일어났다면 경로를 찾지못함
		if (PoolCount_ >= NodePool_.size())
		{
			return std::list<SJH_NaviCell*>();
		}

		// 경로 탐색 시작
		std::vector<SJH_NaviCell*> AdjacentTri;
		AdjacentTri.clear();

		// 시작노드 설정 및 해당 노드에 인접한 삼각형정보 셋팅
		SJH_AStarNode* FirstNode = OpenList_.begin()->second;
		AdjacentTri = FirstNode->CellInfo_->GetAdjacentTriangles();

		// 열린노드 목록에서 해당 노드를 삭제
		OpenList_.erase(OpenList_.begin());
		OpenKeys_.erase(FirstNode->CellInfo_->GetCellInfomationIndex());

		// 닫힌노드 목록에 해당 노드 추가
		CloseList_.push_back(FirstNode);
		CloseKeys_.insert(FirstNode->CellInfo_->GetCellInfomationIndex());

		// 해당 노드에 인접한 노드갯수만큼 체크
		int AdjacentTriCount = static_cast<int>(AdjacentTri.size());
		for (int Tri = 0; Tri < AdjacentTriCount; ++Tri)
		{
			// 
			SJH_NaviCell* CheckNode = AdjacentTri[Tri];

			// 열린노드목록에 해당 노드가 존재하면 Continue
			if (OpenKeys_.end() != OpenKeys_.find(CheckNode->GetCellInfomationIndex()))
			{
				continue;
			}

			// 닫힌노드목록에 해당 노드가 존재하면 Continue
			if (CloseKeys_.end() != CloseKeys_.find(CheckNode->GetCellInfomationIndex()))
			{
				continue;
			}

			// 위조건이 모두 아니라면 이동가능 노드로 판단
			SJH_AStarNode* NewNode = CreateNode(CheckNode, _EndCell, FirstNode);
			if (nullptr != NewNode)
			{
				// 노드 생성이 정상적이라면 
				NewNode->Parent_ = FirstNode;

				// 목표 노드에 도착했다면 이동경로 반환
				if (_EndCell == CheckNode)
				{
					return CalReturn(NewNode);
				}

				// 아니라면 열린목록에 추가
				OpenList_.insert(std::make_pair(NewNode->TotalLen_, NewNode));
				OpenKeys_.insert(FirstNode->CellInfo_->GetCellInfomationIndex());
			}
		}
	}

	return std::list<SJH_NaviCell*>();
}

SJH_AStarNode* SJH_PathFinder::CreateNode(SJH_NaviCell* _StartCell, SJH_NaviCell* _EndCell, SJH_AStarNode* _ParentNode)
{
	// 최대 노드생성횟수 넘어가면 노드 생성불가
	if (PoolCount_ >= NodePool_.size())
	{
		return nullptr;
	}

	// 노드 생성
	SJH_AStarNode* NewNode = &NodePool_[PoolCount_++];
	NewNode->Reset();
	NewNode->CellInfo_ = _StartCell;
	NewNode->CalLen(_EndCell);

	return NewNode;
}

std::list<SJH_NaviCell*> SJH_PathFinder::CalReturn(SJH_AStarNode* _EndNode)
{
	// 목표지점의 노드 ~ 시작지점 노드를 순회하며 경로 생성
	std::list<SJH_NaviCell*> ReturnList;

	SJH_AStarNode* FindNode = _EndNode;
	while (nullptr != FindNode->Parent_)
	{
		ReturnList.push_front(FindNode->CellInfo_);

		// 부모노드로 이동
		FindNode = FindNode->Parent_;
	}

	return ReturnList;
}

SJH_PathFinder::SJH_PathFinder()
	: PoolCount_(0)
{
}

SJH_PathFinder::~SJH_PathFinder()
{
}
