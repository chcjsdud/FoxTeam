#pragma once

#include <GameEngineBase/GameEngineMath.h>

class GHNode
{
public:
	// ����� ��ġ�� ������ ���� ���̱� ������ ������ �� �ʱ�ȭ�Ѵ�.
	GHNode(const float4& _position);
	~GHNode();
	GHNode(const GHNode& _other);
	GHNode(GHNode&& _other) noexcept;
	GHNode& operator=(const GHNode& _other) = delete;
	GHNode& operator=(const GHNode&& _other) = delete;

public:
	void Open() { bOpen_ = true; }
	void Close() { bOpen_ = false; }

	bool IsOpen() { return bOpen_; }
	bool IsClose() { return !bOpen_; }

	float4 GetPosition() { return position_; }

private:
	float4 position_;
	bool bOpen_;
};

