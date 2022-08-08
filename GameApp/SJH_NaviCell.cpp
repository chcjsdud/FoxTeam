#include "Precompile.h"
#include "SJH_NaviCell.h"

void SJH_NaviCell::CreateNavigationCellInfo(int _MeshIndex, int _FaceIndex, std::vector<float4> _VertexList)
{
	// 기본정보 저장
	MeshIndex_ = _MeshIndex;
	FaceIndex_ = _FaceIndex;
	VertexList_ = _VertexList;

	// 수신받은 3개의 정점을 연결하는 선분의 중점을 계산
	MidPointCalculation();

	// 수신받은 3개의 정점으로 무게중심 계산
	CenterOfGravityCalculation();
}

void SJH_NaviCell::SearchAdjacentTriangles()
{
	// 해당 면과 인접한 모든 삼각형을 탐색하여 목록작성
	// 단, 인접한 면이 존재하지않다면 목록을 저장하지않는다.




}

void SJH_NaviCell::MidPointCalculation()
{
	// 각 정점을 연결하는 선분들의 각각의 중점을 계산


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
