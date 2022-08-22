#include "Precompile.h"
#include "SJH_Funnel.h"

std::list<float4> SJH_Funnel::PathOptimization(const float4& _StartPos, const float4& _EndPos, std::list<SJH_NaviCell*>& _MovePath)
{
	std::list<float4> ReturnPath;

	// ���Ź��� �������� ������ ����
	StartPos_ = _StartPos;
	EndPos_ = _EndPos;

	// ��Ż���� ���� ��
	LeftPortal_.clear();
	RightPortal_.clear();

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
	// ������Ż�� ��������Ż ��� �� ��Ż�� ���� �ۼ�
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
			float4 FrontVector = SecondCellGravity - FirstCellGravity;

			// �����߽��� �����ϴ� ���⺤�� ���� �ΰ��� ������ �������� ���������� �Ǻ� �� ��Ͽ� ����
			for (int VertexIndex = 0; VertexIndex < static_cast<int>(ShareVertex.size()); ++VertexIndex)
			{
				// �������� ����Ǵ� ����
				float4 TargetVector = ShareVertex[VertexIndex].POSITION - FirstCellGravity;

				float4 Cross = float4::Cross3D(TargetVector, FrontVector).NormalizeReturn3D();
				float Dot = float4::Dot3D(Cross, float4(0.0f, 1.0f, 0.0f, 0.0f));
				if (Dot > 0.0f)
				{
					LeftPortal_.push_back(ShareVertex[VertexIndex].POSITION);
				}
				else
				{
					RightPortal_.push_back(ShareVertex[VertexIndex].POSITION);
				}
			}
		}
	}
}

bool SJH_Funnel::OptimizationStart(std::list<float4>& _ReturnPath)
{
	// ������ġ ����
	_ReturnPath.push_back(StartPos_);

	// ��Ż �˻��� ����
	// 1. ��Ż�� �˻��� ������ġ ����
	float4 StartPoint = StartPos_;

	// 2. ���� ��Ż���� ������Ż�� ���Եȴٸ� ������Ż�� ���� ��Ż�� �ȴ�.
	float4 CheckCurLPortal = float4(0.0f, 0.0f, 0.0f, 0.0f);		// ���� ��Ż�� ���� ����
	float4 CheckNextLPortal = float4(0.0f, 0.0f, 0.0f, 0.0f);		// ���� ��Ż�� ���� ����
	float4 CheckCurRPortal = float4(0.0f, 0.0f, 0.0f, 0.0f);		// ���� ��Ż�� ������ ����
	float4 CheckNextRPortal = float4(0.0f, 0.0f, 0.0f, 0.0f);		// ���� ��Ż�� ������ ����

	// 3. ���� ������Ż
	// -> ���� ��Ż�� ���������Ż ���ο� �����Ҷ� ������Ż ����
	int CurLPortalIndex = 0;
	int CurRPortalIndex = 0;

	// ������� �� �ڳ� ������ǥ Ž�� ����
	while(true)
	{
		if (CurLPortalIndex >= static_cast<int>(LeftPortal_.size()) ||
			CurLPortalIndex + 1 >= static_cast<int>(LeftPortal_.size()) ||
			CurRPortalIndex >= static_cast<int>(RightPortal_.size()) ||
			CurRPortalIndex + 1 >= static_cast<int>(RightPortal_.size()))
		{
			break;
		}

		//============================================ ���� ��Ż �˻� ���� ���� ============================================//
		CheckCurLPortal = LeftPortal_[CurLPortalIndex];
		CheckNextLPortal = LeftPortal_[CurLPortalIndex + 1];

		float4 StartToCurLPortal = (CheckCurLPortal - StartPoint).NormalizeReturn3D();
		float4 StartToNextLPortal = (CheckNextLPortal - StartPoint).NormalizeReturn3D();
		float4 LPortalCross = float4::Cross3D(StartToNextLPortal, StartToCurLPortal).NormalizeReturn3D();
		float LPortalDot = float4::Dot3D(LPortalCross, float4(0.0f, 1.0f, 0.0f, 0.0f));

		//=========================================== ������ ��Ż �˻� ���� ���� ===========================================//
		CheckCurRPortal = RightPortal_[CurRPortalIndex];
		CheckNextRPortal = RightPortal_[CurRPortalIndex + 1];

		float4 StartToCurRPortal = (CheckCurRPortal - StartPoint).NormalizeReturn3D();
		float4 StartToNextRPortal = (CheckNextRPortal - StartPoint).NormalizeReturn3D();
		float4 RPortalCross = float4::Cross3D(StartToNextRPortal, StartToCurRPortal).NormalizeReturn3D();
		float RPortalDot = float4::Dot3D(RPortalCross, float4(0.0f, 1.0f, 0.0f, 0.0f));

		// < ���� ������Ż�� ���� ������Ż�� �����ʿ� ��ġ�ϹǷ� ���ᰡ�� ��Ż�� ���� >
		// ���� 1 : ���� ������Ż���� ���� ������Ż�� �����ʿ� ��ġ�ϰ�, ������ ��Ż�� ���ʿ� ��ġ�Ѵٸ� ���ᰡ���� ��Ż
		// ���� 2 : ���� ������Ż���� ���� ��Ż�� ���ʿ� ��ġ�Ѵٸ� ����Ұ�
		if (LPortalDot <= 0)
		{
			// < �����ʿ� ��ġ�ϰų� �����϶� >
			//
			// �ڳ� ����
			float4 RightCornerCheck = float4::Cross3D(StartToNextLPortal, StartToCurRPortal).NormalizeReturn3D();
			float RightCornerDot = float4::Dot3D(RightCornerCheck, float4(0.0f, 1.0f, 0.0f, 0.0f));
			if (RightCornerDot <= 0)
			{
				// ���� ������ ��Ż���� ���� ������Ż�� �����ʿ� ��ġ�ϰų� �����ϴٸ� ���� ������ ��Ż�� ��η� ����
				_ReturnPath.push_back(CheckCurRPortal);
				StartPoint = CheckCurRPortal;

				++CurRPortalIndex;
				CurLPortalIndex = CurRPortalIndex;
				continue;
			}
			else
			{
				++CurLPortalIndex;

				// ������ ��Ż �˻縦 ���� ���� ����
				if (CurLPortalIndex >= static_cast<int>(LeftPortal_.size()))
				{
					break;
				}

				//============================================ ���� ��Ż �˻� ���� ���� ============================================//
				CheckCurLPortal = LeftPortal_[CurLPortalIndex];
				StartToCurLPortal = (CheckCurLPortal - StartPoint).NormalizeReturn3D();
			}
		}

		// < ���� ������ ��Ż�� ���� ������ ��Ż�� ���ʿ� ��ġ�ϹǷ� ���ᰡ�� ��Ż�� ���� >
		// ���� 1 : ���� ��������Ż���� ���� ��������Ż�� ���ʿ� ��ġ�ϰ�, ���� ��Ż�� �����ʿ� ��ġ�Ѵٸ� ���ᰡ���� ��Ż
		// ���� 2 : ���� ��������Ż���� ���� ��������Ż�� �����ʿ� ��ġ�Ѵٸ� ����Ұ�
		if (RPortalDot >= 0)
		{
			// < ���ʿ� ��ġ�ϰų� �����϶� >
			//
			// �ڳ� ����
			float4 LeftCornerCheck = float4::Cross3D(StartToNextRPortal, StartToCurLPortal).NormalizeReturn3D();
			float LeftCornerDot = float4::Dot3D(LeftCornerCheck, float4(0.0f, 1.0f, 0.0f, 0.0f));
			if (LeftCornerDot >= 0)
			{
				// ���� ���� ��Ż���� ���� ��������Ż�� ���ʿ� ��ġ�ϰų� �����ϴٸ� ���� ���� ��Ż�� ��η� ����
				_ReturnPath.push_back(CheckCurLPortal);
				StartPoint = CheckCurLPortal;

				++CurLPortalIndex;
				CurRPortalIndex = CurLPortalIndex;
				continue;
			}
			else
			{
				++CurRPortalIndex;
			}
		}

		// ��Ż ������ ��� �Ұ����Ѱ�� �ش� ��Ż�� ������ ������ġ�� �����ϰ�
		// �򶧱⸦ �缳�� �� �ٽ� Ž�� ����
		if (LPortalDot > 0 && RPortalDot < 0)
		{
			// �ΰ��� ���� �ε��� �� �� ���� �ε����� ������ ���� ������ ������ġ�� ����
			int CheckIndex = CurLPortalIndex < CurRPortalIndex ? CurLPortalIndex : CurRPortalIndex;

			float4 Left = LeftPortal_[CheckIndex];
			float4 Right = RightPortal_[CheckIndex];
			float4 MidPoint = (Left + Right) * 0.5f;

			StartPoint = MidPoint;
			_ReturnPath.push_back(StartPoint);
			
			++CheckIndex;
			CurLPortalIndex = CheckIndex;
			CurRPortalIndex = CheckIndex;
		}
	}

	// ��� ��οϼ��� ������ EndPos ����
	_ReturnPath.push_back(EndPos_);

	return true;
}

SJH_Funnel::SJH_Funnel()
	: StartPos_(float4(0.0f, 0.0f, 0.0f, 0.0f))
	, EndPos_(float4(0.0f, 0.0f, 0.0f, 0.0f))
{
}

SJH_Funnel::~SJH_Funnel()
{
}
