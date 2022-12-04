#include "PreCompile.h"
#include "GameEngineThread.h"
#include <Windows.h>
#include "GameEngineString.h"

void GameEngineThread::GameServerThreadFunction(GameEngineThread* _Thread, std::string _Name)
{
	std::wstring WName;
	GameEngineString::AnsiToUnicode(_Name, WName);
	SetThreadDescription(GetCurrentThread(), WName.c_str());
	_Thread->ThreadFunction_(_Thread);
}

GameEngineThread::GameEngineThread() 
{
}

GameEngineThread::~GameEngineThread() 
{
	if (nullptr != ThreadFunction_)
	{
		Join();
	}
}
