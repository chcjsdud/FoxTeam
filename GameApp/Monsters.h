#pragma once
#include <GameEngine/GameEngineFSM.h>

#include "IUnit.h"
#include "MonsterStateInfo.h"

// �з� : ����
// �뵵 : �⺻(�θ�)Ŭ����
// ���� : ���� �ʿ� ��ġ�Ǵ� ���͵��� ��������
class GameEngineFBXRenderer;
class GameEngineCollision;
class NavFace;
class NavMesh;
class LumiaMap;
class Character;
class Monsters : public IUnit
{
public: // Static Value
	
public: // inline Get Function
	inline MonsterStateBasicType GetMonsterBasicStateType()
	{
		return CurStateBasicType_;
	}

	inline MonsterStateType GetMonsterDetailStateType()
	{
		return CurStateType_;
	}

	inline MonsterStateInfo GetMonsterStateInfo()
	{
		return StateInfo_;
	}

	inline Location GetMonsterAreaType()
	{
		return AreaType_;
	}

	inline MonsterType GetMonsterType()
	{
		return Type_;
	}

public: // inline Set Function
	inline void SetMonsterIndex(int _Index)
	{
		Index_ = _Index;
	}

	inline void SetMonsterAreaType(Location _AreaType)
	{
		AreaType_ = _AreaType;
	}

public:
	int GetIndex() override;
	void Damage(float _amount) override {}

public: // Initalize Function
	void InitalizeSpawnPosition(const float4& _SpawnPosition);				// ���ڷ� ������ _SpawnPosition�� �ش� ������ ������ġ�� ����(Ŭ���̾�Ʈ����)

public: // public ChangeAnimation Function
	void ChangeAnimationAndState(MonsterStateType _StateType);				// ���ͻ���Ÿ������ �ִϸ��̼� �� ���� ��ȯ

public:
protected: // Protected Pure Virtual Function
	virtual void InitalizeStateInfo() = 0;									// ���� �������� �ʱ�ȭ
	virtual void InitalizeResourceLoad() = 0;								// ���� ���ҽ� �ε�
	virtual void InitalizeRenderAndAnimation() = 0;							// ������ & �ִϸ��̼� ����
	virtual void InitalizeCollider() = 0;									// �浹ü ����

private: // Private Initalize Function
	void InitalizeFSMState();												// FSM ���� ����(����)

private: // Private Update Function
	void CheckMonsterStateInfo(float _DeltaTime);							// �������� üũ(���º� ����������ȯ üũ)
	void CheckAllCollision(float _DeltaTime);								// �浹ü �浹üũ

private: // FSM State Function
	//==================================== Main State
	void StartNormalState();												// �Ϲݻ��� ����
	void UpdateNormalState(float _DeltaTime);								// �Ϲݻ��� ����
	void EndNormalState();													// �Ϲݻ��� ����

	void StartCrowdControlState();											// ����������� ����
	void UpdateCrowdControlState(float _DeltaTime);							// ����������� ����
	void EndCrowdControlState();											// ����������� ����

	void StartAttackState();												// ���ݻ��� ����
	void UpdateAttackState(float _DeltaTime);								// ���ݻ��� ����
	void EndAttackState();													// ���ݻ��� ����

	//==================================== Normal State
	void StartAppearState();												// ù������� ����
	void UpdateAppearState(float _DeltaTime);								// ù������� ����
	void EndAppearState();													// ù������� ����

	void StartRegenState();													// �������� ����
	void UpdateRegenState(float _DeltaTime);								// �������� ����
	void EndRegenState();													// �������� ����

	void StartIdleState();													// ������ ����
	void UpdateIdleState(float _DeltaTime);									// ������ ����
	void EndIdleState();													// ������ ����

	void StartRunState();													// �̵����� ����
	void UpdateRunState(float _DeltaTime);									// �̵����� ����
	void EndRunState();														// �̵����� ����

	void StartHomingInstinctState();										// �ͼҺ��ɻ��� ����
	void UpdateHomingInstinctState(float _DeltaTime);						// �ͼҺ��ɻ��� ����
	void EndHomingInstinctState();											// �ͼҺ��ɻ��� ����

	void StartChaseState();													// Ÿ���������� ����
	void UpdateChaseState(float _DeltaTime);								// Ÿ���������� ����
	void EndChaseState();													// Ÿ���������� ����

	//==================================== CrowdControl State
	void StartHitState();													// �ǰݻ��� ����
	void UpdateHitState(float _DeltaTime);									// �ǰݻ��� ����
	void EndHitState();														// �ǰݻ��� ����

	void StartDeathState();													// ����߻��� ����
	void UpdateDeathState(float _DeltaTime);								// ����߻��� ����
	void EndDeathState();													// ����߻��� ����

	void StartDeadState();													// ���(�������)���� ����
	void UpdateDeadState(float _DeltaTime);									// ���(�������)���� ����
	void EndDeadState();													// ���(�������)���� ����

	//==================================== Attack State(���� : Ÿ�������� �Ǿ�������)
	void StartAttack01State();												// �Ϲݰ���1���� ����
	void UpdateAttack01State(float _DeltaTime);								// �Ϲݰ���1���� ����
	void EndAttack01State();												// �Ϲݰ���1���� ����

	void StartAttack02State();												// �Ϲݰ���2���� ����
	void UpdateAttack02State(float _DeltaTime);								// �Ϲݰ���2���� ����
	void EndAttack02State();												// �Ϲݰ���2���� ����

	void StartSkillAttackState();											// ��ų���ݻ��� ����
	void UpdateSkillAttackState(float _DeltaTime);							// ��ų���ݻ��� ����
	void EndSkillAttackState();												// ��ų���ݻ��� ����

private:
	void Start() override;
	void Update(float _deltaTime) override;

public:
	Monsters();
	virtual ~Monsters() = 0;												// �ش� Ŭ������ ��� ������ ����(�θ�) Ŭ������ 
																			// �ݵ�� ��ӹ޾Ƽ� ��üȭ�ؾ��ϱ⶧���� ��������Ҹ��ڸ� ���

protected:
	Monsters(const Monsters& _other) = delete;
	Monsters(Monsters&& _other) = delete;

private:
	Monsters& operator=(const Monsters& _other) = delete;
	Monsters& operator=(const Monsters&& _other) = delete;

public:
protected: // ������Ʈ
	GameEngineFBXRenderer* MainRenderer_;									// ���� ������
	GameEngineFBXRenderer* EffectRenderer_;									// ����Ʈ ������
	GameEngineCollision* BodyCollider_;										// ��ü �浹ü(���� -> ���� ���ʹ� �ش� �浹ü�� ���ݹڽ�, ��Ʈ�ڽ��� ��� ���)
	GameEngineCollision* AtkCollider_;										// ���� �浹ü(���� -> ���� �浹ü�� �и��Ǿ� �ʿ��� ���Ͱ� ����)
	GameEngineCollision* TargetCollider_;									// ���͸� �����Ͽ� Ÿ������ ������ ������ �浹ü

protected: // ��������(Ž����)
	int Index_;																// ���� �ε���
	MonsterType Type_;														// ������ Ÿ��
	Location AreaType_;														// ���� ���� ���� Ÿ��(StateInfo_.AppearAreaList_ �� �������� ���õ� ����)

protected: // �� �� ��ã��
	NavFace* CurrentNavFace_;												// ���� ������
	NavMesh* CurrentNavMesh_;												// �׺�޽�
	LumiaMap* CurrentMap_;													// ���� ���Ͱ� ��ġ�� ��
	
protected: // ��������(���ſ�)
	bool DetectTarget_;														// Ÿ�������Ϸ� Flag(true: Ÿ������) -> ���ݹ޾����� ����
	MonsterStateInfo StateInfo_;											// ������ ��������
	float4 Destination_;													// �̵�������

protected: // FSM State
	GameEngineFSM MainState_;												// ���λ���(�⺻: NormalState ����)
	GameEngineFSM NormalState_;												// �⺻����(CC, ATK �� ���ƿ��� ����)
	GameEngineFSM CrowdControlState_;										// �����������(�ܺ� Callback������ ����Ǵ� ����)
	GameEngineFSM AttackState_;												// ���ݻ���(�Ϲݰ���, ��ų���� ���¸� ���� �� ����)
	
protected: // Ÿ��
	Character* CurTarget_;													// ���� ������ Ÿ��(�ǰݽ� Ÿ������, ��ȯ����(�ͼҺ��� ��ġ�� 0.0f�϶�)���� �ʱ�ȭ)

private: // ��������
	MonsterStateBasicType CurStateBasicType_;								// ������ ������� �⺻Ÿ��(Normal, CrowdControl, Attack)
	MonsterStateType CurStateType_;											// ������ ������� ��Ÿ��(Run, Idle, Hit, ...)
};

