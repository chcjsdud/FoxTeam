#include "Precompile.h"
#include "SJH_NaviCell.h"

void SJH_NaviCell::CreateNavigationCellInfo(int _MeshIndex, int _FaceIndex, std::vector<float4> _VertexList)
{
	// �⺻���� ����
	MeshIndex_ = _MeshIndex;
	FaceIndex_ = _FaceIndex;
	VertexList_ = _VertexList;

	// ���Ź��� 3���� ������ �����ϴ� ������ ������ ���
	CreateSideLineInfo();

	// ���Ź��� 3���� �������� �����߽� ���
	CenterOfGravityCalculation();

	// �� ������ �����߽��� �����ϴ� ���⺤�� ����

}

void SJH_NaviCell::SearchAdjacentTriangles()
{
	// �ش� ��� ������ ��� �ﰢ���� Ž���Ͽ� ����ۼ�
	// ��, ������ ���� ���������ʴٸ� ����� ���������ʴ´�.




}

void SJH_NaviCell::CreateSideLineInfo()
{
	// �� ������ �����ϴ� ���е��� ������ ������ ���
	int VertexCount = static_cast<int>(VertexList_.size());
	for (int Vertex = 0; Vertex < VertexCount; ++Vertex)
	{
		SideLine NewSideLine = {};

		// �ش� ���̵������ ������-���� ����
		NewSideLine.StartVertex_ = float4();
		NewSideLine.EndVertex_ = float4();

		// ������ ������ ����Ͽ� ����



		NewSideLine.MidPoint_ = float4();

		// �� 3���� ���̵���� ����
		SideLines_.push_back(NewSideLine);
	}
}

void SJH_NaviCell::CenterOfGravityCalculation()
{
	// 3���� ������ �� ������ �����ϴ� ���е��� ������ ������ �̿��Ͽ� �����߽��� ���


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
