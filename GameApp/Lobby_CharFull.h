#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineImageRenderer.h>

using namespace std;

class Lobby_CharFull : public GameEngineActor
{
public:
	Lobby_CharFull();
	~Lobby_CharFull();
	
	void SetChar(JobType _Character);

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	Lobby_CharFull(const Lobby_CharFull& _Other) = delete;
	Lobby_CharFull(Lobby_CharFull&& _Other) noexcept = delete;
	Lobby_CharFull& operator=(const Lobby_CharFull& _Other) = delete;
	Lobby_CharFull& operator=(Lobby_CharFull&& _Other) noexcept = delete;

private:
	GameEngineImageRenderer* CharFull_Renderer;
};

