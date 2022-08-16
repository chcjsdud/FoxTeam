#pragma once
#include <GameEngine/EngineVertex.h>

struct SideLine
{
public:
	GameEngineVertex StartVertex_;		// 선분의 시작점
	GameEngineVertex EndVertex_;		// 선분의 끝점
	float4 MidPoint_;					// 선분의 중점
	float ArrivalCost_;					// 무게중심-중점까지의 거리

public:
	SideLine() : StartVertex_(), EndVertex_(), MidPoint_(float4::ZERO), ArrivalCost_(0.0f)
	{
	}
};

// 분류 : 삼각형(면)
// 용도 : 길찾기알고리즘 베이스
// 설명 : 메쉬를 이루는 삼각형집합 중 한개
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
	void SetPortalVertex(SJH_NaviCell* _NextCell); // A* 경로에 해당하는 삼각형의 Portal Vertex Setting

protected:

private: // 셀 상세정보 생성
	void CreateSideLineInfo();
	void CenterOfGravityCalculation();

private: // 현재 셀에 존재하는 액터 탐색(갱신용)
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

private: // 수신받는 정보
	int Index_;											// 현재 목록에 해당하는 Index(탐색용)
	int MeshIndex_;										// 해당 면을 포함하는 MeshIndex(탐색용)
	int FaceIndex_;										// 해당 면의 인덱스(탐색용)
	std::vector<GameEngineVertex> VertexList_;			// 해당 면을 구성하는 정점목록(3개)
	std::vector<UINT> IndexList_;						// 해당 면을 구성하는 인덱스목록

private: // 해당 면을 감싸는 선분목록
	std::vector<SideLine> SideLines_;					// 3개의 정점을 각각 연결하는 선분의 정보(선분의 2정점, 중점)

private: // 무게중심 및 인접한 면목록
	float4 CenterOfGravity_;							// 해당 면(삼각형)의 무게중심
	std::vector<SJH_NaviCell*> AdjacentTriangles_;		// 해당 면(삼각형)과 인접한 삼각형의 인덱스목록

private: // 경로로 지정된 삼각형일때 공유하는 정점목록
	std::vector<GameEngineVertex> PortalVertex_;		// 포탈 정점

private:
	GameEngineActor* OntheFaceActor_;					// 해당 면에 존재하는 액터(아무것도 없을때 nullptr)
};

