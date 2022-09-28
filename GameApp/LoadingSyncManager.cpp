#include "PreCompile.h"
#include "LoadingSyncManager.h"

LoadingSyncManager* LoadingSyncManager::GetInstance()
{
	static LoadingSyncManager instance;
	return &instance;
}

LoadingSyncManager::LoadingSyncManager()
	: LoadingInfo_{}
{
}

LoadingSyncManager::~LoadingSyncManager()
{
}
