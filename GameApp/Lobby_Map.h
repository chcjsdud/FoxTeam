#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineImageRenderer.h>

using namespace std;

class Lobby_Map : public GameEngineActor
{
public:
	Lobby_Map();
	~Lobby_Map();

	Location GetSelectLocation(float4 _Position);

	Location SelectedLocation;

public:
	GameEngineImageRenderer* fullMapRenderer_;
	GameEngineImageRenderer* areaChoiceMapRenderer_;
	GameEngineImageRenderer* selectAreaRenderer_;

	GameEngineCollision* textureCollision_;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	Lobby_Map(const Lobby_Map& _Other) = delete;
	Lobby_Map(Lobby_Map&& _Other) noexcept = delete;
	Lobby_Map& operator=(const Lobby_Map& _Other) = delete;
	Lobby_Map& operator=(Lobby_Map&& _Other) noexcept = delete;
};

