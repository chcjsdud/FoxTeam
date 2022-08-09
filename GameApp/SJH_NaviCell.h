#pragma once

struct SideLine
{
	float4 StartVertex_;		// ������ ������
	float4 EndVertex_;			// ������ ����
	float4 MidPoint_;			// ������ ����
};

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
	void CreateSideLineInfo();
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

private: // ���Ź޴� ����
	int MeshIndex_;										// �ش� ���� �����ϴ� MeshIndex(Ž����)
	int FaceIndex_;										// �ش� ���� �ε���(Ž����)
	std::vector<float4> VertexList_;					// �ش� ���� �����ϴ� �������(3��)

private: // �ش� ���� ���δ� ���и��
	std::vector<SideLine> SideLines_;					// 3���� ������ ���� �����ϴ� ������ ����

private: // �����߽� �� ������ ����
	float4 CenterOfGravity_;							// �ش� ��(�ﰢ��)�� �����߽�
	std::vector<int> AdjacentTriangles_;				// �ش� ��(�ﰢ��)�� ������ �ﰢ���� �ε��� ���
};

