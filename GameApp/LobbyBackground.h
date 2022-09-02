#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineImageRenderer.h>

using namespace std;

class LobbyBackground : public GameEngineActor
{
public:
	LobbyBackground();
	~LobbyBackground();
	LobbyBackground(const LobbyBackground& _Other) = delete;
	LobbyBackground(LobbyBackground&& _Other) noexcept = delete;
	LobbyBackground& operator=(const LobbyBackground& _Other) = delete;
	LobbyBackground& operator=(LobbyBackground&& _Other) noexcept = delete;

public:
	GameEngineImageRenderer* BackGroundRenderer;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:

};

