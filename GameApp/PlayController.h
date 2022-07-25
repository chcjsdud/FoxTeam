#pragma once
#include <GameEngine/GameEngineActor.h>

// *** ���⿡ �ռ�
// �ӽ÷� ���� ��Ʈ�ѷ� Ŭ�����Դϴ�.
// Play ���� ��� ���۰� ���͵��� ���¸� ������ �ֽ��ϴ�.

// ���� AI ���� �� ��Ʈ�ѷ����� ����Ī�� �����ϰ� �ϸ� ���� �� ��������

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
	
	// std::vector<Player*> players_;
	Player* testPlayer_;
	
private:
	void InitInput();
	void InitActor();

private:
	void CheckInput(float _DeltaTime);
	

private:
	void Start() override;
	void Update(float _DeltaTime) override;

};

