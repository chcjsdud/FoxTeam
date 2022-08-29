#pragma once
#include <GameEngine/GameEngineLevel.h>
#include <GameEngine/GameEngineFSM.h>

#include <GameEngine/GameEngineSocketServer.h>
#include <GameEngine/GameEngineSocketClient.h>

#include "LobbyPlayerInfo.h"

class LobbyLevel : public GameEngineLevel
{
public:
	LobbyLevel(); 
	~LobbyLevel();
	LobbyLevel(const LobbyLevel& _other) = delete;
	LobbyLevel(LobbyLevel&& _other) noexcept = delete;
	LobbyLevel& operator=(const LobbyLevel& _other) = delete;
	LobbyLevel& operator=(const LobbyLevel&& _other) = delete;

public:
	// 스테이트를 나눠 사용하는 게 옳을 것
	// Idle :: 아무 입력이 들어가지 않은 초기 스테이트
	// CharSelect :: 캐릭터 셀렉트와 시작 지점 설정을 위한 스테이트?
	// (플레이어 정보... 캐릭터 선택 시작 위치 등을 담을 수 있는 패킷을 준비해야 한다.)
	// LoginPacket? CharSelectPacket? 이름을 무엇으로?

	
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

protected:
	void StartIdle();
	void UpdateIdle(float _DeltaTime);
	void EndIdle();
	void StartSelect();
	void UpdateSelect(float _DeltaTime);
	void EndSelect();
	void StartJoin();
	void UpdateJoin(float _DeltaTime);
	void EndJoin();

private:
	GameEngineFSM state_;
public:
	std::vector<LobbyPlayerInfo> playerList_;
	static GameEngineSocketServer serverSocket_;
	static GameEngineSocketClient clientSocket_;
};

