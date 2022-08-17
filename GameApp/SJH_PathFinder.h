#pragma once
#include "SJH_Funnel.h"

// A* ��� ����
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
		// �θ��尡 ���°�� : ���۳���϶�
		if (nullptr == Parent_)
		{
			PathLen_ = 0.0f;
		}
		// �θ��尡 �����ϴ°�� : Ž������϶�
		else
		{
			// �����߽ɱ��� �Ÿ� ����
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

// �뵵 : A* �� �ִܰ�θ� ���� �� Funnel Algorithm�� ���� ������θ� ��ȯ
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

private: // ������
	int PoolCount_;										// �ִ� ������Ƚ��(����)
	std::vector<SJH_AStarNode> NodePool_;				// ���������

private: // ���������
	std::multimap<float, SJH_AStarNode*> OpenList_;		// (�Ÿ�, �������) ���
	std::set<int> OpenKeys_;							// ������� Key

private: // ���������
	std::list<SJH_AStarNode*> CloseList_;				// ���������
	std::set<int> CloseKeys_;							// ������� Key

private:
	SJH_Funnel* Funnel_;								// �򶧱�˰���ü
};

