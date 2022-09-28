#pragma once
#include "Monsters.h"

// �з� : ����
// �뵵 : 
// ���� : ����(����)�� ���� Ư���� �����ϴ� Ŭ����
class Wolf : public Monsters
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
	Wolf();
	~Wolf();

protected:
	Wolf(const Wolf& _other) = delete;
	Wolf(Wolf&& _other) = delete;

private:
	Wolf& operator=(const Wolf& _other) = delete;
	Wolf& operator=(const Wolf&& _other) = delete;

public:
protected:
private:
};

