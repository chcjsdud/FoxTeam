#include "Precompile.h"
#include "SJH_NaviCell.h"

void SJH_NaviCell::CreateNavigationCellInfo(int _MeshIndex, int _FaceIndex, std::vector<float4> _VertexList, std::vector<UINT> _IndexList)
{
	// 기본정보 저장
	MeshIndex_ = _MeshIndex;
	FaceIndex_ = _FaceIndex;
	VertexList_ = _VertexList;
	IndexList_ = _IndexList;

	// 수신받은 3개의 정점을 연결하는 선분의 중점을 계산
	CreateSideLineInfo();

	// 수신받은 3개의 정점으로 무게중심 계산
	CenterOfGravityCalculation();
}

void SJH_NaviCell::SearchAdjacentTriangles(SJH_NaviCell* _CompareNaviCell, bool _3Direction)
{
	if (true == _3Direction)
	{
		// 조건: 2개의 정점을 공유하고 무게중심이 다른 면이라면 인접한 면으로 판단 -> 3방향의 인접한면을 가지고있게된다.
		int ThisVertexSize = static_cast<int>(VertexList_.size());

		int SharedVerticesCount = 0;
		for (int ThisVertex = 0; ThisVertex < ThisVertexSize; ++ThisVertex)
		{
			int CompareVertexSize = static_cast<int>(_CompareNaviCell->VertexList_.size());
			for (int CompareVertex = 0; CompareVertex < CompareVertexSize; ++CompareVertex)
			{
				if (VertexList_[ThisVertex] == _CompareNaviCell->VertexList_[CompareVertex])
				{
					++SharedVerticesCount;
				}

				// 2개의 정점을 공유하면서 무게중심이 다를때 인접한 면으로 판단한다.
				if (2 == SharedVerticesCount && CenterOfGravity_ != _CompareNaviCell->CenterOfGravity_)
				{
					AdjacentTriangles_.push_back(_CompareNaviCell);
					SharedVerticesCount = 0;
					return;
				}
			}
		}
	}
	else
	{
		// 조건 : 3개의 정점 중 1개의 정점이라도 공유하는 면이라면 인접한 면으로 판단			-> 몇방향이 나오는지 예측불가
		int ThisVertexSize = static_cast<int>(VertexList_.size());
		for (int ThisVertex = 0; ThisVertex < ThisVertexSize; ++ThisVertex)
		{
			int CompareVertexSize = static_cast<int>(_CompareNaviCell->VertexList_.size());
			for (int CompareVertex = 0; CompareVertex < CompareVertexSize; ++CompareVertex)
			{
				if (VertexList_[ThisVertex] == _CompareNaviCell->VertexList_[CompareVertex])
				{
					AdjacentTriangles_.push_back(_CompareNaviCell);
					return;
				}
			}
		}
	}
}

SJH_NaviCell* SJH_NaviCell::CompareVertices(const float4& _V0, const float4& _V1, const float4& _V2)
{
	std::vector<float4> CompareVertexList;
	CompareVertexList.push_back(_V0);
	CompareVertexList.push_back(_V1);
	CompareVertexList.push_back(_V2);

	// 세개의 버텍스를 모두 포함하고 있다면 나 자신을 반환
	int ThisVertexSize = static_cast<int>(VertexList_.size());
	int SameVertexCnt = 0;
	for (int ThisVertexNumber = 0; ThisVertexNumber < ThisVertexSize; ++ThisVertexNumber)
	{
		for (int CompareVertexNumber = 0; CompareVertexNumber < static_cast<int>(CompareVertexList.size()); ++CompareVertexNumber)
		{
			if (VertexList_[ThisVertexNumber] == CompareVertexList[CompareVertexNumber])
			{
				++SameVertexCnt;
				break;
			}
		}
	}

	if (3 == SameVertexCnt)
	{
		return this;
	}

	return nullptr;
}

void SJH_NaviCell::CreateSideLineInfo()
{
	// 각 정점을 연결하는 선분들의 각각의 중점을 계산
	int VertexCount = static_cast<int>(VertexList_.size());
	for (int Vertex = 0; Vertex < VertexCount; ++Vertex)
	{
		SideLine NewSideLine = {};

		// 해당 사이드라인의 시작점-끝점 선분
		NewSideLine.StartVertex_ = float4(0.0f, 0.0f, 0.0f, 0.0f);
		NewSideLine.EndVertex_ = float4(0.0f, 0.0f, 0.0f, 0.0f);
		if (VertexList_.size() == Vertex + 1)
		{
			// A,B,C 세개의 정점을 가지는 삼각형일때
			// 마지막정점인 C 정점순서이면 C-A 연결
			NewSideLine.StartVertex_ = VertexList_[Vertex];
			NewSideLine.EndVertex_ = VertexList_[0];
		}
		else
		{
			// 아니라면 A-B, B-C 연결
			NewSideLine.StartVertex_ = VertexList_[Vertex];
			NewSideLine.EndVertex_ = VertexList_[Vertex + 1];
		}

		// 선분의 중점을 계산하여 저장
		float4 MidPoint = (NewSideLine.EndVertex_ - NewSideLine.StartVertex_) * 0.5f;
		NewSideLine.MidPoint_ = MidPoint;

		// 총 3개의 사이드라인 생성
		SideLines_.push_back(NewSideLine);
	}
}

void SJH_NaviCell::CenterOfGravityCalculation()
{
	// 해당 면을 이루는 사이드라인의 정보를 모두 수집했으므로
	// 해당 면의 무게중심을 계산하고,
	float4 Vertex0 = VertexList_[0];
	float4 Vertex1 = VertexList_[1];
	float4 Vertex2 = VertexList_[2];
	CenterOfGravity_ = (Vertex0 + Vertex1 + Vertex2) / 3.0f;

	// 면의 무게중심에서 각 선분의 중점을 향하는 벡터의 길이를 알아낸다.
	int SideLineCount = static_cast<int>(SideLines_.size());
	for (int SideLine = 0; SideLine < SideLineCount; ++SideLine)
	{
		SideLines_[SideLine].ArrivalCost_ = std::sqrtf(std::powf(SideLines_[SideLine].MidPoint_.x - CenterOfGravity_.x, 2) + std::powf(SideLines_[SideLine].MidPoint_.y - CenterOfGravity_.y, 2) +
											std::powf(SideLines_[SideLine].MidPoint_.z - CenterOfGravity_.z, 2));
	}

	// 기본정보 셋팅 완료
	FaceInfoCompleted_ = true;
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
