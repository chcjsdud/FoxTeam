#include "Precompile.h"
#include "SJH_Funnel.h"

std::list<float4> SJH_Funnel::PathOptimization(const float4& _StartPos, const float4& _EndPos, std::list<SJH_NaviCell*>& _MovePath)
{
	std::list<float4> ReturnPath;

	// 수신받은 시작점과 도착점 저장
	StartPos_ = _StartPos;
	EndPos_ = _EndPos;

	// 포탈정보 삭제 후
	if (false == LeftPortal_.empty() && false == RightPortal_.empty())
	{
		LeftPortal_.clear();
		RightPortal_.clear();
	}

	// (경로의 갯수 - 1) = 포탈의 갯수
	TotalPortalCount_ = static_cast<int>(_MovePath.size()) - 1;

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
	// 왼쪽포탈과 오른쪽포탈 목록 작성
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
			float4 vecGravityToGravity = SecondCellGravity - FirstCellGravity;
			vecGravityToGravity.Normalize3D();

			// 무게중심을 연결하는 방향벡터 기준 두개의 정점이 왼쪽인지 오른쪽인지 판별 후 목록에 저장

			// 무게중심을 연결하는 벡터와 비교하여 왼쪽포탈과 오른쪽포탈을 알아낸다.
			for (int VertexIndex = 0; VertexIndex < static_cast<int>(ShareVertex.size()); ++VertexIndex)
			{
				// 시작점과 연결되는 벡터
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
	// 시작위치 저장
	_ReturnPath.push_back(StartPos_);

	float4 LeftPortalVertex = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 RightPortalVertex = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// 최적경로 및 코너 정점좌표 탐색 시작
	for (int PortalIndex = 0; PortalIndex < TotalPortalCount_; ++PortalIndex)
	{
		// (왼쪽 포탈정점), (왼쪽포탈정점 + 1) 비교하여 깔때기를 좁힐수있는지 판단
		if (true == LeftPortalCheck(StartPos_, PortalIndex))
		{
			// 깔때기를 좁힐수 있으므로 왼쪽 깔때기를 좁힌다.



		}

		// (오른쪽 포탈정점), (오른쪽포탈정점 + 1) 비교하여 깔때기를 좁힐수있는지 판단
		if (true == RightPortalCheck(StartPos_, PortalIndex))
		{
			// 깔때기를 좁힐수 있으므로 오른쪽 깔때기를 좁힌다.



		}




	}

	// 모든 경로완성후 마지막 EndPos 저장
	_ReturnPath.push_back(EndPos_);
	if (2 < _ReturnPath.size())
	{
		return true;
	}

	return false;
}

bool SJH_Funnel::LeftPortalCheck(float4& _StartPos, int _PortalIndex)
{
	// 현재 오른쪽포탈까지의 벡터와 교차한다면 시작위치가 변경
	// 교차 : 왼쪽포탈벡터가 오른쪽포탈벡터보다 오른쪽에 존재하면 교차했다고 판정
	// 시작위치 = 현재 오른쪽포탈의 정점으로 갱신





	// 왼쪽과 오른쪽포탈이 교차했으므로 오른쪽포탈정점을 경로에 추가하고 시작위치를 오른쪽포탈 정점으로 설정





	// 아니고 다음 인덱스의 포탈이 현재 포탈의 오른쪽에 존재한다면 연결가능
	// 단, 다음인덱스의 포탈이 현재 포탈의 왼쪽에 존재한다면 연결불가판정



	return false;
}

bool SJH_Funnel::RightPortalCheck(float4& _StartPos, int _PortalIndex)
{
	// 현재 왼쪽포탈까지의 벡터와 교차한다면 시작위치가 변경
	// 교차 : 오른쪽포탈벡터가 왼쪽포탈벡터보다 왼쪽에 존재하면 교차했다고 판명
	// 시작위치 = 현재 왼쪽포탈의 정점으로 갱신





	// 왼쪽과 오른쪽포탈이 교차했으므로 왼쪽포탈정점을 경로에 추가하고 시작위치를 왼쪽포탈 정점으로 설정





	// 아니고 다음 인덱스의 포탈이 현재 포탈의 왼쪽에 존재한다면 연결가능
	// 단, 다음인덱스의 포탈이 현재 포탈의 오른쪽에 존재한다면 연결불가판정





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
