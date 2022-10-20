#pragma once
#include "Monsters.h"

// �з� : ����
// �뵵 : 
// ���� : ��(����)�� ���� ����� ����
class Bear : public Monsters
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
	Bear();
	~Bear();

protected:
	Bear(const Bear& _other) = delete;
	Bear(Bear&& _other) noexcept = delete;

private:
	Bear& operator=(const Bear& _other) = delete;
	Bear& operator=(const Bear&& _other) = delete;

public:
protected:
private:
};

