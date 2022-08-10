#pragma once

struct SideLine
{
public:
	float4 StartVertex_;		// 선분의 시작점
	float4 EndVertex_;			// 선분의 끝점
	float4 MidPoint_;			// 선분의 중점
	float ArrivalCost_;			// 무게중심-중점까지의 거리

public:
	SideLine() : StartVertex_(float4::ZERO), EndVertex_(float4::ZERO), MidPoint_(float4::ZERO), ArrivalCost_(0.0f)
	{
	}
};

// 분류 : 삼각형(면)
// 용도 : 길찾기알고리즘 베이스
// 설명 : 메쉬를 이루는 삼각형집합 중 한개
class SJH_NaviCell
{
public:
	inline bool GetFaceInfomationFlag()
	{
		return FaceInfoCompleted_;
	}

public:
	void CreateNavigationCellInfo(int _MeshIndex, int _FaceNumber, std::vector<float4> _VertexList, std::vector<UINT> _IndexList);
	void SearchAdjacentTriangles(SJH_NaviCell* _CompareNaviCell, bool _3Direction = true);

public:
	SJH_NaviCell* IsPickedCellInfo(const float4& _PickedPos);

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

private: // 수신받는 정보
	int MeshIndex_;										// 해당 면을 포함하는 MeshIndex(탐색용)
	int FaceIndex_;										// 해당 면의 인덱스(탐색용)
	std::vector<float4> VertexList_;					// 해당 면을 구성하는 정점목록(3개)
	std::vector<UINT> IndexList_;						// 해당 면을 구성하는 인덱스목록

private: // 해당 면을 감싸는 선분목록
	std::vector<SideLine> SideLines_;					// 3개의 정점을 각각 연결하는 선분의 정보(선분의 2정점, 중점)

private: // 무게중심 및 인접한 면목록
	float4 CenterOfGravity_;							// 해당 면(삼각형)의 무게중심
	std::vector<SJH_NaviCell*> AdjacentTriangles_;		// 해당 면(삼각형)과 인접한 삼각형의 인덱스목록

private:
	bool FaceInfoCompleted_;							// 해당 면의 정보 수집완료 Flag
};

