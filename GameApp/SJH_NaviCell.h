#pragma once

struct SideLine
{
	float4 StartVertex_;		// 선분의 시작점
	float4 EndVertex_;			// 선분의 끝점
	float4 MidPoint_;			// 선분의 중점
};

// 분류 : 삼각형
// 용도 : 
// 설명 : 메쉬를 이루는 삼각형들을 집합중 한개
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

private: // 수신받는 정보
	int MeshIndex_;										// 해당 면을 포함하는 MeshIndex(탐색용)
	int FaceIndex_;										// 해당 면의 인덱스(탐색용)
	std::vector<float4> VertexList_;					// 해당 면을 구성하는 정점목록(3개)

private: // 해당 면을 감싸는 선분목록
	std::vector<SideLine> SideLines_;					// 3개의 정점을 각각 연결하는 선분의 정보

private: // 무게중심 및 인접한 면목록
	float4 CenterOfGravity_;							// 해당 면(삼각형)의 무게중심
	std::vector<int> AdjacentTriangles_;				// 해당 면(삼각형)과 인접한 삼각형의 인덱스 목록
};

