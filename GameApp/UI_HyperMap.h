#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineImageRenderer.h>

using namespace std;

class UI_HyperMap : public GameEngineActor
{
public:
	UI_HyperMap();
	~UI_HyperMap();

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
	UI_HyperMap(const UI_HyperMap& _Other) = delete;
	UI_HyperMap(UI_HyperMap&& _Other) noexcept = delete;
	UI_HyperMap& operator=(const UI_HyperMap& _Other) = delete;
	UI_HyperMap& operator=(UI_HyperMap&& _Other) noexcept = delete;
};
