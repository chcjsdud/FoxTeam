#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

/*
�ϴ��� : �ÿ��̾� ���� ȭ���� ������ Ŭ����, 
		1. UI ǥ�ÿ� �÷��̾� ĳ���� ���ۿ� ���� ���
		2. ��� �÷��̾ ������ ����, ����Ī�ϸ� ��Ʈ�� �� ����� ������ �� ����
		3. �÷��̾��� Ű ���۰� ����� ������ �ִ� �÷��̾�� �ϴ��ϴ� �߰� ����
		4. ��� -> �÷��̾� ��Ʈ�ѷ� -> �÷��̾� 

�� �ֳ�? : �÷��̾ ����Ī�� ���� ������ �� �ְԲ� �����
			��� �÷��̾���� �̸� ������ ����, ���߿��� �����ϰ� ���� ����� �� �� �ִ�.

*/

class Player;
class GameMouse;
class PlayerController : public GameEngineActor
{
	friend Player;
	friend GameMouse;

public:
	PlayerController(); // default constructer ����Ʈ ������
	~PlayerController(); // default destructer ����Ʈ �Ҹ���

private:
	GameMouse* testGameMouse_;

	GameEngineFSM CameraState_;

	std::map<std::string, Player*> PlayerMap_;
	Player* MainPlayer_;

	// �ӽ� bool �ڵ� 
#pragma region ���� ����
	bool Key_RB_;
	bool Key_LB_;
	bool Key_Stop_;

	GameEngineActor* TargetActor_;
	bool Stopkey_;
	bool isClick_;
	
	bool IsAttack_;

	//�ӽ�, ���콺�� Ŭ���� ��ǥ, ���콺 Ŭ���� �� 1�� ������, ���߿� �� ��ǥ�� �� ��ǥ�� �������� �ʱ�ȭ
	float4 TempMousePos_; // �ʰ� �浹�� ���콺�� ��ǥ�� �����;���, ������ UI Ŭ���� ���콺 ��ǥ�� �ʿ���

	//�ӽ� ���콺 Ŭ�� ���� , ���콺 Ŭ���� �� 1�� ������
	float4 TmepMouseDir_;

	//�÷��̾� ������ ������ ���� �̸� ������ �� �ʿ䰡 �ִ�.
	/*
	1. ��Ŭ�� �̵�
	2. ��Ŭ�� ����
	3. SŰ�� ������ ĵ��

	2. ��Ŭ�� ��Ŭ��
	3. ��Ŭ�� ����
	4. AŰ, ���� (�ֳ�?)	
	*/

#pragma endregion



private:
	void InitInput();
	void UIInit();

	void MouseUpdate();
	void PlayerStateUpdate();
	void KeyStateUpdate();
	void CameraUpdate(float _DeltaTime);

private:

	// ���ͳ� ���Ͽ� �߰� : ���ͺ� ����
	void CameraUpdate_EternalReturn(float _DeltaTime);

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	void SwitchMainPlayer(std::string _Name);

	void ChangePlayerState(std::string _State)
	{
		if (MainPlayer_->Unit_GetStateName() != _State)
		{
			MainPlayer_->Unit_ChangeState(_State);
		}
	}

	void AddPlayerMap(std::string _Name, Player* _Player)
	{
		PlayerMap_.insert(std::make_pair(_Name, _Player));

		if (MainPlayer_ == nullptr)
		{
			MainPlayer_ = _Player;
		}
	}

	//�̸��� ���� �������
	void AddPlayerMap(Player* _Player)
	{
		if (_Player->GetName() == "")
		{
			GameEngineDebug::MsgBoxError("ĳ���� �̸��� �������� �ʾҽ��ϴ�.");
		}

		PlayerMap_.insert(std::make_pair(_Player->GetName(), _Player));

		if (MainPlayer_ == nullptr)
		{
			MainPlayer_ = _Player;
		}
	}

	void ErasePlayerMap(std::string _Name)
	{
		PlayerMap_.erase(PlayerMap_.find(_Name));
	}

	void SetMainActor(Player* _Actor)
	{
		MainPlayer_ = _Actor;
	}

	void SetMainActor(std::string _Name)
	{
		MainPlayer_ = PlayerMap_.find(_Name)->second;
	}

private:
	PlayerController(const PlayerController& _other) = delete;
	PlayerController(PlayerController&& _other) = delete;
	PlayerController& operator=(const PlayerController& _other) = delete;
	PlayerController& operator=(const PlayerController&& _other) = delete;
};

