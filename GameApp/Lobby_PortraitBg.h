#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineImageRenderer.h>

using namespace std;

class Lobby_PortraitBg : public GameEngineActor
{
public:
	Lobby_PortraitBg();
	~Lobby_PortraitBg();

public:
	GameEngineImageRenderer* BasicImageRenderer;
	GameEngineImageRenderer* SelectImageRenderer;
	GameEngineCollision* MouseCollision;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	Lobby_PortraitBg(const Lobby_PortraitBg& _Other) = delete;
	Lobby_PortraitBg(Lobby_PortraitBg&& _Other) noexcept = delete;
	Lobby_PortraitBg& operator=(const Lobby_PortraitBg& _Other) = delete;
	Lobby_PortraitBg& operator=(Lobby_PortraitBg&& _Other) noexcept = delete;
};

