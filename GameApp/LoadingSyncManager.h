#pragma once
#include "LoadingSyncInfo.h"

// �з� : Manager
// �뵵 : 
// ���� : ���� ���ӿ� �ʿ��� ���� �ε����¸� �����ϴ� ������
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

