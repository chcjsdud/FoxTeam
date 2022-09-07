#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineImageRenderer.h>

using namespace std;

class Lobby_ButtonTwo : public GameEngineActor
{
public:
	Lobby_ButtonTwo();
	~Lobby_ButtonTwo();

	bool MouseCollisionCheck();

public:
	GameEngineImageRenderer* ButtonTwoRenderer;
	GameEngineCollision* ButtonTwoCollision;



protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	Lobby_ButtonTwo(const Lobby_ButtonTwo& _Other) = delete;
	Lobby_ButtonTwo(Lobby_ButtonTwo&& _Other) noexcept = delete;
	Lobby_ButtonTwo& operator=(const Lobby_ButtonTwo& _Other) = delete;
	Lobby_ButtonTwo& operator=(Lobby_ButtonTwo&& _Other) noexcept = delete;
};

