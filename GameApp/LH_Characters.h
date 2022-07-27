#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

#include "Enums.h"
#include "Status.h"

struct SkillInfo
{
	// skilltype(�нú�, ����, ...)
	// �Ҹ��ڿ�(sp)
	// ��Ÿ��
	// ��Ÿ�
	// ȿ��
};

// �з� : 
// �뵵 : ĳ���� �ֻ��� Ŭ����
// ���� : ��� ĳ������ Ŀ�ǵ� ���� �� �⺻������ ���� 07.26 ���� : ���� ����
// �÷��̾�� ĳ���ʹ� Player.h

class Characters : public GameEngineActor
{
public:
	Characters();
	~Characters();

protected:		// delete constructer
	Characters(const Characters& _other) = delete;
	Characters(Characters&& _other) = delete;
	Characters& operator=(const Characters& _other) = delete;
	Characters& operator=(const Characters&& _other) = delete;

private: // ��������
	CharacterType CharacterType_;		// (0: AI, 1: MAIN) : default(AI)

protected: // �⺻����
	GameEngineFSM State_;
	//���� ����Ʈ, 
	std::map <std::string, Status*> BufferList_;
	// ĳ���� �⺻ ����
	Status Status_Base_;
	// ĳ���� �߰� ���� / ��
	Status Status_Add_;
	// ĳ���� �߰� ���� / ��
	Status Status_Mult_;
	// ĳ���� ���� ����
	Status Status_Final_;

	//Ű ���� 07.26, �������� ���� ���������� ����, 
	float4 KeyDir_;

	//���µ� ����� ����, ������ ���⺸�� �ٶ������ ���⿡ �����
	float4 TargetDir_;

	//�ٶ���� �� ����
	float4 FowordDir_;

	//���� �ٶ󺸴� ����
	float4 CurFowordDir_;

	//Y�� ȸ�� �ӵ�
	float YRotateSpeed_;

	bool IsMove_;

	//���� ��
	float AttackTurm_;
	//���� ���ϸ��̼� �ܰ�
	int AttackLevel_;
	//���� �ð�, �� �ð� �̳��� ����Ű�� ������ ���� �ܰ� ������ ��
	float AttackTime_;
	//Attack Hit Box Ȱ��ȭ �ð�
	float AttackHitTime_;

	GameEngineActor* Target_;


private: // ���� �����÷��̾� ����
	static Characters* MainPlayer;
	 //�ٸ� ������ Ŭ������ ��� �ִ°� ������ ����


public:
	static Characters* GetMainPlayer();
	static void SetMainPlayer(Characters* _Player);

private: // ��ų����
	SkillInfo SkillInfo_[6];			// ��ų(�нú�, Q, W, E, R, D)
										// �нú�, Q~R ���� ��ų
										// D ��ų�� ������ ��
private: // Ŀ�ǵ� ����
// 1. ����ĳ���� �þ� �ۿ����� ����(�̵�-�Ĺ� ����)
// 2. ����ĳ���� �þ� ������������ ����(�̵�-���� or ����-ȸ��)

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

protected: 
	// ĳ���� State, �����Լ��� ���� ĳ���� Ŭ������ �⺻������ ������ �־�� �� state�� ����
	virtual void Idle_Start() = 0;
	virtual void Idle_Update(float _DeltaTime) = 0;
	virtual void Idle_End() = 0;

	virtual void Walk_Start() = 0;
	virtual void Walk_Update(float _DeltaTime) = 0;
	virtual void Walk_End() = 0;

	virtual void Run_Start() = 0;
	virtual void Run_Update(float _DeltaTime) = 0;
	virtual void Run_End() = 0;

	virtual void Attack_Start() = 0;
	virtual void Attack_Update(float _DeltaTime) = 0;
	virtual void Attack_End() = 0;

	// ���ݿ� ��ȭ�� ���涧���� ������
protected: 

	void StateInit();
	void SyncStatus();


public:
	inline void SetCharacterType(CharacterType _Type)
	{
		// �߸��� Ÿ�� ���Ž� ����
		if (_Type == CharacterType::NONE || _Type == CharacterType::MAX)
		{
			return;
		}

		CharacterType_ = _Type;
	}

	std::map < std::string, Status*> PlayerGetBuffList()
	{
		return BufferList_;
	}

	const Status PlayerGetBaseStat()
	{
		return Status_Base_;
	}

	const Status PlayerGetAddStat()
	{
		return Status_Add_;
	}

	const Status PlayerGetMultStat()
	{
		return Status_Mult_;
	}

	const Status PlayerGetFinalStat()
	{
		return Status_Final_;
	}

	const int PlayerGetCurHP()
	{
		return Status_Final_.Stat_Health_;
	}

	const float PlayerGetCurStamina()
	{
		return Status_Final_.Stat_Stamina_;
	}

	GameEngineActor* PlayerGetTarget()
	{
		return Target_;
	}

	const int PlayerGetAttackPower()
	{
		return Status_Final_.Stat_AttackPower_;
	}

	void SetTarget(GameEngineActor* _Target)
	{
		Target_ = _Target;
	}

protected: // �� ĳ���� ���� ���� �� ��ų����
	//virtual void Initialize() = 0;

protected: // �� ĳ���� Ŀ�ǵ� �ο�


};
