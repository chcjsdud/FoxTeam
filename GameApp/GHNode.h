#pragma once

#include <GameEngineBase/GameEngineMath.h>
#include <stack>

// �ڽ�Ʈ�� float �� �Ǿ� �ִµ� �����ٸ� ������ �ٲٴ� ���� ���
class GHNode
{
public:
	// ����� ��ġ�� ������ ���� ���̱� ������ ������ �� �ʱ�ȭ�Ѵ�.
	GHNode(int _index1D, int _zIndex, int _xIndex, const float4& _position);
	~GHNode();
	GHNode(const GHNode& _other);
	GHNode(GHNode&& _other) noexcept;
	GHNode& operator=(const GHNode& _other) = delete;
	GHNode& operator=(const GHNode&& _other) noexcept;

public:
	// ������κ��� _other ���� ������ G ���� ��ȯ�Ѵ�.
	float GetEvaluatedGCostToOhterNode(const GHNode& _other);

	// ��� ����� f���� ���Ѵ�.
	void EvaluateCost(const GHNode& _other, const GHNode& _endNode);

	bool IsObstacle() { return bObstacle_; }

	void SetObstacle(bool _bObstacle) { bObstacle_ = _bObstacle; }
	void SetZIndex(int _zIndex) { zIndex_ = _zIndex; }
	void SetXIndex(int _xIndex) { xIndex_ = _xIndex; }
	void SetParent(GHNode* _parent);

	float GetGCost() { return gCost_; }
	float GetHCost() { return hCost_; }
	float GetFCost() { return fCost_; }

	float4 GetPosition() { return position_; }
	int GetIndex() { return index_; }
	int GetZIndex() { return zIndex_; }
	int GetXIndex() { return xIndex_; }

	std::vector<float4> GetPath();

private:
	int index_;
	int zIndex_;
	int xIndex_;
	float4 position_;
	bool bObstacle_;
	
	float gCost_;
	float hCost_;
	float fCost_;

	GHNode* parent_;
};

