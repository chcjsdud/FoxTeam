#include "Precompile.h"
#include "SJH_Funnel.h"

std::list<float4> SJH_Funnel::PathOptimization(const float4& _StartPos, const float4& _EndPos, std::list<SJH_NaviCell*>& _MovePath)
{
	std::list<float4> ReturnPath;

	// ���Ź��� �������� ������ ����
	StartPos_ = _StartPos;
	EndPos_ = _EndPos;

	// ��Ż���� ���� ��
	if (false == LeftPortal_.empty() && false == RightPortal_.empty())
	{
		LeftPortal_.clear();
		RightPortal_.clear();
	}

	// (����� ���� - 1) = ��Ż�� ����
	TotalPortalCount_ = static_cast<int>(_MovePath.size()) - 1;

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

	float4 LeftPortalVertex = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 RightPortalVertex = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// ������� �� �ڳ� ������ǥ Ž�� ����
	for (int PortalIndex = 0; PortalIndex < TotalPortalCount_; ++PortalIndex)
	{
		// (���� ��Ż����), (������Ż���� + 1) ���Ͽ� �򶧱⸦ �������ִ��� �Ǵ�
		if (true == LeftPortalCheck(StartPos_, PortalIndex))
		{
			// �򶧱⸦ ������ �����Ƿ� ���� �򶧱⸦ ������.



		}

		// (������ ��Ż����), (��������Ż���� + 1) ���Ͽ� �򶧱⸦ �������ִ��� �Ǵ�
		if (true == RightPortalCheck(StartPos_, PortalIndex))
		{
			// �򶧱⸦ ������ �����Ƿ� ������ �򶧱⸦ ������.



		}




	}

	// ��� ��οϼ��� ������ EndPos ����
	_ReturnPath.push_back(EndPos_);
	if (2 < _ReturnPath.size())
	{
		return true;
	}

	return false;
}

bool SJH_Funnel::LeftPortalCheck(float4& _StartPos, int _PortalIndex)
{
	// ���� ��������Ż������ ���Ϳ� �����Ѵٸ� ������ġ�� ����
	// ���� : ������Ż���Ͱ� ��������Ż���ͺ��� �����ʿ� �����ϸ� �����ߴٰ� ����
	// ������ġ = ���� ��������Ż�� �������� ����





	// ���ʰ� ��������Ż�� ���������Ƿ� ��������Ż������ ��ο� �߰��ϰ� ������ġ�� ��������Ż �������� ����





	// �ƴϰ� ���� �ε����� ��Ż�� ���� ��Ż�� �����ʿ� �����Ѵٸ� ���ᰡ��
	// ��, �����ε����� ��Ż�� ���� ��Ż�� ���ʿ� �����Ѵٸ� ����Ұ�����



	return false;
}

bool SJH_Funnel::RightPortalCheck(float4& _StartPos, int _PortalIndex)
{
	// ���� ������Ż������ ���Ϳ� �����Ѵٸ� ������ġ�� ����
	// ���� : ��������Ż���Ͱ� ������Ż���ͺ��� ���ʿ� �����ϸ� �����ߴٰ� �Ǹ�
	// ������ġ = ���� ������Ż�� �������� ����





	// ���ʰ� ��������Ż�� ���������Ƿ� ������Ż������ ��ο� �߰��ϰ� ������ġ�� ������Ż �������� ����





	// �ƴϰ� ���� �ε����� ��Ż�� ���� ��Ż�� ���ʿ� �����Ѵٸ� ���ᰡ��
	// ��, �����ε����� ��Ż�� ���� ��Ż�� �����ʿ� �����Ѵٸ� ����Ұ�����





	return false;
}

SJH_Funnel::SJH_Funnel()
	: StartPos_(float4(0.0f, 0.0f, 0.0f, 0.0f))
	, EndPos_(float4(0.0f, 0.0f, 0.0f, 0.0f))
	, TotalPortalCount_(0)
{
}

SJH_Funnel::~SJH_Funnel()
{
}
