#pragma once
#include <GameEngine/GameEngineActor.h>

// *** 들어가기에 앞서
// 임시로 만든 컨트롤러 클래스입니다.
// Play 씬의 모든 조작과 액터들의 상태를 가지고 있습니다.

// 이후 AI 도입 시 컨트롤러에서 스위칭이 가능하게 하면 좋을 거 같은데수

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
	
	// std::vector<Player*> players_;
	Player* testPlayer_;
	
private:
	void InitInput();
	void InitActor();

private:
	void CheckInput(float _DeltaTime);
	

private:
	void Start() override;
	void Update(float _DeltaTime) override;

};

