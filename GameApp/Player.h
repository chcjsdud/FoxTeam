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


	void AttackStateInit()
	{
		// �� �ð� �ȿ� �߰� ���� �ϸ� ���ӱⰡ ����, ���ݽ� ����
		AttackTime_ = 0.75f;

		//���ӱ� �ܰ�, 3�ܰ� ������ �ٽ� 0����
		AttackLevel_ = 0;

		//0.5�� �������� �����Ѵ�. ���ݽ� ����
		AttackTurm_ = 0.5f;

		//0.1�� ���ȸ� Ÿ�� ������ �ִ�. ���ݽ� ����
		AttackHitTime_ = 0.1f;	
	}

#pragma endregion

#pragma region MemberUpdateFunc
private:
	//Member Func
	void DEBUGUpdate(float _DeltaTime);

	// update�� �Լ��� ���� �� ī�޶� ������Ʈ, �ٷ� �ڿ��� �ٶ󺸴� �Լ�
	void CameraUpdate_BackPosition(float _DeltaTime);
	void CameraUpdate_UpPosition(float _DeltaTime);

	void KeyDirUpdate(float _DeltaTime);
	// update�� �Լ��� ���� �� �̵��� ������
	void MoveUpdate(float _DeltaTime);

	//ĳ���Ͱ� �ٶ󺸴� ������ �ٷ� �ٷ� �Ⱥ��ϰ� õõ�� ����(�̵����� �����ϰ�), 
	//07.18 �ٶ󺸴� ������ �־������
	void MoveRotateUpdate(float _DeltaTime);

	//RockOn ���϶� ���� ������Ʈ
	void RockOnDirUpdate(float _DeltaTime);

	void CurDirUpdate(float _DeltaTime);

	void RockOnUpdate(float _DeltaTime);
	//void ChangeCamFunc(void (Player::* _Camfunc)(float))
	//{
	//	CamFunc_ = std::bind(_Camfunc, this, std::placeholders::_1);
	//}
#pragma endregion

#pragma region MemberFunc
private:

	void SetTarget(GameEngineActor* _Target)
	{
		Target_ = _Target;
	}

	const float4 CalculateTargetDir(GameEngineTransform* TargetTransform)
	{
		float4 Dir = TargetTransform->GetWorldPosition()- GetTransform()->GetWorldPosition();
		Dir.Normalize3D();

		return Dir;
	}

#pragma endregion
private:
	//state
	GameEngineFSM PlayerState_;

	GameEngineFSM CameraState_;

	//component
	// 
	//�ٴ� �ݸ���
	GameEngineCollision* PlayerGroundCollision_;
	// RockOn �浹ü
	GameEngineCollision* PlayerRockOnCollision_;
	//�ǰ� ��Ʈ�ڽ�
	GameEngineCollision* PlayerHitBoxCollision_;
	//���� ��Ʈ�ڽ�
	GameEngineCollision* PlayerAttackHitBoxCollision_;

	GameEngineFBXRenderer* FBXRenderer_;



	GameEngineImageRenderer* RockOnImageRenderer_;
	GameEngineActor* Target_;

	//GameEngineFBXMesh* mesh_; //�ʿ��Ѱ�?

	//Ű ����
	float4 KeyDir_;
	//���µ� ����� ����
	float4 TargetDir_;
	//�ٶ���� �� ����
	float4 FowordDir_;
	//���� �ٶ󺸴� ����
	float4 CurFowordDir_;
	//Y�� ȸ�� �ӵ�
	float YRotateSpeed_;

	bool IsMove_;
	bool IsRockon_;

	float Speed_;

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

	//Status
	int Hp_;
	float Stamina_;

public:
	//UI
	class TopUI* TopUI_;

public:
	const int PlayerGetHP()
	{
		return Hp_;
	}

	void PlayerSetHP(int _Hp)
	{
		Hp_ = _Hp;
	}

	const float PlayerGetStamina()
	{
		return Stamina_;
	}

	void PlayerSetStamina(float _Stamina)
	{
		Stamina_ = _Stamina;
	}
};

