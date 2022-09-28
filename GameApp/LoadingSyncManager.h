#pragma once
#include "LoadingSyncInfo.h"

// 분류 : Manager
// 용도 : 
// 설명 : 현재 게임에 필요한 정보 로딩상태를 관리하는 관리자
class LoadingSyncManager
{
public:
	static LoadingSyncManager* GetInstance();

public: // Refernce Get Function
	inline LoadingSyncInfo& GetCurrentLoadingInfo()
	{
		return LoadingInfo_;
	}

public:
	LoadingSyncManager();
	~LoadingSyncManager();

protected:
	LoadingSyncManager(const LoadingSyncManager& _other) = delete;
	LoadingSyncManager(LoadingSyncManager&& _other) noexcept = delete;

private:
	LoadingSyncManager& operator=(const LoadingSyncManager& _other) = delete;
	LoadingSyncManager& operator=(const LoadingSyncManager&& _other) = delete;

public:
protected:
private:
	LoadingSyncInfo LoadingInfo_;
};

