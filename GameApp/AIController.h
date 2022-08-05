#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>


// AI를 통제하는 컨트롤러,
// 각기AI 유닛마다 보유할 수 있게 해줄 생각
//

class Unit;
class AIController : public GameEngineActor
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

private:
	void InitInput();
	//void InitActor();
	void UIInit();

	//void PlayerStateUpdate();
	void KeyStateUpdate();

private:
	void CameraUpdate_EternalReturn(float _DeltaTime);

private:
	void Start() override;
	void Update(float _DeltaTime) override;

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

