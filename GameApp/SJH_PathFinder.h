#pragma once
#include "SJH_Funnel.h"

// A* 노드 정보
class SJH_NaviCell;
class SJH_AStarNode
{
public:
	SJH_NaviCell* CellInfo_;
	SJH_AStarNode* Parent_;
	float PathLen_;
	float EndLen_;
	float TotalLen_;

public:
	void CalLen(SJH_NaviCell* _CellInfo)
	{
		// 부모노드가 없는경우 : 시작노드일때
		if (nullptr == Parent_)
		{
			PathLen_ = 0.0f;
		}
		// 부모노드가 존재하는경우 : 탐색노드일때
		else
		{
			// 무게중심기준 거리 측정
			PathLen_ = Parent_->PathLen_ + (Parent_->CellInfo_->GetCenterToGravity() - CellInfo_->GetCenterToGravity()).Len3D();
		}

		EndLen_ = (_CellInfo->GetCenterToGravity() - CellInfo_->GetCenterToGravity()).Len3D();
		TotalLen_ = PathLen_ + EndLen_;
	}

	void Reset()
	{
		memset(this, 0x00, sizeof(*this));
	}
};

// 용도 : A* 로 최단경로를 구한 후 Funnel Algorithm을 통해 최적경로를 반환
class SJH_Funnel;
class SJH_PathFinder
{
public:
	std::list<float4> SearchMovePath(const float4& _StartPos, const float4& _EndPos, SJH_NaviCell* _StartCell, SJH_NaviCell* _EndCell, int _Maximum = 10000);

protected:

private: // A*
	std::list<SJH_NaviCell*> AStarMovePath(SJH_NaviCell* _StartCell, SJH_NaviCell* _EndCell, int _Maximum = 1000);
	SJH_AStarNode* CreateNode(SJH_NaviCell* _StartCell, SJH_NaviCell* _EndCell, SJH_AStarNode* _ParentNode = nullptr);
	std::list<SJH_NaviCell*> CalReturn(SJH_AStarNode* _EndNode);

public:
	SJH_PathFinder();
	~SJH_PathFinder();

protected:		// delete constructer
	SJH_PathFinder(const SJH_PathFinder& _other) = delete;
	SJH_PathFinder(SJH_PathFinder&& _other) noexcept = delete;

private:		//delete operator
	SJH_PathFinder& operator=(const SJH_PathFinder& _other) = delete;
	SJH_PathFinder& operator=(const SJH_PathFinder&& _other) = delete;

public:

protected:

private: // 노드생성
	int PoolCount_;										// 최대 노드생성횟수(고정)
	std::vector<SJH_AStarNode> NodePool_;				// 생성노드목록

private: // 열린노드목록
	std::multimap<float, SJH_AStarNode*> OpenList_;		// (거리, 열린노드) 목록
	std::set<int> OpenKeys_;							// 열린노드 Key

private: // 닫힌노드목록
	std::list<SJH_AStarNode*> CloseList_;				// 닫힌노드목록
	std::set<int> CloseKeys_;							// 닫힌노드 Key

private:
	SJH_Funnel* Funnel_;								// 깔때기알고리즘객체
};

