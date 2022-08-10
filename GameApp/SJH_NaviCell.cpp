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
}

void SJH_NaviCell::SearchAdjacentTriangles()
{
	



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
	// 해당 면을 이루는 사이드라인의 정보를 모두 수집했으므로
	// 해당 면의 무게중심을 계산하고,
	CenterOfGravity_ = float4::ZERO;


	// 면의 무게중심에서 각 선분의 중점을 향하는 벡터의 길이를 알아낸다.
	int SideLineCount = static_cast<int>(SideLines_.size());
	for (int SideLine = 0; SideLine < SideLineCount; ++SideLine)
	{
		//SideLines_[SideLine].ArriveCost_;





	}
}

SJH_NaviCell::SJH_NaviCell()
	: MeshIndex_(-1)
	, FaceIndex_(-1)
	, CenterOfGravity_(float4::ZERO)
	, FaceInfoCompleted_(false)
{
}

SJH_NaviCell::~SJH_NaviCell()
{
}
