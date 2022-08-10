#include "Precompile.h"
#include "SJH_NaviCell.h"

void SJH_NaviCell::CreateNavigationCellInfo(int _MeshIndex, int _FaceIndex, std::vector<float4> _VertexList, std::vector<UINT> _IndexList)
{
	// �⺻���� ����
	MeshIndex_ = _MeshIndex;
	FaceIndex_ = _FaceIndex;
	VertexList_ = _VertexList;
	IndexList_ = _IndexList;

	// ���Ź��� 3���� ������ �����ϴ� ������ ������ ���
	CreateSideLineInfo();

	// ���Ź��� 3���� �������� �����߽� ���
	CenterOfGravityCalculation();
}

void SJH_NaviCell::SearchAdjacentTriangles(SJH_NaviCell* _CompareNaviCell, bool _3Direction)
{
	if (true == _3Direction)
	{
		// ����: 2���� ������ �����ϰ� �����߽��� �ٸ� ���̶�� ������ ������ �Ǵ� -> 3������ �����Ѹ��� �������ְԵȴ�.
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

				// 2���� ������ �����ϸ鼭 �����߽��� �ٸ��� ������ ������ �Ǵ��Ѵ�.
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
		// ���� : 3���� ���� �� 1���� �����̶� �����ϴ� ���̶�� ������ ������ �Ǵ�			-> ������� �������� �����Ұ�
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

	// ������ ���ؽ��� ��� �����ϰ� �ִٸ� �� �ڽ��� ��ȯ
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
	// �� ������ �����ϴ� ���е��� ������ ������ ���
	int VertexCount = static_cast<int>(VertexList_.size());
	for (int Vertex = 0; Vertex < VertexCount; ++Vertex)
	{
		SideLine NewSideLine = {};

		// �ش� ���̵������ ������-���� ����
		NewSideLine.StartVertex_ = float4(0.0f, 0.0f, 0.0f, 0.0f);
		NewSideLine.EndVertex_ = float4(0.0f, 0.0f, 0.0f, 0.0f);
		if (VertexList_.size() == Vertex + 1)
		{
			// A,B,C ������ ������ ������ �ﰢ���϶�
			// ������������ C ���������̸� C-A ����
			NewSideLine.StartVertex_ = VertexList_[Vertex];
			NewSideLine.EndVertex_ = VertexList_[0];
		}
		else
		{
			// �ƴ϶�� A-B, B-C ����
			NewSideLine.StartVertex_ = VertexList_[Vertex];
			NewSideLine.EndVertex_ = VertexList_[Vertex + 1];
		}

		// ������ ������ ����Ͽ� ����
		float4 MidPoint = (NewSideLine.EndVertex_ - NewSideLine.StartVertex_) * 0.5f;
		NewSideLine.MidPoint_ = MidPoint;

		// �� 3���� ���̵���� ����
		SideLines_.push_back(NewSideLine);
	}
}

void SJH_NaviCell::CenterOfGravityCalculation()
{
	// �ش� ���� �̷�� ���̵������ ������ ��� ���������Ƿ�
	// �ش� ���� �����߽��� ����ϰ�,
	float4 Vertex0 = VertexList_[0];
	float4 Vertex1 = VertexList_[1];
	float4 Vertex2 = VertexList_[2];
	CenterOfGravity_ = (Vertex0 + Vertex1 + Vertex2) / 3.0f;

	// ���� �����߽ɿ��� �� ������ ������ ���ϴ� ������ ���̸� �˾Ƴ���.
	int SideLineCount = static_cast<int>(SideLines_.size());
	for (int SideLine = 0; SideLine < SideLineCount; ++SideLine)
	{
		SideLines_[SideLine].ArrivalCost_ = std::sqrtf(std::powf(SideLines_[SideLine].MidPoint_.x - CenterOfGravity_.x, 2) + std::powf(SideLines_[SideLine].MidPoint_.y - CenterOfGravity_.y, 2) +
											std::powf(SideLines_[SideLine].MidPoint_.z - CenterOfGravity_.z, 2));
	}

	// �⺻���� ���� �Ϸ�
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
