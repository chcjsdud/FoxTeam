#pragma once
#include "Characters.h"

// �з� : ĳ����
// �뵵 : ��Ű
// ���� : ���� ��ų �� ������ ����
class Yuki : public Characters
{
private:

public:
	Yuki();
	~Yuki();

protected:		// delete constructer
	Yuki(const Yuki& _other) = delete;
	Yuki(Yuki&& _other) noexcept = delete;

private:		//delete operator
	Yuki& operator=(const Yuki& _other) = delete;
	Yuki& operator=(const Yuki&& _other) = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void BaseInfoSetting();
	void SkillInfoSetting();

public: // ��Ű �������� �� ��ų���� ����
	void Initialize() override;

};

