#pragma once
//#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>


// AI�� �����ϴ� ��Ʈ�ѷ�,
// ����AI ���ָ��� ������ �� �ְ� ���� ����
//

// �̰� �ʿ��Ѱ�? 

class Unit;
class AIController //: public GameEngineActor
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

	GameEngineFSM AIState_;

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

public:
	void Update(float _DeltaTime);

	void AIController_Off();
	void AIController_On();

private:

	void Idle_Start();
	void Idle_Update(float _DeltaTime);
	void Idle_End();

	void Chase_Start();
	void Chase_Update(float _DeltaTime);
	void Chase_End();

	void Attack_Start();
	void Attack_Update(float _DeltaTime);
	void Attack_End();


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

