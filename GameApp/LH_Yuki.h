#pragma once
#include "Player.h"

// �з� : ĳ����
// �뵵 : ��Ű
// ���� : ���� ��ų �� ������ ����
class Yuki : public Player
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

	virtual void Idle_Start() ;
	virtual void Idle_Update(float _DeltaTime) ;
	virtual void Idle_End() ;

	virtual void Walk_Start() ;
	virtual void Walk_Update(float _DeltaTime) ;
	virtual void Walk_End() ;

	virtual void Run_Start() ;
	virtual void Run_Update(float _DeltaTime) ;
	virtual void Run_End() ;

	virtual void Attack_Start() ;
	virtual void Attack_Update(float _DeltaTime) ;
	virtual void Attack_End() ;

private:
	void BaseInfoSetting();
	void SkillInfoSetting();

public: // ��Ű �������� �� ��ų���� ����
	//void Initialize() override;

};

