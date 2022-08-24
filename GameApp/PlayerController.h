#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include "Controller.h"

#include "LH_Mouse.h"

#include "ItemBase.h"

/*
하는일 : 플에이어 조작 화면이 구현된 클레스,
		1. UI 표시와 플레이어 캐릭터 조작에 관한 기능
		2. 모든 플레이어를 저장해 놓고, 스위칭하며 컨트롤 할 대상을 조정할 수 있음
		3. 플레이어의 키 조작과 명령을 가지고 있는 플레이어에게 하달하는 중간 역할
		4. 사람 -> 플레이어 컨트롤러 -> 플레이어

		5. 컨트롤 조작에 의해 발생하는것들을 Player class에 전달해줌
		마우스 좌표, 선택한 엑터, 키 조작 등등


		오로지 셀렉트된 유닛에 명령만 내림

		서버와 무관하게 독립적으로 개개인이 가지게 될것
*/

// 시작과 동시에 자신의 이름을 짓고 ip 서버에 접속하도록 하고,
// 만약 중복된 이름이 존재하면 못들어가게함

class Player;
class Unit;
class GameMouse;
//class PlayerController : public GameEngineActor
class PlayerController : public Controller
{
	friend Player;
	friend GameMouse;

public:
	PlayerController(); // default constructer 디폴트 생성자
	~PlayerController(); // default destructer 디폴트 소멸자

#pragma region Public:
public:
	void PlayerController_SetPlayUserName(std::string _UserName)
	{
		PlayUserName_ = _UserName;
	}

	const std::string PlayerController_GetPlayUserName()
	{
		return PlayUserName_;
	}

	void PlayerController_SetMainPlayer(Player* _Player)
	{
		MainPlayer_ = _Player;
	}
#pragma endregion
	
private:
	// 컨트롤러가 서버로 모든 명령을 보내주면, 서버가 모든 명령을 받아 각기 캐릭터들에게 명령을 하달한 후, 
	// 그 결과값만 클라이언트로 보내줌
	// 클라이언트는 결과값만 반영하고 다시 명령을 서버로 보내줌
#pragma region 플레이 유저 고유 정보
	//GameEngineFSM ServerState_; // 서버 대기, 대기실, 플레이 등등 

	std::string PlayUserName_; //유저 이름
	GameEngineFSM CameraState_; // 카메라
	LH_Mouse* GameMouse_; // 마우스 인풋

	Player* MainPlayer_; // 메인 플레이어 캐릭터 //각기 pc마다 따로 가지게 되는 포인터 값

	//unsigned int MainPlayerID_; // 메인 플레이어 캐릭터 ID 서버 전체가 공유하는 ID값

	ItemBase* Controller_ItemList[10];
	// 아이템이 가졌ㅇ면 하는것
	// 1. std::fuction의 지속효과, 발동 효과
	// 2. 스텟 구조체
	// 3. 아이템 타입
	// 번외
	// 아이템 조합기능
#pragma endregion

#pragma region 조작 변수
	float4 UIMousePos2D_; 
	float4 UIMousePos3D_; 

	bool Mouse_NaviCol_; // 마우스가 네비메쉬를 클릭 했을때
	bool Mouse_UICol_; // 마우스가 UI를 클릭 했을때

	bool Key_LB_;
	bool Key_RB_;

	//Order
	bool Key_A_;	// 공격
	bool Key_S_;	// 이동, 공격 정지
	bool Key_H_;	// 이동 멈춤
	bool Key_X_;	// 휴식
	bool Key_F_;	// 재장전

	bool Key_Q_;
	bool Key_W_;
	bool Key_E_;
	bool Key_R_;
	bool Key_D_;	// 무기 스킬

	//UI
	bool Key_Tab_;	//스코어 보드
	bool Key_C_;	// 캐릭터 정보
	bool Key_V_;	// 숙련도 확인
	bool Key_B_;	// 모든 아이템 확인
	bool Key_M_;	// 전체 지도
	bool Key_P_;	// 설정 루트 확인
	bool Key_Y_;	// 카메라 고정 변경

	bool Key_Esc_;	// ESC

	//아이템
	bool Key_Num_[10];

	class PlayerControllerUpdatePaket // 임시 구현 상태
	{
		friend PlayerController;

		std::string PlayUserName_; //유저 이름

		float4 Controller_Target_Pos_; // 마우스로 타겟한 최종 지점
		//Unit* Controller_Target_Unit_; //타겟이 된 유닛
		unsigned int Controller_Target_UnitID_; //타겟이 된 유닛 ID

		Controller_Order Controller_Order_; //서버에 플레이어 캐릭터의 명령을 전달해주면 서버에서 처리해서 최종 결과값만 반영되게 될것

		bool Key_Num_[10];
	};

	PlayerControllerUpdatePaket* PlayerControllerUpdatePaket_;

#pragma endregion

private:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void PlayerController_Init();
	void PlayerController_KeyState_Init();
	void PlayerController_KeyState_Update(float _DeltaTime);
	void PlayerController_Mouse_Update();
	void PlayerController_Camera_Update(float _DeltaTime);

private:
	// 이터널 리턴용 추가 : 쿼터뷰 시점
	void CameraUpdate_EternalReturn(float _DeltaTime);

private:
	PlayerController(const PlayerController& _other) = delete;
	PlayerController(PlayerController&& _other) = delete;
	PlayerController& operator=(const PlayerController& _other) = delete;
	PlayerController& operator=(const PlayerController&& _other) = delete;
};

