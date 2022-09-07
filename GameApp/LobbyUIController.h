#pragma once
#include <GameEngine/GameEngineActor.h>
// 분류 : 
// 용도 : 
// 설명 : 



class Lobby_ButtonOne;
class Lobby_ButtonTwo;
class Lobby_PortraitBg;
class LobbyBackground;
class LobbyUIController : public GameEngineActor
{
public:
	LobbyUIController(); // default constructer 디폴트 생성자
	~LobbyUIController(); // default destructer 디폴트 소멸자
	LobbyUIController(const LobbyUIController& _other) = delete; // default Copy constructer 디폴트 복사생성자
	LobbyUIController(LobbyUIController&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	LobbyUIController& operator=(const LobbyUIController& _other) = delete; // default Copy operator 디폴트 대입 연산자
	LobbyUIController& operator=(const LobbyUIController&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void InitUI();
	Lobby_PortraitBg* GetPortraitVector(int _index);


protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	LobbyBackground* backGroundUI_;
	Lobby_ButtonOne* hostButtonUI_;
	Lobby_ButtonTwo* clientButtonUI_;
	std::vector<Lobby_PortraitBg*> PortraitBg;
};

