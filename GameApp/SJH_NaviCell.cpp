#include "Precompile.h"
#include "SJH_NaviCell.h"

void SJH_NaviCell::CreateNavigationCellInfo(int _MeshIndex, int _FaceIndex, std::vector<float4> _VertexList)
{
	// �⺻���� ����
	MeshIndex_ = _MeshIndex;
	FaceIndex_ = _FaceIndex;
	VertexList_ = _VertexList;

	// ���Ź��� 3���� ������ �����ϴ� ������ ������ ���
	MidPointCalculation();

	// ���Ź��� 3���� �������� �����߽� ���
	CenterOfGravityCalculation();
}

void SJH_NaviCell::SearchAdjacentTriangles()
{
	// �ش� ��� ������ ��� �ﰢ���� Ž���Ͽ� ����ۼ�
	// ��, ������ ���� ���������ʴٸ� ����� ���������ʴ´�.




}

void SJH_NaviCell::MidPointCalculation()
{
	// �� ������ �����ϴ� ���е��� ������ ������ ���


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
