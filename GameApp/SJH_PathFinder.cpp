#include "Precompile.h"
#include "SJH_PathFinder.h"

std::list<float4> SJH_PathFinder::SearchMovePath(const float4& _StartPos, const float4& _EndPos, SJH_NaviCell* _StartCell, SJH_NaviCell* _EndCell, int _Maximum)
{
	std::list<float4> ReturnPath;

	// 이동시작셀과 이동목표셀이 같다면 
	if (_StartCell == _EndCell)
	{
		// 시작위치 -> 클릭지점 경로 생성후 리턴
		ReturnPath.push_back(_StartPos);
		ReturnPath.push_back(_EndPos);
		return ReturnPath;
	}

	// 1. A*를 이용하여 최단경로를 탐색한 후
	std::list<SJH_NaviCell*> AStartPath;
	AStartPath = AStarMovePath(_StartCell, _EndCell, _Maximum);
	if (false == AStartPath.empty())
	{
		// 2. Funnel Algorithm을 이용하여 가시적인 최적직선경로 반환
		if (nullptr == Funnel_)
		{
			// 객체 생성
			Funnel_ = new SJH_Funnel();
		}

		ReturnPath = Funnel_->PathOptimization(_StartPos, _EndPos, AStartPath);
		if (false == ReturnPath.empty())
		{
			// 최적경로 생성완료
			return ReturnPath;
		}
	}

	return std::list<float4>();
}

std::list<SJH_NaviCell*> SJH_PathFinder::AStarMovePath(SJH_NaviCell* _StartCell, SJH_NaviCell* _EndCell, int _Maximum)
{
	// 초기화
	PoolCount_ = 0;
	NodePool_.clear();
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
			// 인접한 셀 Get
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
				if (_EndCell == NewNode->CellInfo_)
				{
					return CalReturn(NewNode);
				}

				// 아니라면 열린목록에 추가
				OpenList_.insert(std::make_pair(NewNode->TotalLen_, NewNode));
				OpenKeys_.insert(NewNode->CellInfo_->GetCellInfomationIndex());
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

	NodePool_.clear();

	return ReturnList;
}

SJH_PathFinder::SJH_PathFinder()
	: PoolCount_(0)
	, Funnel_(nullptr)
{
}

SJH_PathFinder::~SJH_PathFinder()
{
	if (nullptr != Funnel_)
	{
		delete Funnel_;
		Funnel_ = nullptr;
	}
}
