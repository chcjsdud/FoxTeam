#pragma once
#include "LH_Player.h"

// 분류 : 캐릭터
// 용도 : 유키
// 설명 : 고유 스킬 및 패턴을 관리
class Player_Yuki : public Player
{
private:

public:
	Player_Yuki();
	~Player_Yuki();

private:
	//FBX Renderer
	//GameEngineFBXRenderer* FBXRenderer_;

private:		//delete operator
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void ComponenetInit();
	virtual void Unit_Set_State_Init()override;
	void UIInit();

	void DEBUGUpdate(float _DeltaTime);

private:

	virtual void Order_Q_Start();
	virtual void Order_Q_Update(float _DeltaTime);
	virtual void Order_Q_End();

	virtual void Order_W_Start();
	virtual void Order_W_Update(float _DeltaTime);
	virtual void Order_W_End();

	virtual void Order_E_Start();
	virtual void Order_E_Update(float _DeltaTime);
	virtual void Order_E_End();

	virtual void Order_R_Start();
	virtual void Order_R_Update(float _DeltaTime);
	virtual void Order_R_End();

	virtual void Order_D_Start();
	virtual void Order_D_Update(float _DeltaTime);
	virtual void Order_D_End();

	virtual void Action_Idle_Start() override;
	virtual void Action_Idle_Update(float _DeltaTime) override;
	virtual void Action_Idle_End() override;

	virtual void Action_Walk_Start() override;
	virtual void Action_Walk_Update(float _DeltaTime) override;
	virtual void Action_Walk_End() override;

	virtual void Action_Attack_Start() override;
	virtual void Action_Attack_Update(float _DeltaTime) override;
	virtual void Action_Attack_End() override;

	virtual void Action_Q_Start();
	virtual void Action_Q_Update(float _DeltaTime);
	virtual void Action_Q_End();

	virtual void Action_W_Start();
	virtual void Action_W_Update(float _DeltaTime);
	virtual void Action_W_End();

	virtual void Action_E_Start();
	virtual void Action_E_Update(float _DeltaTime);
	virtual void Action_E_End();

	virtual void Action_R_Start();
	virtual void Action_R_Update(float _DeltaTime);
	virtual void Action_R_End();

	virtual void Action_D_Start();
	virtual void Action_D_Update(float _DeltaTime);
	virtual void Action_D_End();

private:		// delete constructer
	Player_Yuki(const Player_Yuki& _other) = delete;
	Player_Yuki(Player_Yuki&& _other) = delete;
	Player_Yuki& operator=(const Player_Yuki& _other) = delete;
	Player_Yuki& operator=(const Player_Yuki&& _other) = delete;
};

