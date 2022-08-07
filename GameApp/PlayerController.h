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

왜 있냐? : 플레이어를 스위칭해 가며 조종할 수 있게끔 만든것
			모든 플레이어들을 미리 스폰해 놓고, 그중에서 조종하고 싶은 대상을 고를 수 있다.



	노트 1
	1. 플레이어 컨트롤러는 플레이어의 조작에 관해,
	2. 플레이어 클레스는 조작에 의한 세부 동작에 관해
*/

class Player;
class GameMouse;
class PlayerController : public GameEngineActor
{
	friend Player;
	friend GameMouse;

public:
	PlayerController(); // default constructer 디폴트 생성자
	~PlayerController(); // default destructer 디폴트 소멸자

private:
	LH_Mouse* GameMouse_;

	GameEngineFSM CameraState_;

	std::map<std::string, Player*> PlayerMap_;
	Player* MainPlayer_;

	// 임시 bool 코드 
#pragma region 조작 변수
	bool Key_RB_;
	bool Key_LB_;
	bool Key_Stop_;
	bool Key_Attack_;


	bool IsAttack_;
	bool IsMove_;

	bool Mouse_NavoCol_; // 마우스가 네비메쉬를 클릭 했을때
	//bool Mouse_ActorCol_; // 마우스가 엑터를 클릭 했을때
	GameEngineActor* Mouse_TargetActor_; // nullptr 인가를 조사해서 bool 값처럼 사용
	bool Mouse_UICol_; // 마우스가 UI를 클릭 했을때

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

	void MouseUpdate();
	void PlayerStateUpdate();
	void KeyStateUpdate();
	void CameraUpdate(float _DeltaTime);

private:

	// 이터널 리턴용 추가 : 쿼터뷰 시점
	void CameraUpdate_EternalReturn(float _DeltaTime);

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	void SwitchMainPlayer(std::string _Name);

	void ChangePlayerState(std::string _State);

	void AddPlayerMap(std::string _Name, Player* _Player);

	//이름을 지어 줬을경우
	void AddPlayerMap(Player* _Player);

	void ErasePlayerMap(std::string _Name)
	{
		PlayerMap_.erase(PlayerMap_.find(_Name));
	}

	void SetMainActor(Player* _Actor)
	{
		MainPlayer_ = _Actor;
	}

	void SetMainActor(std::string _Name)
	{
		MainPlayer_ = PlayerMap_.find(_Name)->second;
	}

private:
	PlayerController(const PlayerController& _other) = delete;
	PlayerController(PlayerController&& _other) = delete;
	PlayerController& operator=(const PlayerController& _other) = delete;
	PlayerController& operator=(const PlayerController&& _other) = delete;
};

