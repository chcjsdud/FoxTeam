#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

// *** ���⿡ �ռ�
// �ӽ÷� ���� ��Ʈ�ѷ� Ŭ�����Դϴ�.
// Play ���� ��� ���۰� ���͵��� ���¸� ������ �ֽ��ϴ�.

// ���� AI ���� �� ��Ʈ�ѷ����� ����Ī�� �����ϰ� �ϸ� ���� �� ��������



// �÷��̾��� "���� ȭ��"�� �����ϰ� �� Ŭ������ �缳����
// ���� ������ '�÷��̾�' Ŭ������ �����ϰ�, ȭ�� UI�� KeyInput ���� �����Ұ�




class Player;
class GameMouse;
class PlayController : public GameEngineActor
{
	friend Player;
	friend GameMouse;

public:
	PlayController(); // default constructer ����Ʈ ������
	~PlayController(); // default destructer ����Ʈ �Ҹ���
	PlayController(const PlayController& _other) = delete; // default Copy constructer ����Ʈ ���������
	PlayController(PlayController&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	PlayController& operator=(const PlayController& _other) = delete; // default Copy operator ����Ʈ ���� ������
	PlayController& operator=(const PlayController&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

private:
	GameMouse* testGameMouse_;

	GameEngineFSM CameraState_;

	class Player* MainActor_;
	
	// std::vector<Player*> players_;
	//Player* testPlayer_;
	
private:
	void InitInput();
	void InitActor();

private:
	// update�� �Լ��� ���� �� ī�޶� ������Ʈ, �ٷ� �ڿ��� �ٶ󺸴� �Լ�
	void CameraUpdate_BackPosition(float _DeltaTime);
	void CameraUpdate_UpPosition(float _DeltaTime);

	// ���ͳ� ���Ͽ� �߰� : ���ͺ� ����
	void CameraUpdate_EternalReturn(float _DeltaTime);

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	void SetMainActor(Player* _Actor)
	{
		MainActor_ = _Actor;
	}
};

