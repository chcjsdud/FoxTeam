#pragma once

// �з� : �ﰢ��
// �뵵 : 
// ���� : �޽��� �̷�� �ﰢ������ ������ �Ѱ�
class SJH_NaviCell
{
public:
protected:
private:

public:
	SJH_NaviCell();
	~SJH_NaviCell();

protected:
	SJH_NaviCell(const SJH_NaviCell& _other) = delete;
	SJH_NaviCell(SJH_NaviCell&& _other) noexcept = delete;

private:
	SJH_NaviCell& operator=(const SJH_NaviCell& _other) = delete;
	SJH_NaviCell& operator=(const SJH_NaviCell&& _other) = delete;

public:
protected:
private:
	std::vector<int> AdjacentTriangles_;				// �ش� ��(�ﰢ��)�� ������ �ﰢ���� �ε��� ���
	float4 CenterOfGravity_;							// �ش� ��(�ﰢ��)�� �����߽�
	std::vector<float4> VertexList_;					// �ش� ���� �����ϴ� �������(3��)

private:
};

