#include "Precompile.h"
#include "SJH_NaviCell.h"

void SJH_NaviCell::CreateNavigationCellInfo(int _Index, int _MeshIndex, int _FaceIndex, std::vector<GameEngineVertex> _VertexList, std::vector<UINT> _IndexList)
{
	// �⺻���� ����
	Index_ = _Index;
	MeshIndex_ = _MeshIndex;
	FaceIndex_ = _FaceIndex;
	VertexList_ = _VertexList;
	IndexList_ = _IndexList;

	// ���Ź��� 3���� ������ �����ϴ� ������ ������ ���
	CreateSideLineInfo();

	// ���Ź��� 3���� �������� �����߽� ���
	CenterOfGravityCalculation();
}

void SJH_NaviCell::SearchAdjacentTriangles(SJH_NaviCell* _CompareNaviCell, bool _2Vertex)
{
	if (true == _2Vertex)
	{
		// ����: 2���� ������ �����ϰ� �����߽��� �ٸ� ���̶�� ������ ������ �Ǵ� -> �ִ� �����θ��� 3���� ���� �����Ѵ�
		int ThisVertexSize = static_cast<int>(VertexList_.size());

		int SharedVerticesCount = 0;
		for (int ThisVertex = 0; ThisVertex < ThisVertexSize; ++ThisVertex)
		{
			int CompareVertexSize = static_cast<int>(_CompareNaviCell->VertexList_.size());
			for (int CompareVertex = 0; CompareVertex < CompareVertexSize; ++CompareVertex)
			{
				if (VertexList_[ThisVertex].POSITION == _CompareNaviCell->VertexList_[CompareVertex].POSITION)
				{
					++SharedVerticesCount;
				}

				// 2���� ������ �����ϸ鼭 �����߽��� �ٸ��� ������ ������ �Ǵ��Ѵ�.
				if (2 == SharedVerticesCount &&																	// 2���� ������ �����ϰ�
					CenterOfGravity_ != _CompareNaviCell->CenterOfGravity_)										// ������ ���� �����߽��� �ٸ����
				{
					AdjacentTriangles_.push_back(_CompareNaviCell);
					return;
				}
			}
		}
	}
	else
	{
		// ���� : 3���� ���� �� 1���� �����̶� �����ϴ� ���̶�� ������ ������ �Ǵ�	 -> �ִ� �����θ��� 12���� ���� �����Ѵ�
		int ThisVertexSize = static_cast<int>(VertexList_.size());
		for (int ThisVertex = 0; ThisVertex < ThisVertexSize; ++ThisVertex)
		{
			int CompareVertexSize = static_cast<int>(_CompareNaviCell->VertexList_.size());
			for (int CompareVertex = 0; CompareVertex < CompareVertexSize; ++CompareVertex)
			{
				if (VertexList_[ThisVertex].POSITION == _CompareNaviCell->VertexList_[CompareVertex].POSITION)
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
			if (VertexList_[ThisVertexNumber].POSITION == CompareVertexList[CompareVertexNumber])
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

bool SJH_NaviCell::CheckPointisIncludedIntheTriangle(const float4& _Position)
{
	// �ش� �ﰢ���� ���ο� ����(_Position)�� �����ϸ� true ��ȯ
	// Inside�� Zero�ΰ�� �𼭸��� ���� ����
	float4 Inside = DirectX::Internal::PointOnPlaneInsideTriangle(_Position.DirectVector, VertexList_[0].POSITION.DirectVector, VertexList_[1].POSITION.DirectVector, VertexList_[2].POSITION.DirectVector);
	if (Inside != float4(0.0f, 0.0f, 0.0f, 0.0f))
	{
		return true;
	}

	return false;
}

std::vector<GameEngineVertex> SJH_NaviCell::SearchShareVertex(SJH_NaviCell* _ShareCell)
{
	std::vector<GameEngineVertex> ReturnVertex;

	int VertexListSize = static_cast<int>(VertexList_.size());
	for (int i = 0; i < VertexListSize; ++i)
	{
		for (int j = 0; j < static_cast<int>(_ShareCell->VertexList_.size()); ++j)
		{
			if (VertexList_[i].POSITION == _ShareCell->VertexList_[j].POSITION)
			{
				ReturnVertex.push_back(VertexList_[i]);
			}
		}
	}

	if (2 == ReturnVertex.size())
	{
		return ReturnVertex;
	}

	return std::vector<GameEngineVertex>();
}

void SJH_NaviCell::CreateSideLineInfo()
{
	// �� ������ �����ϴ� ���е��� ������ ������ ���
	int VertexCount = static_cast<int>(VertexList_.size());
	for (int Vertex = 0; Vertex < VertexCount; ++Vertex)
	{
		SideLine NewSideLine = {};

		// �ش� ���̵������ ������-���� ����
		if (VertexList_.size() == Vertex + 1)
		{
			// A,B,C ������ ������ ������ �ﰢ���϶�
			// ������������ C ���������̸� C-A ����
			NewSideLine.Vertex_.push_back(VertexList_[Vertex]);
			NewSideLine.Vertex_.push_back(VertexList_[0]);
		}
		else
		{
			// �ƴ϶�� A-B, B-C ����
			NewSideLine.Vertex_.push_back(VertexList_[Vertex]);
			NewSideLine.Vertex_.push_back(VertexList_[Vertex + 1]);
		}

		// ������ ������ ����Ͽ� ����
		float4 MidPoint = (NewSideLine.Vertex_[1].POSITION - NewSideLine.Vertex_[0].POSITION) * 0.5f;
		NewSideLine.MidPoint_ = MidPoint;

		// �� 3���� ���̵���� ����
		SideLines_.push_back(NewSideLine);
	}
}

void SJH_NaviCell::CenterOfGravityCalculation()
{
	// �ش� ���� �̷�� ���̵������ ������ ��� ���������Ƿ�
	// �ش� ���� �����߽��� ����ϰ�,
	float4 Vertex0 = VertexList_[0].POSITION;
	float4 Vertex1 = VertexList_[1].POSITION;
	float4 Vertex2 = VertexList_[2].POSITION;
	CenterOfGravity_ = (Vertex0 + Vertex1 + Vertex2) / 3.0f;
}

void SJH_NaviCell::StandingOntheCellCheck()
{
}

SJH_NaviCell::SJH_NaviCell()
	: Index_(-1)
	, MeshIndex_(-1)
	, FaceIndex_(-1)
	, CenterOfGravity_(float4::ZERO)
{
}

SJH_NaviCell::~SJH_NaviCell()
{
}
