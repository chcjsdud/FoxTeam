#pragma once
#include "SJH_NaviCell.h"

// �з� : 
// �뵵 : A*�� �� ����� ������ ������θ� ����
// ���� : A*�� �� ���(������)�� ���Ź޾� �������� ������θ� ����Ͽ� ����
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

private: // ��������
	float4 StartPos_;
	float4 EndPos_;

private: // ��Ż����
	std::vector<float4> LeftPortal_;
	std::vector<float4> RightPortal_;

private:
};

