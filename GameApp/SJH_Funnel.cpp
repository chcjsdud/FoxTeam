#include "Precompile.h"
#include "SJH_Funnel.h"

std::list<float4> SJH_Funnel::PathOptimization(const float4& _StartPos, const float4& _EndPos, std::list<SJH_NaviCell*>& _MovePath)
{
	// 수신받은 시작점과 도착점 저장
	StartPos_ = _StartPos;
	EndPos_ = _EndPos;

	// 포탈정보 삭제 후 새로운 포탈정보 생성
	LeftPortal_.clear();
	RightPortal_.clear();

	std::list<float4> ReturnPath;

	// A*로 찾아낸 경로가 1개만 존재한다면 -> 같은삼각형내에서 이동하려고할때 경로 1개만 존재
	if (1 == _MovePath.size())
	{
		// 그러므로 StartPos_ -> EndPos_의 결과경로만을 반환
		ReturnPath.push_back(StartPos_);
		ReturnPath.push_back(EndPos_);
		return ReturnPath;
	}

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

	// 경로 만들기시작



	// 모든 경로완성후 마지막 EndPos 저장
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
