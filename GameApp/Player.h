#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
/// <summary>
/// ���� ���ΰ� ����
/// </summary>
// ���� :
class GameEngineLevel;
class GameEngineImageRenderer;
class Player : public GameEngineActor
{
public:
	Player();
	~Player();
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

private:
	virtual void Start();
	virtual void Update(float _DeltaTime);

#pragma region Init
private:
	//Init

	// ������, �ݸ���, ��� Componenet �ʱ�ȭ
	void ComponenetInit();
	//State �ʱ�ȭ
	void StateInit();
	void KeyInit();
#pragma endregion

#pragma region State
private:
	void Idle_Start();
	void Idle_Update(float _DeltaTime);
	void Idle_End();

	void Walk_Start();
	void Walk_Update(float _DeltaTime);
	void Walk_End();

	void Run_Start();
	void Run_Update(float _DeltaTime);
	void Run_End();

	void Attack_Start();
	void Attack_Update(float _DeltaTime);
	void Attack_End();
#pragma endregion

private:
	//Member Func
	void Attack();

	// update�� �Լ��� ���� �� ī�޶� ������Ʈ, �ٷ� �ڿ��� �ٶ󺸴� �Լ�
	void CameraUpdate_BackPosition(float _DeltaTime);
	void CameraUpdate_UpPosition(float _DeltaTime);

	// update�� �Լ��� ���� �� �̵��� ������
	void MoveUpdate(float _DeltaTime);

	//ĳ���Ͱ� �ٶ󺸴� ������ �ٷ� �ٷ� �Ⱥ��ϰ� õõ�� ����(�̵����� �����ϰ�)
	void MoveRotateUpdate(float _DeltaTime);

	//void ChangeCamFunc(void (Player::* _Camfunc)(float))
	//{
	//	CamFunc_ = std::bind(_Camfunc, this, std::placeholders::_1);
	//}
private:
	//state
	GameEngineFSM PlayerState_;

	GameEngineFSM CameraState_;

	//component
	// 
	//�ٴ� �ݸ���
	GameEngineCollision* PlayerGroundCollision_;
	//�ǰ� ��Ʈ�ڽ�
	GameEngineCollision* PlayerHitBoxCollision_;
	//���� ��Ʈ�ڽ�
	GameEngineCollision* PlayerAttackHitBoxCollision_;

	GameEngineFBXRenderer* FBXRenderer_;

	//GameEngineFBXMesh* mesh_; //�ʿ��Ѱ�?

	float Speed_;

	float4 KeyDir_;
	float4 CurFowordDir_;

	//Y�� ȸ�� �ӵ�
	float YRotateSpeed_;

	//std::function<void(float)> CamFunc_;

	//������ ��Ÿ��
	float RollTurmSecond_;
	//������ �ð�(����Ÿ��)
	float RollSecond_;

	//���� ��
	float AttackTurm_; 
	//���� ���ϸ��̼� �ܰ�
	int AttackLevel_;
	//���� �ð�, �� �ð� �̳��� ����Ű�� ������ ���� �ܰ� ������ ��
	float AttackTime_;
	float AttackHitTime_;

	int Hp_;
	float Stamina_;

public:
	class TopUI* TopUI_;

public:
	const int GetHP()
	{
		return Hp_;
	}

	void SetHP(int _Hp)
	{
		Hp_ = _Hp;
	}

	const float GetStamina()
	{
		return Stamina_;
	}

	void SetStamina(float _Stamina)
	{
		Stamina_ = _Stamina;
	}

private:

	//�ӽ� ������
	float UnitVectorToDegree(float x, float y)
	{
		float result = 0.f;

		if (y >= 0)
		{
				result = acosf(x) * GameEngineMath::RadianToDegree;
		}

		if (y < 0)
		{
				result = (acosf(-x) * GameEngineMath::RadianToDegree) +180.f;
		}

		return result;
	}
};

