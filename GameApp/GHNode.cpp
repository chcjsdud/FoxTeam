#include "PreCompile.h"
#include "GHNode.h"

GHNode::GHNode(const float4& _position)
	: position_(_position)
	, bOpen_(true)
{

}

GHNode::~GHNode()
{

}

GHNode::GHNode(const GHNode& _other)
	: position_(_other.position_)
	, bOpen_(_other.bOpen_)
{
}

GHNode::GHNode(GHNode&& _other) noexcept
	: position_(_other.position_)
	, bOpen_(_other.bOpen_)
{
}
