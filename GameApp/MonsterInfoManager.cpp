#include "PreCompile.h"
#include "MonsterInfoManager.h"

MonsterInfoManager* MonsterInfoManager::GetInstance()
{
	static MonsterInfoManager instance;
	return &instance;
}

void MonsterInfoManager::AddMonsterInfo(MonsterInfo _MonsterInfo)
{
	AllMonsters_.push_back(_MonsterInfo);
}

void MonsterInfoManager::SetMonsterInfos(std::vector<MonsterInfo> _MonsterInfos)
{
	AllMonsters_ = _MonsterInfos;
}

MonsterInfoManager::MonsterInfoManager()
{
}

MonsterInfoManager::~MonsterInfoManager()
{
}
