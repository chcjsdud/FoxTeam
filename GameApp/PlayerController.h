#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include "LH_Mouse.h"

/*
하는일 : 플에이어 조작 화면이 구현된 클레스,
		1. UI 표시와 플레이어 캐릭터 조작에 관한 기능
		2. 모든 플레이어를 저장해 놓고, 스위칭하며 컨트롤 할 대상을 조정할 수 있음
		3. 플레이어의 키 조작과 명령을 가지고 있는 플레이어에게 하달하는 중간 역할
		4. 사람 -> 플레이어 컨트롤러 -> 플레이어

		5. 컨트롤 조작에 의해 발생하는것들을 Player class에 전달해줌
		마우스 좌표, 선택한 엑터, 키 조작 등등


		오로지 셀렉트된 유닛에 명령만 내림
*/
enum class Order
{
	None,
	Attack_Target,
	Attack_Pos,
	Move,
	Stop,
	Hold,
	Pattrol,

	Q,
	W,
	E,
	R
};

enum class ControllerkeyState
{
	None,
	A,
	S,
	M,
	H,
	P,

	Q,
	W,
	E,
	R
};

class Player;
class Unit;
class GameMouse;
class PlayerController : public GameEngineActor
{
	friend Player;
	friend GameMouse;

public:
	PlayerController(); // default constructer 디폴트 생성자
	~PlayerController(); // default destructer 디폴트 소멸자

	
private:
#pragma region 컨트롤러가 넘겨줘야할 변수들, 컨트롤러는 결과적으로 이 3가지를 리턴하기 위해 존재함
	float4 Controller_Target_Pos_; // 마우스로 타겟한 최종 지점
	Unit* Controller_Target_Unit_; //타겟이 된 유닛
	//unsigned int Controller_Target_ID_;
	Order Controller_Order_;

#pragma endregion

#pragma region 컨트롤러가 넘겨줘야할 변수들, 컨트롤러는 결과적으로 이 3가지를 리턴하기 위해 존재함
#pragma endregion
public:
	void SetPlayUserName(std::string _UserName)
	{
		PlayUserName_ = _UserName;
	}

	const std::string GetPlayUserName()
	{
		return PlayUserName_;
	}

	const Order PlayerController_GetOrder()
	{
		return Controller_Order_;
	}

	void SetController_Order(Order _Controller_Order)
	{
		Controller_Order_ = _Controller_Order;
	}
	Unit* PlayerController_GetTargetUnit()
	{
		return Controller_Target_Unit_;
	}

	const float4 PlayerController_GetTarget_Pos()
	{
		return Controller_Target_Pos_;
	}

	void SetMainActor(Player* _Actor)
	{
		MainPlayer_ = _Actor;
	}

	//unsigned int GetPlayerID_()
	//{
	//	return CurPlayerUnitID_;
	//}

	//const unsigned int PlayerController_GetTarget_ID()
	//{
	//	return Controller_Target_ID_;
	//}

	//void SetMainPlayerID_(unsigned int _PlayerID)
	//{
	//	CurPlayerUnitID_ = _PlayerID;
	//}


private:
	std::string PlayUserName_;

	ControllerkeyState ControllerkeyState_;
	//unsigned int CurPlayerUnitID_;
	LH_Mouse* GameMouse_;

	GameEngineFSM CameraState_;

	Player* MainPlayer_;

	// 임시 bool 코드 
#pragma region 조작 변수

	float4 UIMousePos2D_; 
	float4 UIMousePos3D_; 

	//bool IsAttack_;
	//bool IsMove_;

	bool Mouse_NaviCol_; // 마우스가 네비메쉬를 클릭 했을때
	bool Mouse_UICol_; // 마우스가 UI를 클릭 했을때

	bool Key_LB_;
	bool Key_RB_;

	bool Key_A_;	// 공격
	bool Key_S_;	// 이동, 공격 정지
	bool Key_H_;	// 이동 멈춤

	bool Key_X_;	// 휴식
	bool Key_F_;	// 재장전
	bool Key_Tab_;	//스코어 보드
	bool Key_Esc_;	// ESC
	bool Key_C_;	// 캐릭터 정보
	bool Key_V_;	// 숙련도 확인
	bool Key_B_;	// 모든 아이템 확인
	bool Key_M_;	// 전체 지도
	bool Key_P_;	// 설정 루트 확인
	bool Key_Y_;	// 카메라 고정 변경

	//아이템
	bool Key_1_;
	bool Key_2_;
	bool Key_3_;
	bool Key_4_;
	bool Key_5_;
	bool Key_6_;
	bool Key_7_;
	bool Key_8_;
	bool Key_9_;
	bool Key_0_;

	bool Key_Q_;
	bool Key_W_;
	bool Key_E_;
	bool Key_R_;
	bool Key_D_;	// 무기 스킬

#pragma endregion


private:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void ControllerInit();
	void InitKeyState();
	void MouseUpdate();
	void KeyStateUpdate(float _DeltaTime);
	void CameraUpdate(float _DeltaTime);

private:
	// 이터널 리턴용 추가 : 쿼터뷰 시점
	void CameraUpdate_EternalReturn(float _DeltaTime);

private:
	PlayerController(const PlayerController& _other) = delete;
	PlayerController(PlayerController&& _other) = delete;
	PlayerController& operator=(const PlayerController& _other) = delete;
	PlayerController& operator=(const PlayerController&& _other) = delete;
};

