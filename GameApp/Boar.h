#pragma once
#include "Monsters.h"

// �з� : ����
// �뵵 : 
// ���� : �����(����)�� ���� ����� ����
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
	bool SkillAtk_;										// ��ų���� ó��
	float SkillAtk_Range_;								// ��ų���� �����Ÿ�
	float SkillAtk_CastTimeMax_;						// ��ų���� �����ð� �ִ�ġ(����)
	float SkillAtk_CastTime_;							// ��ų���� �����ð�(����)
	float SkillAtk_FixedDamage_;						// ��ų���� ����������(����)

	
};

