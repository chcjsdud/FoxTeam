#pragma once
#include "Enums.h"

struct LoadingSyncInfo
{
public:
	LoadingSyncInfo()
		: MapLoadEnd_(0)
		, MonsterLoadEnd_(0)
		, CharacterLoadEnd_(0)
	{
	}

	~LoadingSyncInfo()
	{
	}

public: // 로딩완료 Flag 정보
	int MapLoadEnd_;					// 현재 게임에 지정된 맵 로드완료(1: 로딩완료)
	int MonsterLoadEnd_;				// 현재 맵에 배치되는 모든 몬스터 로드완료(1: 로딩완료)
	int CharacterLoadEnd_;				// 현재 게임에 진입한 플레이어 로드완료(1: 로딩완료)
};
