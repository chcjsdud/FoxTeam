#pragma once
#include <GameEngine/GameEngineLevel.h>
#include <GameEngine/GameEngineFSM.h>
// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineSocketInterface;
class GameEngineSocketServer;
class GameEngineSocketClient;
class ServerTestLevel : public GameEngineLevel
{
public:
	ServerTestLevel(); // default constructer 디폴트 생성자
	~ServerTestLevel(); // default destructer 디폴트 소멸자
	ServerTestLevel(const ServerTestLevel& _other) = delete; // default Copy constructer 디폴트 복사생성자
	ServerTestLevel(ServerTestLevel&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	ServerTestLevel& operator=(const ServerTestLevel& _other) = delete; // default Copy operator 디폴트 대입 연산자
	ServerTestLevel& operator=(const ServerTestLevel&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;
protected:


private:
	// *** 서버 소켓과 클라이언트 소켓 둘 중 하나만 생성해서 사용해야 합니다.
	static GameEngineSocketServer* server_;
	GameEngineSocketClient* client_;
	bool bIsServer_;
};

