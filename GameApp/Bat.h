#pragma once
#include "Monsters.h"

// �з� : ����
// �뵵 : 
// ���� : ����(����)�� ���� Ư���� �����ϴ� Ŭ����
class Bat : public Monsters
{
public:
	static bool ResourceLoadFlag;

protected:
	void InitalizeStateInfo() override;
	void InitalizeResourceLoad() override;
	void InitalizeRenderAndAnimation() override;
	void InitalizeCollider() override;

private:

public:
	Bat();
	~Bat();

protected:
	Bat(const Bat& _other) = delete;
	Bat(Bat&& _other) noexcept = delete;

private:
	Bat& operator=(const Bat& _other) = delete;
	Bat& operator=(const Bat&& _other) = delete;

public:
protected:
private:
};

