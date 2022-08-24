#pragma once

// �з� : 
// �뵵 : 
// ���� : 
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
	Controller(); // default constructer ����Ʈ ������
	~Controller(); // default destructer ����Ʈ �Ҹ���

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

#pragma region ��Ʈ�ѷ��� �Ѱ������ ������, ��Ʈ�ѷ��� ��������� �� 3������ �����ϱ� ���� ������
	float4 Controller_Target_Pos_; // ���콺�� Ÿ���� ���� ����
	Unit* Controller_Target_Unit_; //Ÿ���� �� ����
	Controller_Order Controller_Order_;
	//unsigned int Controller_Target_ID_;
#pragma endregion

protected:		// delete constructer
	Controller(const Controller& _other) = delete; // default Copy constructer ����Ʈ ���������
	Controller(Controller&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	Controller& operator=(const Controller& _other) = delete; // default Copy operator ����Ʈ ���� ������
	Controller& operator=(const Controller&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����
};

