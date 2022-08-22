#pragma once
#include "SJH_NaviCell.h"

// 분류 : 
// 용도 : A*로 얻어낸 경로의 최적의 직선경로를 생성
// 설명 : A*로 얻어낸 경로(셀집합)을 수신받아 가시적인 직선경로를 계산하여 관리
class SJH_Funnel
{
public:
	std::list<float4> PathOptimization(const float4& _StartPos, const float4& _EndPos, std::list<SJH_NaviCell*>& _MovePath);

protected:

private:
	void CreatePortalVertexList(std::list<SJH_NaviCell*>& _MovePath);
	bool OptimizationStart(std::list<float4>& _ReturnPath);

public:
	SJH_Funnel();
	~SJH_Funnel();

protected:
	SJH_Funnel(const SJH_Funnel& _other) = delete;
	SJH_Funnel(SJH_Funnel&& _other) noexcept = delete;

private:
	SJH_Funnel& operator=(const SJH_Funnel& _other) = delete;
	SJH_Funnel& operator=(const SJH_Funnel&& _other) = delete;

public:

protected:

private: // 수신정보
	float4 StartPos_;						// 이동시작위치
	float4 EndPos_;							// 이동목표위치

private: // 포탈정보
	std::vector<float4> LeftPortal_;		// 각 셀의 무게중심을 연결하는 벡터 기준 왼쪽의 정점목록
	std::vector<float4> RightPortal_;		// 각 셀의 무게중심을 연결하는 벡터 기준 오른쪽의 정점목록

private:
};

