#pragma once

// �з� : �ﰢ��
// �뵵 : 
// ���� : �޽��� �̷�� �ﰢ������ ������ �Ѱ�
class SJH_NaviCell
{
public:
	void CreateNavigationCellInfo(int _MeshIndex, int _FaceNumber, std::vector<float4> _VertexList);
	void SearchAdjacentTriangles();

protected:

private:
	void MidPointCalculation();
	void CenterOfGravityCalculation();

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
	int MeshIndex_;										// �ش� ���� �����ϴ� MeshIndex(Ž����)
	int FaceIndex_;										// �ش� ���� �ε���(Ž����)
	std::vector<float4> VertexList_;					// �ش� ���� �����ϴ� �������(3��)

private:
	float4 CenterOfGravity_;							// �ش� ��(�ﰢ��)�� �����߽�
	std::vector<int> AdjacentTriangles_;				// �ش� ��(�ﰢ��)�� ������ �ﰢ���� �ε��� ���
};

