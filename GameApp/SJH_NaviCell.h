#pragma once
#include <GameEngine/EngineVertex.h>

// �з� : �ﰢ��(��)
// �뵵 : ��ã��˰��� ���̽�
// ���� : �޽��� �̷�� �ﰢ������ �� �Ѱ�
class SJH_NaviCell
{
public:
	inline int GetCellInfomationIndex() const
	{
		return Index_;
	}

	inline float4 GetCenterToGravity() const
	{
		return CenterOfGravity_;
	}

	inline std::vector<SJH_NaviCell*> GetAdjacentTriangles() const
	{
		return AdjacentTriangles_;
	}

public:
	void CreateNavigationCellInfo(int _Index, int _MeshIndex, int _FaceNumber, std::vector<GameEngineVertex> _VertexList, std::vector<UINT> _IndexList);
	void SearchAdjacentTriangles(SJH_NaviCell* _CompareNaviCell, bool _2Vertex = true);

public:
	SJH_NaviCell* CompareVertices(const float4& _V0, const float4& _V1, const float4& _V2);
	bool CheckPointisIncludedIntheTriangle(const float4& _Position);

public:
	std::vector<GameEngineVertex> SearchShareVertex(SJH_NaviCell* _ShareCell);

protected:

private: // �� ������ ����
	void CenterOfGravityCalculation();

private: // ���� ���� �����ϴ� ���� Ž��(���ſ�)
	void StandingOntheCellCheck();

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
	int Index_;											// ���� ��Ͽ� �ش��ϴ� Index(Ž����)
	int MeshIndex_;										// �ش� ���� �����ϴ� MeshIndex(Ž����)
	int FaceIndex_;										// �ش� ���� �ε���(Ž����)
	std::vector<GameEngineVertex> VertexList_;			// �ش� ���� �����ϴ� �������(3��)
	std::vector<UINT> IndexList_;						// �ش� ���� �����ϴ� �ε������

private: // �����߽� �� ������ ����
	float4 CenterOfGravity_;							// �ش� ��(�ﰢ��)�� �����߽�
	std::vector<SJH_NaviCell*> AdjacentTriangles_;		// �ش� ��(�ﰢ��)�� ������ �ﰢ���� �ε������
};

