#pragma once
#include <GameEngine/GameEngineActor.h>
// 분류 : 
// 용도 : 로비레벨에 들어갈 UI액터를 모아두는 곳
// 설명 : 



class Lobby_ButtonOne;
class Lobby_ButtonTwo;
class Lobby_PortraitBg;
class LobbyBackground;
class Lobby_CharFull;
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
	Lobby_ButtonOne* GetHostButton() { return hostButtonUI_; }
	Lobby_ButtonTwo* GetClientButton() { return clientButtonUI_; }

public:
	//내가 고른 캐릭터가 뭔지 가지고 있을 static 변수
	static JobType SelectedChar;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	LobbyBackground* backGroundUI_;
	Lobby_ButtonOne* hostButtonUI_;
	Lobby_ButtonTwo* clientButtonUI_;
	Lobby_CharFull* charFullUI_;
	std::vector<Lobby_PortraitBg*> PortraitBg;


};

