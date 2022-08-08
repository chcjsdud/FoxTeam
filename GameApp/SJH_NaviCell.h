#pragma once

// 분류 : 삼각형
// 용도 : 
// 설명 : 메쉬를 이루는 삼각형들을 집합중 한개
class SJH_NaviCell
{
public:
protected:
private:

public:
	SJH_NaviCell();
	~SJH_NaviCell();

protected:
	SJH_NaviCell(const SJH_NaviCell& _other) = delete;
	SJH_NaviCell(SJH_NaviCell&& _other) noexcept = delete;

private:
	SJH_NaviCell& operator=(const SJH_NaviCell& _other) = delete;
	SJH_NaviCell& operator=(const SJH_NaviCell&& _other) = delete;

public:
protected:
private:
	std::vector<int> AdjacentTriangles_;				// 해당 면(삼각형)과 인접한 삼각형의 인덱스 목록
	float4 CenterOfGravity_;							// 해당 면(삼각형)의 무게중심
	std::vector<float4> VertexList_;					// 해당 면을 구성하는 정점목록(3개)

private:
};

