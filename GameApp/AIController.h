#pragma once
//#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>


// AI를 통제하는 컨트롤러,
// 각기AI 유닛마다 보유할 수 있게 해줄 생각
//

// 이거 필요한가? 

class Unit;
class AIController //: public GameEngineActor
{
public:
	AIController();
	~AIController();

private:
	Unit* MainUnit_;

	// 임시 bool 코드 
#pragma region 조작 변수
	bool Key_RB_;
	bool Key_LB_;
	bool Key_Stop_;

	GameEngineFSM AIState_;

	GameEngineActor* TargetActor_;
	bool Stopkey_;
	bool isClick_;

	bool IsAttack_;

	//임시, 마우스로 클릭한 좌표, 마우스 클릭시 딱 1번 정해짐, 나중에 현 좌표와 이 좌표가 같아지면 초기화
	float4 TempMousePos_; // 맵과 충돌한 마우스의 좌표를 가져와야함, 별개로 UI 클릭할 마우스 좌표도 필요함

	//임시 마우스 클릭 방향 , 마우스 클릭시 딱 1번 정해짐
	float4 TmepMouseDir_;

	//플레이어 조작의 종류에 대해 미리 정의해 둘 필요가 있다.
	/*
	1. 우클릭 이동
	2. 우클릭 공격
	3. S키로 움직임 캔슬

	2. 좌클릭 우클릭
	3. 좌클릭 공격
	4. A키, 공격 (있나?)
	*/

#pragma endregion

public:
	void Update(float _DeltaTime);

	void AIController_Off();
	void AIController_On();

private:

	void Idle_Start();
	void Idle_Update(float _DeltaTime);
	void Idle_End();

	void Chase_Start();
	void Chase_Update(float _DeltaTime);
	void Chase_End();

	void Attack_Start();
	void Attack_Update(float _DeltaTime);
	void Attack_End();


public:
	void ChangePlayerState(std::string _State);

	void SetMainActor(Unit* _MainUnit)
	{
		MainUnit_ = _MainUnit;
	}

private:
	AIController(const AIController& _other) = delete;
	AIController(AIController&& _other) = delete;
	AIController& operator=(const AIController& _other) = delete;
	AIController& operator=(const AIController&& _other) = delete;
};

