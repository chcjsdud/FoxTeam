#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// Ό³Έν :
class GameEngineUIRenderer;
class Player;
class UI_Time : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_Time();
	~UI_Time();

	// delete Function
	UI_Time(const UI_Time& _Other) = delete;
	UI_Time(UI_Time&& _Other) noexcept = delete;
	UI_Time& operator=(const UI_Time& _Other) = delete;
	UI_Time& operator=(UI_Time&& _Other) noexcept = delete;

	bool UIOn;

protected:
	GameEngineUIRenderer* BackGroundRenderer;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;

};

