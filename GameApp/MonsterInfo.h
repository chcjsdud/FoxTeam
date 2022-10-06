#pragma once
#include "Enums.h"

struct MonsterInfo
{
public: // 생성정보
	int Index_;										// 몬스터 생성 인덱스(탐색용)
	Location RegionType_;							// 몬스터 스폰 지역 타입(탐색용)
	MonsterType MonsterType_;						// 몬스터 타입
	int IsGroup_;									// 그룹생성여부(1: 그룹으로생성, 0: 단독생성)
	int GroupCount_;								// 그룹생성일때 같은위치에 생성해야하는 야생동물수

public: // 초기화정보
	float4 SpawnPosition_;							// 스폰 위치
};
