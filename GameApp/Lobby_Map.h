#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;

class Lobby_Map : public GameEngineActor
{
public:
	Lobby_Map();
	~Lobby_Map();

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	Lobby_Map(const Lobby_Map& _Other) = delete;
	Lobby_Map(Lobby_Map&& _Other) noexcept = delete;
	Lobby_Map& operator=(const Lobby_Map& _Other) = delete;
	Lobby_Map& operator=(Lobby_Map&& _Other) noexcept = delete;
};

