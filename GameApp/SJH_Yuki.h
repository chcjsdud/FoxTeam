#pragma once
#include <GameEngine/GameEngineActor.h>

enum class Yuki_State
{
	COM_ARRIVE,
	COM_BOXOPEN,
	COM_COLLECT,
	COM_COLLECT_STONE,
	COM_COLLECT_WATER,
	COM_COLLECT_WOOD,
	COM_CRAFTFOOD,
	COM_CRAFTMETAL,
	COM_DANCE,
	COM_DEATH,
	COM_DOWN_DEAD,
	COM_DOWN_RUN,
	COM_DOWN_START,
	COM_DOWN_WAIT,
	COM_DOWNDEAD,
	COM_FISHING,
	COM_INSTALL_TRAP,
	COM_OPERATE,
	COM_REST_END,
	COM_REST_LOOP,
	COM_REST_START,
	COM_RESURRECT,
	COM_RUN,
	COM_SKILL04,
	COM_SKILL04_END,
	COM_WAIT,
	MAX
};

// �з� : 
// �뵵 : 
// ���� : 
class GameEngineFBXMesh;
class GameEngineFBXRenderer;
class SJH_NaviCell;
class SJH_Yuki : public GameEngineActor
{
public: // ������ġ ����
	void Initialize(SJH_NaviCell* _CurNaviCell, const float4& _InitPos);

public: // �̵�����
	void MoveStart(SJH_NaviCell* _TargetNaviCell, const float4& _MoveTargetPos);

public: // ��Ÿ�׼�
	void Detect(const GameEngineActor* _DetectTarget);
	void Attack(const GameEngineActor* _AttackTarget);
	void SkillAttack(const GameEngineActor* _AttackTarget);
	void Block(const GameEngineActor* _BlockTarget);
	void GetHit(const GameEngineActor* _GetHitTarget);

protected:

private:
	void InputKeyStateCheck(float _DeltaTime);

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	SJH_Yuki();
	~SJH_Yuki();

protected:
	SJH_Yuki(const SJH_Yuki& _other) = delete;
	SJH_Yuki(SJH_Yuki&& _other) noexcept = delete;

private:
	SJH_Yuki& operator=(const SJH_Yuki& _other) = delete;
	SJH_Yuki& operator=(const SJH_Yuki&& _other) = delete;

public:
	static SJH_Yuki* MainPlayer;

protected:

private:
	GameEngineFBXMesh* BaseMesh_;
	GameEngineFBXRenderer* AnimRenderer_;

#pragma region �ִϸ��̼� ��������
private:
	std::vector<std::string> AnimNameList_;	// ���� ĳ���Ͱ� ����ϴ� �ִϸ��̼� �̸� ���
	int CurAnimationIndex_;					// ���� ĳ���� �ִϸ��̼� �ε���

private:
	Yuki_State CurState_;					// ���� ĳ���� ����

private:
#pragma endregion

#pragma region �̵�����
private:
	SJH_NaviCell* CurNaviCell_;				// ���� �÷��̾ ��ġ�ϰ� �ִ� �ﰢ��
	SJH_NaviCell* TargetNaviCell_;			// ���� �÷��̾ �̵��Ϸ��� ������ǥ������ ��ġ�� �ﰢ��
	bool MoveStart_;						// �̵���� ���� �Ϸ�� On

private:
	std::list<float4> MovePath_;			// �̵����

private:
	float4 MoveStartPos_;					// �̵������ ���� ��ġ
	float4 MoveEndPos_;						// �̵������ ��ǥ ��ġ
	float MoveSpeed_;						// �̵��ӵ�
#pragma endregion
};

