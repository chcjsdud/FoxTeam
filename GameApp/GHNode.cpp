#include "PreCompile.h"
#include "GHNode.h"

GHNode::GHNode(int _index1D, int _zIndex, int _xIndex, const float4& _position)
	: position_(_position)
	, bObstacle_(false)
	, index_(_index1D)
	, zIndex_(_zIndex)
	, xIndex_(_xIndex)
	, parent_(nullptr)
	, gCost_(0.0f)
	, hCost_(0.0f)
	, fCost_(0.0f)
{

}

GHNode::~GHNode()
{

}

GHNode::GHNode(const GHNode& _other)
	: position_(_other.position_)
	, bObstacle_(_other.bObstacle_)
	, index_(_other.index_)
	, zIndex_(_other.zIndex_)
	, xIndex_(_other.xIndex_)
	, parent_(_other.parent_)
	, gCost_(_other.gCost_)
	, hCost_(_other.hCost_)
	, fCost_(_other.fCost_)
{
}

GHNode::GHNode(GHNode&& _other) noexcept
	: position_(_other.position_)
	, bObstacle_(_other.bObstacle_)
	, index_(_other.index_)
	, zIndex_(_other.zIndex_)
	, xIndex_(_other.xIndex_)
	, parent_(_other.parent_)
	, gCost_(_other.gCost_)
	, hCost_(_other.hCost_)
	, fCost_(_other.fCost_)
{
}

GHNode& GHNode::operator=(const GHNode&& _other) noexcept
{
	this->index_ = _other.index_;
	this->zIndex_ = _other.zIndex_;
	this->xIndex_ = _other.xIndex_;
	this->position_ = _other.position_;
	this->bObstacle_ = _other.bObstacle_;
	this->parent_ = _other.parent_;
	this->gCost_ = _other.gCost_;
	this->hCost_ = _other.hCost_;
	this->fCost_ = _other.fCost_;
	return *this;
}

void GHNode::SetParent(GHNode* _parent)
{
	if (this == _parent)
	{
		return;
	}

	parent_ = _parent;
}

float GHNode::GetEvaluatedGCostToOhterNode(const GHNode& _other)
{
	return gCost_ + (_other.position_ - position_).Len3D();
}

void GHNode::EvaluateCost(const GHNode& _other, const GHNode& _endNode)
{
	gCost_ = (position_ - _other.position_).Len3D() + (parent_ != nullptr ? parent_->GetGCost() : 0);
	hCost_ = (_endNode.position_ - position_).Len3D();
	fCost_ = gCost_ + hCost_;
}

std::vector<float4> GHNode::GetPath()
{
	std::vector<float4> ret;
	ret.push_back(position_);
	GHNode* oldParent = this;
	GHNode* parent = parent_;

	oldParent->parent_ = nullptr;

	while (nullptr != parent)
	{
		ret.push_back(parent->position_);
		oldParent = parent;
		parent = parent->parent_;
		oldParent->parent_ = nullptr;
	}

	// 자연스러운 움직임을 위한 첫 포인트 제거
	if (!ret.empty())
	{
		ret.pop_back();
	}

	return ret;
}
