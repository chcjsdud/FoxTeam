#pragma once
#include "GameEngineObjectNameBase.h"
#include <thread>
#include <string>
#include <functional>

// 분류 : Thread
// 용도 : 작업 단위
// 설명 : Name(스레드 설명)을 포함한 스레드 할당
class GameEngineThread : public GameEngineObjectNameBase
{
public:
	static void GameServerThreadFunction(GameEngineThread* _Thread, std::string _Name);

public:
	void Start(std::string _ThreadName, std::function<void(GameEngineThread*)> _Function)
	{
		ThreadFunction_ = _Function;
		Thread_ = std::thread(GameServerThreadFunction, this, _ThreadName);
	}

	void Join()
	{
		if (nullptr != ThreadFunction_)
		{
			Sleep(1);
			Thread_.join();
			ThreadFunction_ = nullptr;
		}

	}

protected:
private:

public:
	GameEngineThread();
	~GameEngineThread();

public:
	GameEngineThread(const GameEngineThread& _Other) = delete;
	GameEngineThread(GameEngineThread&& _Other) noexcept = delete;

protected:
	GameEngineThread& operator=(const GameEngineThread& _Other) = delete;
	GameEngineThread& operator=(GameEngineThread&& _Other) noexcept = delete;

public:
protected:
private:
	std::thread Thread_;
	std::function<void(GameEngineThread*)> ThreadFunction_;
};

