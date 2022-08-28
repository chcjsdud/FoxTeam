#pragma once
#include <GameEngine/GameEngineLevel.h>
#include <GameEngine/GameEngineFSM.h>

#include <GameEngine/GameEngineSocketServer.h>
#include <GameEngine/GameEngineSocketClient.h>

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
	// ������Ʈ�� ���� ����ϴ� �� ���� ��
	// Idle :: �ƹ� �Է��� ���� ���� �ʱ� ������Ʈ
	// CharSelect :: ĳ���� ����Ʈ�� ���� ���� ������ ���� ������Ʈ?
	// (�÷��̾� ����... ĳ���� ���� ���� ��ġ ���� ���� �� �ִ� ��Ŷ�� �غ��ؾ� �Ѵ�.)
	// LoginPacket? CharSelectPacket? �̸��� ��������?

	
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

protected:


private:
	GameEngineFSM* state_;

	GameEngineSocketServer serverSocket_;
	GameEngineSocketClient clientSocket_;

};

