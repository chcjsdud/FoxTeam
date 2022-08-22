#include "Precompile.h"
#include "SJH_Funnel.h"

std::list<float4> SJH_Funnel::PathOptimization(const float4& _StartPos, const float4& _EndPos, std::list<SJH_NaviCell*>& _MovePath)
{
	std::list<float4> ReturnPath;

	// 수신받은 시작점과 도착점 저장
	StartPos_ = _StartPos;
	EndPos_ = _EndPos;

	// 포탈정보 삭제 후
	LeftPortal_.clear();
	RightPortal_.clear();

	// A*로 얻어낸 최단경로를 통해 삼각형(셀)별 공유하는 Vertex를 얻어낸 뒤
	// 왼쪽점인지 오른쪽점인지 판단하여 목록 생성
	CreatePortalVertexList(_MovePath);

	// 왼쪽포탈, 오른쪽포탈 목록이 생성되었으므로
	// 시작점 기준으로 깔때기를 생성하여 이동경로 최적화 시작
	if (true == OptimizationStart(ReturnPath))
	{
		return ReturnPath;
	}

	return std::list<float4>();
}

void SJH_Funnel::CreatePortalVertexList(std::list<SJH_NaviCell*>& _MovePath)
{
	// 왼쪽포탈과 오른쪽포탈 목록 및 포탈별 중점 작성
	std::list<SJH_NaviCell*>::iterator StartIter = _MovePath.begin();
	std::list<SJH_NaviCell*>::iterator EndIter = _MovePath.end();
	for (; StartIter != EndIter; )
	{
		// 경로상의 두개의 셀(삼각형)이 공유하는 정점 Get
		std::vector<GameEngineVertex> ShareVertex;
		SJH_NaviCell* FirstCell = (*StartIter);

		++StartIter;
		if (StartIter != EndIter)
		{
			ShareVertex = FirstCell->SearchShareVertex((*StartIter));

			// 두개의 셀의 무게중심을 연결하는 방향벡터 생성
			float4 FirstCellGravity = FirstCell->GetCenterToGravity();
			float4 SecondCellGravity = (*StartIter)->GetCenterToGravity();
			float4 FrontVector = SecondCellGravity - FirstCellGravity;

			// 무게중심을 연결하는 방향벡터 기준 두개의 정점이 왼쪽인지 오른쪽인지 판별 후 목록에 저장
			for (int VertexIndex = 0; VertexIndex < static_cast<int>(ShareVertex.size()); ++VertexIndex)
			{
				// 시작점과 연결되는 벡터
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
	// 시작위치 저장
	_ReturnPath.push_back(StartPos_);

	// 포탈 검사전 셋팅
	// 1. 포탈을 검사할 시작위치 셋팅
	float4 StartPoint = StartPos_;

	// 2. 현재 포탈내에 다음포탈이 포함된다면 다음포탈이 현재 포탈이 된다.
	float4 CheckCurLPortal = float4(0.0f, 0.0f, 0.0f, 0.0f);		// 현재 포탈의 왼쪽 정점
	float4 CheckNextLPortal = float4(0.0f, 0.0f, 0.0f, 0.0f);		// 다음 포탈의 왼쪽 정점
	float4 CheckCurRPortal = float4(0.0f, 0.0f, 0.0f, 0.0f);		// 현재 포탈의 오른쪽 정점
	float4 CheckNextRPortal = float4(0.0f, 0.0f, 0.0f, 0.0f);		// 다음 포탈의 오른쪽 정점

	// 3. 현재 시작포탈
	// -> 다음 포탈이 현재시작포탈 내부에 존재할때 현재포탈 갱신
	int CurLPortalIndex = 0;
	int CurRPortalIndex = 0;

	// 최적경로 및 코너 정점좌표 탐색 시작
	while(true)
	{
		if (CurLPortalIndex >= static_cast<int>(LeftPortal_.size()) ||
			CurLPortalIndex + 1 >= static_cast<int>(LeftPortal_.size()) ||
			CurRPortalIndex >= static_cast<int>(RightPortal_.size()) ||
			CurRPortalIndex + 1 >= static_cast<int>(RightPortal_.size()))
		{
			break;
		}

		//============================================ 왼쪽 포탈 검사 정보 셋팅 ============================================//
		CheckCurLPortal = LeftPortal_[CurLPortalIndex];
		CheckNextLPortal = LeftPortal_[CurLPortalIndex + 1];

		float4 StartToCurLPortal = (CheckCurLPortal - StartPoint).NormalizeReturn3D();
		float4 StartToNextLPortal = (CheckNextLPortal - StartPoint).NormalizeReturn3D();
		float4 LPortalCross = float4::Cross3D(StartToNextLPortal, StartToCurLPortal).NormalizeReturn3D();
		float LPortalDot = float4::Dot3D(LPortalCross, float4(0.0f, 1.0f, 0.0f, 0.0f));

		//=========================================== 오른쪽 포탈 검사 정보 셋팅 ===========================================//
		CheckCurRPortal = RightPortal_[CurRPortalIndex];
		CheckNextRPortal = RightPortal_[CurRPortalIndex + 1];

		float4 StartToCurRPortal = (CheckCurRPortal - StartPoint).NormalizeReturn3D();
		float4 StartToNextRPortal = (CheckNextRPortal - StartPoint).NormalizeReturn3D();
		float4 RPortalCross = float4::Cross3D(StartToNextRPortal, StartToCurRPortal).NormalizeReturn3D();
		float RPortalDot = float4::Dot3D(RPortalCross, float4(0.0f, 1.0f, 0.0f, 0.0f));

		// < 다음 왼쪽포탈이 현재 왼쪽포탈의 오른쪽에 위치하므로 연결가능 포탈로 판정 >
		// 조건 1 : 현재 왼쪽포탈보다 다음 왼쪽포탈이 오른쪽에 위치하고, 오른쪽 포탈의 왼쪽에 위치한다면 연결가능한 포탈
		// 조건 2 : 현재 왼쪽포탈보다 다음 포탈이 왼쪽에 위치한다면 연결불가
		if (LPortalDot <= 0)
		{
			// < 오른쪽에 위치하거나 수평일때 >
			//
			// 코너 판정
			float4 RightCornerCheck = float4::Cross3D(StartToNextLPortal, StartToCurRPortal).NormalizeReturn3D();
			float RightCornerDot = float4::Dot3D(RightCornerCheck, float4(0.0f, 1.0f, 0.0f, 0.0f));
			if (RightCornerDot <= 0)
			{
				// 현재 오른쪽 포탈보다 다음 왼쪽포탈이 오른쪽에 위치하거나 수평하다면 현재 오른쪽 포탈이 경로로 지정
				_ReturnPath.push_back(CheckCurRPortal);
				StartPoint = CheckCurRPortal;

				++CurRPortalIndex;
				CurLPortalIndex = CurRPortalIndex;
				continue;
			}
			else
			{
				++CurLPortalIndex;

				// 오른쪽 포탈 검사를 위해 정보 갱신
				if (CurLPortalIndex >= static_cast<int>(LeftPortal_.size()))
				{
					break;
				}

				//============================================ 왼쪽 포탈 검사 정보 셋팅 ============================================//
				CheckCurLPortal = LeftPortal_[CurLPortalIndex];
				StartToCurLPortal = (CheckCurLPortal - StartPoint).NormalizeReturn3D();
			}
		}

		// < 다음 오른쪽 포탈이 현재 오른쪽 포탈의 왼쪽에 위치하므로 연결가능 포탈로 판정 >
		// 조건 1 : 현재 오른쪽포탈보다 다음 오른쪽포탈이 왼쪽에 위치하고, 왼쪽 포탈의 오른쪽에 위치한다면 연결가능한 포탈
		// 조건 2 : 현재 오른쪽포탈보다 다음 오른쪽포탈이 오른쪽에 위치한다면 연결불가
		if (RPortalDot >= 0)
		{
			// < 왼쪽에 위치하거나 수평일때 >
			//
			// 코너 판정
			float4 LeftCornerCheck = float4::Cross3D(StartToNextRPortal, StartToCurLPortal).NormalizeReturn3D();
			float LeftCornerDot = float4::Dot3D(LeftCornerCheck, float4(0.0f, 1.0f, 0.0f, 0.0f));
			if (LeftCornerDot >= 0)
			{
				// 현재 왼쪽 포탈보다 다음 오른쪽포탈이 왼쪽에 위치하거나 수평하다면 현재 왼쪽 포탈이 경로로 지정
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

		// 포탈 연결이 모두 불가능한경우 해당 포탈의 중점을 시작위치로 셋팅하고
		// 깔때기를 재설정 후 다시 탐색 시작
		if (LPortalDot > 0 && RPortalDot < 0)
		{
			// 두개의 포털 인덱스 중 더 작은 인덱스에 초점을 맞춘 중점을 시작위치로 셋팅
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

	// 모든 경로완성후 마지막 EndPos 저장
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
