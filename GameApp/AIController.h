#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>


// AI�� �����ϴ� ��Ʈ�ѷ�,
// ����AI ���ָ��� ������ �� �ְ� ���� ����
//

class Unit;
class AIController : public GameEngineActor
{
public:
	AIController(); 
	~AIController();

private:
	Unit* MainUnit_;

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
	//void InitActor();
	void UIInit();

	//void PlayerStateUpdate();
	void KeyStateUpdate();

private:
	void CameraUpdate_EternalReturn(float _DeltaTime);

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	void ChangePlayerState(std::string _State);

	void SetMainActor(Unit* _MainUnit)
	{
		MainUnit_ = _MainUnit;
	}

private:
	AIController(const AIController& _other) = delete;
	AIController(AIController&& _other) = delete;
	AIController& operator=(const AIController& _other) = delete;
	AIController& operator=(const AIController&& _other) = delete;
};

