#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;

class LobbyBackground : GameEngineActor
{
public:
	LobbyBackground();
	~LobbyBackground();

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	LobbyBackground(const LobbyBackground& _Other) = delete;
	LobbyBackground(LobbyBackground&& _Other) noexcept = delete;
	LobbyBackground& operator=(const LobbyBackground& _Other) = delete;
	LobbyBackground& operator=(LobbyBackground&& _Other) noexcept = delete;
};

