#include "Precompile.h"
#include "SJH_NaviCell.h"

void SJH_NaviCell::CreateNavigationCellInfo(int _MeshIndex, int _FaceIndex, std::vector<float4> _VertexList)
{
	// 기본정보 저장
	MeshIndex_ = _MeshIndex;
	FaceIndex_ = _FaceIndex;
	VertexList_ = _VertexList;

	// 수신받은 3개의 정점을 연결하는 선분의 중점을 계산
	CreateSideLineInfo();

	// 수신받은 3개의 정점으로 무게중심 계산
	CenterOfGravityCalculation();

	// 각 중점과 무게중심을 연결하는 방향벡터 생성

}

void SJH_NaviCell::SearchAdjacentTriangles()
{
	// 해당 면과 인접한 모든 삼각형을 탐색하여 목록작성
	// 단, 인접한 면이 존재하지않다면 목록을 저장하지않는다.




}

void SJH_NaviCell::CreateSideLineInfo()
{
	// 각 정점을 연결하는 선분들의 각각의 중점을 계산
	int VertexCount = static_cast<int>(VertexList_.size());
	for (int Vertex = 0; Vertex < VertexCount; ++Vertex)
	{
		SideLine NewSideLine = {};

		// 해당 사이드라인의 시작점-끝점 선분
		NewSideLine.StartVertex_ = float4();
		NewSideLine.EndVertex_ = float4();

		// 선분의 중점을 계산하여 저장



		NewSideLine.MidPoint_ = float4();

		// 총 3개의 사이드라인 생성
		SideLines_.push_back(NewSideLine);
	}
}

void SJH_NaviCell::CenterOfGravityCalculation()
{
	// 3개의 정점과 각 정점을 연결하는 선분들의 각각의 중점을 이용하여 무게중심을 계산


}

SJH_NaviCell::SJH_NaviCell()
	: MeshIndex_(-1)
	, FaceIndex_(-1)
	, CenterOfGravity_(float4::ZERO)
{
}

SJH_NaviCell::~SJH_NaviCell()
{
}
