#pragma once

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
	void CalLen(SJH_NaviCell* _EndNaviCell)
	{
		//if (nullptr == Parent_)
		//{
		//	PathLen_ = 0.0f;
		//}
		//else
		//{
		//	float4 ParentIndex = float4(static_cast<float>(Parent_->Index_.X_), static_cast<float>(Parent_->Index_.Y_));
		//	float4 ThisIndex = float4(static_cast<float>(Index_.X_), static_cast<float>(Index_.Y_));
		//	PathLen_ = Parent_->PathLen_ + (ParentIndex - ThisIndex).Len2D();
		//}
		//
		//float4 EndIndex = float4(static_cast<float>(_EndIndex.X_), static_cast<float>(_EndIndex.Y_));
		//float4 ThisIndex = float4(static_cast<float>(Index_.X_), static_cast<float>(Index_.Y_));
		//EndLen_ = (EndIndex - ThisIndex).Len2D();
		//
		//TotalLen_ = PathLen_ + EndLen_;
	}

	void Reset()
	{
		memset(this, 0x00, sizeof(*this));
	}
};

// 용도 : A* 
class SJH_PathFinder
{
public:
protected:
private:

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

private:
	int PoolCount_;
	std::vector<SJH_AStarNode> NodePool_;

private:
	std::multimap<float, SJH_AStarNode*> OpenList_;
	std::set<__int64> OpenKeys_;

private:
	std::list<SJH_AStarNode*> CloseList_;
	std::set<__int64> CloseKeys_;
};

