#pragma once
#include "MonsterInfo.h"

// 분류 : Manager
// 용도 : 
// 설명 : 현재 맵에 배치된 모든 몬스터의 정보를 관리하는 관리자
class MonsterInfoManager
{
public:
	static MonsterInfoManager* GetInstance();

public:
	inline int GetCurMonsterListSize()
	{
		return static_cast<int>(AllMonsters_.size());
	}

	inline std::vector<MonsterInfo>& GetAllMonsterListRef()
	{
		return AllMonsters_;
	}

	inline std::vector<MonsterInfo> GetAllMonsterListValue()
	{
		return AllMonsters_;
	}

public:
	void AddMonsterInfo(MonsterInfo _MonsterInfo);
	void SetMonsterInfos(std::vector<MonsterInfo> _MonsterInfos);

protected:
private:

public:
	MonsterInfoManager();
	~MonsterInfoManager();

protected:
	MonsterInfoManager(const MonsterInfoManager& _other) = delete;
	MonsterInfoManager(MonsterInfoManager&& _other) noexcept = delete;

private:
	MonsterInfoManager& operator=(const MonsterInfoManager& _other) = delete;
	MonsterInfoManager& operator=(const MonsterInfoManager&& _other) = delete;

public:
protected:
private:
	std::vector<MonsterInfo> AllMonsters_;						// 현재 맵에 배치된 모든 몬스터의 정보
};

