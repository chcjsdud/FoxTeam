#pragma once
#include "LH_Player.h"

// �з� : ĳ����
// �뵵 : ��Ű
// ���� : ���� ��ų �� ������ ����
class YSJ_Char : public Player
{
private:

public:
	YSJ_Char();
	~YSJ_Char();

private:
	//FBX Renderer
	GameEngineFBXRenderer* FBXRenderer_;
	GameEngineCollision* AttackHitBoxCollision_;

private:		//delete operator
	void Start() override;
	void Update(float _DeltaTime) override;

	// ������, �ݸ���, ��� Componenet �ʱ�ȭ
	void ComponenetInit();
	//State �ʱ�ȭ
	void StateInit();
	void UIInit();

	void DEBUGUpdate(float _DeltaTime);



private:
	void Action_Idle_Start() override;
	void Action_Idle_Update(float _DeltaTime) override;
	void Action_Idle_End() override;

	void Action_Walk_Start() override;
	void Action_Walk_Update(float _DeltaTime) override;
	void Action_Walk_End() override;

	//void Run_Start() override;
	//void Run_Update(float _DeltaTime) override;
	//void Run_End() override;

	void Action_Attack_Start() override;
	void Action_Attack_Update(float _DeltaTime) override;
	void Action_Attack_End() override;

private:		// delete constructer
	YSJ_Char(const YSJ_Char& _other) = delete;
	YSJ_Char(YSJ_Char&& _other) = delete;
	YSJ_Char& operator=(const YSJ_Char& _other) = delete;
	YSJ_Char& operator=(const YSJ_Char&& _other) = delete;
};

