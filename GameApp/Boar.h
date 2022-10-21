#pragma once
#include "Monsters.h"

// �з� : ����Ʈ
// �뵵 : 
// ���� : �����(����)�� ��ų�������� ������ �������Ǵ� ���ݹ���
class GameEngineFBXRenderer;
class BoarHitRange : public GameEngineActor
{
public:
	static bool HitRangeMeshLoad;

public: // Public Inline Get Function
public: // Public Inline Set Function
	inline void SetHitRangeLength(float _HitRnageLength)
	{
		CurHitRnageLength_ = _HitRnageLength;
	}

public: // Public Initalize Related Function
	void InitalizeHitRnage();

public: // Public Function
	void SkillAttackRangeOn(float _Length, const float4& _Dir);
	void SkillAttackRangeOff();

protected:

private: // Private HitRange Length Calc Function
	void CalcHitRangeArrowCount(float _Length);
	void UpdateHitRangeArrow();

private: // Private Initalize Function
	void LoadHitRangeMesh();
	void CreateHitRangeRenderer();

public:
	BoarHitRange();
	~BoarHitRange();

protected:
	BoarHitRange(const BoarHitRange& _other) = delete;
	BoarHitRange(BoarHitRange&& _other) noexcept = delete;

private:
	BoarHitRange& operator=(const BoarHitRange& _other) = delete;
	BoarHitRange& operator=(const BoarHitRange&& _other) = delete;

public:
protected:
private:
	GameEngineFBXRenderer* Frame_;						// �����׵θ�
	GameEngineFBXRenderer* Bottom_;						// �����ٴ�
	std::vector<GameEngineFBXRenderer*> Arrows_;		// ����ȭ��ǥ����

private:
	float PrevHitRangeLength_;							// ������ ���ŵ� ��Ʈ�ڽ� ����
	int PrevArrowCount_;								// ������ ������ ȭ��ǥ����

private:
	float CurHitRnageLength_;							// ���� ���ŵ� ��Ʈ�ڽ� ����
	int CurArrowCount_;									// ���� ��Ʈ�ڽ� ���̿� ���� ȭ��ǥ ����
};

// �з� : ����
// �뵵 : 
// ���� : �����(����)�� ���� ����� ����
class GameEngineFBXRenderer;
class Boar : public Monsters
{
public:
	static bool ResourceLoadFlag;

protected:
	void InitalizeStateInfo() override;
	void InitalizeResourceLoad() override;
	void InitalizeRenderAndAnimation() override;
	void InitalizeCollider() override;

protected:
	void SkillAttackProcessing(float _DeltaTime) override;

private:
public:
	Boar();
	~Boar();

protected:
	Boar(const Boar& _other) = delete;
	Boar(Boar&& _other) noexcept = delete;

private:
	Boar& operator=(const Boar& _other) = delete;
	Boar& operator=(const Boar&& _other) = delete;

public:
protected:
private:
	bool SkillAtkReady_;								// ��ų�����غ� ó��
	bool SkillAtk_;										// ��ų���� ó��
	float SkillAtk_Range_;								// ��ų���� �����Ÿ�
	float SkillAtk_CastTimeMax_;						// ��ų���� �����ð� �ִ�ġ(����)
	float SkillAtk_CastTime_;							// ��ų���� �����ð�(����)
	float SkillAtk_FixedDamage_;						// ��ų���� ����������(����)
	float SkillAtk_RushSpeed_;							// ��ų���� �����ӵ�(����)

private:
	float4 SkillAtk_DetectTargetPos_;					// ���� ��ų�غ�� ������ Ÿ���� ��ġ
	float4 SkillAtk_DetectTargetDir_;					// ���� ��ų�غ�� ������ Ÿ���� ���� ����

private:
	BoarHitRange* SkillAtk_HitRnage_;					// ��Ʈ�ڽ� ������
};

