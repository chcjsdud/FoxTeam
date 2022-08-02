#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

// *** 들어가기에 앞서
// 임시로 만든 컨트롤러 클래스입니다.
// Play 씬의 모든 조작과 액터들의 상태를 가지고 있습니다.

// 이후 AI 도입 시 컨트롤러에서 스위칭이 가능하게 하면 좋을 거 같은데수



// 플레이어의 "조종 화면"을 구현하게 될 클레스로 재설계함
// 조종 가능한 '플레이어' 클레스만 저장하고, 화면 UI나 KeyInput 등을 관리할것




class Player;
class GameMouse;
class PlayController : public GameEngineActor
{
	friend Player;
	friend GameMouse;

public:
	PlayController(); // default constructer 디폴트 생성자
	~PlayController(); // default destructer 디폴트 소멸자
	PlayController(const PlayController& _other) = delete; // default Copy constructer 디폴트 복사생성자
	PlayController(PlayController&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	PlayController& operator=(const PlayController& _other) = delete; // default Copy operator 디폴트 대입 연산자
	PlayController& operator=(const PlayController&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

private:
	GameMouse* testGameMouse_;

	GameEngineFSM CameraState_;

	class Player* MainActor_;
	
	// std::vector<Player*> players_;
	//Player* testPlayer_;
	
private:
	void InitInput();
	void InitActor();

private:
	// update류 함수에 같이 들어가 카메라 업데이트, 바로 뒤에서 바라보는 함수
	void CameraUpdate_BackPosition(float _DeltaTime);
	void CameraUpdate_UpPosition(float _DeltaTime);

	// 이터널 리턴용 추가 : 쿼터뷰 시점
	void CameraUpdate_EternalReturn(float _DeltaTime);

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	void SetMainActor(Player* _Actor)
	{
		MainActor_ = _Actor;
	}
};

