#pragma once
#include "Monsters.h"

// �з� : ����
// �뵵 : 
// ���� : ��(����)�� ���� ����� ����
class Chicken : public Monsters
{
public:
	static bool ResourceLoadFlag;

protected:
	void InitalizeStateInfo() override;
	void InitalizeResourceLoad() override;
	void InitalizeRenderAndAnimation() override;
	void InitalizeCollider() override;

protected:
	void SkillAttackProcessing() override;

private:

public:
	Chicken();
	~Chicken();

protected:
	Chicken(const Chicken& _other) = delete;
	Chicken(Chicken&& _other) noexcept = delete;

private:
	Chicken& operator=(const Chicken& _other) = delete;
	Chicken& operator=(const Chicken&& _other) = delete;

public:
protected:
private:
};

