#include "Precompile.h"
#include "SJH_Funnel.h"

std::list<float4> SJH_Funnel::PathOptimization(const float4& _StartPos, const float4& _EndPos, std::list<SJH_NaviCell*>& _MovePath)
{
	// ���Ź��� �������� ������ ����
	StartPos_ = _StartPos;
	EndPos_ = _EndPos;

	// ��Ż���� ���� �� ���ο� ��Ż���� ����
	LeftPortal_.clear();
	RightPortal_.clear();

	std::list<float4> ReturnPath;

	// A*�� ã�Ƴ� ��ΰ� 1���� �����Ѵٸ� -> �����ﰢ�������� �̵��Ϸ����Ҷ� ��� 1���� ����
	if (1 == _MovePath.size())
	{
		// �׷��Ƿ� StartPos_ -> EndPos_�� �����θ��� ��ȯ
		ReturnPath.push_back(StartPos_);
		ReturnPath.push_back(EndPos_);
		return ReturnPath;
	}

	// A*�� �� �ִܰ�θ� ���� �ﰢ��(��)�� �����ϴ� Vertex�� �� ��
	// ���������� ������������ �Ǵ��Ͽ� ��� ����
	CreatePortalVertexList(_MovePath);

	// ������Ż, ��������Ż ����� �����Ǿ����Ƿ�
	// ������ �������� �򶧱⸦ �����Ͽ� �̵���� ����ȭ ����
	if (true == OptimizationStart(ReturnPath))
	{
		return ReturnPath;
	}

	return std::list<float4>();
}

void SJH_Funnel::CreatePortalVertexList(std::list<SJH_NaviCell*>& _MovePath)
{
	// ������Ż�� ��������Ż ��� �ۼ�
	std::list<SJH_NaviCell*>::iterator StartIter = _MovePath.begin();
	std::list<SJH_NaviCell*>::iterator EndIter = _MovePath.end();
	for (; StartIter != EndIter; )
	{
		// ��λ��� �ΰ��� ��(�ﰢ��)�� �����ϴ� ���� Get
		std::vector<GameEngineVertex> ShareVertex;
		SJH_NaviCell* FirstCell = (*StartIter);

		++StartIter;
		if (StartIter != EndIter)
		{
			ShareVertex = FirstCell->SearchShareVertex((*StartIter));

			// �ΰ��� ���� �����߽��� �����ϴ� ���⺤�� ����
			float4 FirstCellGravity = FirstCell->GetCenterToGravity();
			float4 SecondCellGravity = (*StartIter)->GetCenterToGravity();
			float4 vecGravityToGravity = SecondCellGravity - FirstCellGravity;
			vecGravityToGravity.Normalize3D();

			// �����߽��� �����ϴ� ���⺤�� ���� �ΰ��� ������ �������� ���������� �Ǻ� �� ��Ͽ� ����

			// �����߽��� �����ϴ� ���Ϳ� ���Ͽ� ������Ż�� ��������Ż�� �˾Ƴ���.
			for (int VertexIndex = 0; VertexIndex < static_cast<int>(ShareVertex.size()); ++VertexIndex)
			{
				// �������� ����Ǵ� ����
				float4 CheckVector = ShareVertex[VertexIndex].POSITION - FirstCellGravity;
				CheckVector.Normalize3D();

				float4 Cross = float4::Cross3D(CheckVector, vecGravityToGravity).NormalizeReturn3D();
				float Dot = float4::Dot3D(Cross, float4(0.0f, 1.0f, 0.0f, 0.0f));
				if (Dot >= 0)
				{
					RightPortal_.push_back(ShareVertex[VertexIndex].POSITION);
				}
				else if (Dot < 0)
				{
					LeftPortal_.push_back(ShareVertex[VertexIndex].POSITION);
				}
			}
		}
	}
}

bool SJH_Funnel::OptimizationStart(std::list<float4>& _ReturnPath)
{
	// ������ġ ����
	_ReturnPath.push_back(StartPos_);

	// ��� ��������



	// ��� ��οϼ��� ������ EndPos ����
	_ReturnPath.push_back(EndPos_);
	if (2 < _ReturnPath.size())
	{
		return true;
	}

	return false;
}

SJH_Funnel::SJH_Funnel()
	: StartPos_(float4(0.0f, 0.0f, 0.0f, 0.0f))
	, EndPos_(float4(0.0f, 0.0f, 0.0f, 0.0f))
{
}

SJH_Funnel::~SJH_Funnel()
{
}
