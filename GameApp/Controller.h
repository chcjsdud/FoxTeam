#pragma once

// 분류 : 
// 용도 : 
// 설명 : 
enum class Controller_Order
{
	None_Idle,
	A_RB_Attack_Target,
	A_Attack_Pos,
	RB_Move,
	S_Stop,
	H_Hold,
	X_Rest,
	F_ReLoad,

	Q_Skill,
	W_Skill,
	E_Skill,
	R_Skill,
	D_Weapon_Skill
};

class Unit;
class Controller
{
private:	// member Var

public:
	Controller(); // default constructer 디폴트 생성자
	~Controller(); // default destructer 디폴트 소멸자

public:
	const Controller_Order Controller_GetOrder()
	{
		return Controller_Order_;
	}

	void Controller_SetController_Order(Controller_Order _Controller_Order)
	{
		Controller_Order_ = _Controller_Order;
	}
	Unit* Controller_GetTargetUnit()
	{
		return Controller_Target_Unit_;
	}

	const float4 Controller_GetTarget_Pos()
	{
		return Controller_Target_Pos_;
	}

private:		//delete operator

	virtual void Start() =0;
	virtual void Update(float _DeltaTime)=0 ;
private:	

#pragma region 컨트롤러가 넘겨줘야할 변수들, 컨트롤러는 결과적으로 이 3가지를 리턴하기 위해 존재함
	float4 Controller_Target_Pos_; // 마우스로 타겟한 최종 지점
	Unit* Controller_Target_Unit_; //타겟이 된 유닛
	Controller_Order Controller_Order_;
	//unsigned int Controller_Target_ID_;
#pragma endregion

protected:		// delete constructer
	Controller(const Controller& _other) = delete; // default Copy constructer 디폴트 복사생성자
	Controller(Controller&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	Controller& operator=(const Controller& _other) = delete; // default Copy operator 디폴트 대입 연산자
	Controller& operator=(const Controller&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자
};

