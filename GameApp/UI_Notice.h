#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// Ό³Έν :
class GameEngineUIRenderer;
class Player;
class UI_Notice : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_Notice();
	~UI_Notice();

	// delete Function
	UI_Notice(const UI_Notice& _Other) = delete;
	UI_Notice(UI_Notice&& _Other) noexcept = delete;
	UI_Notice& operator=(const UI_Notice& _Other) = delete;
	UI_Notice& operator=(UI_Notice&& _Other) noexcept = delete;

	bool UIOn;

protected:
	GameEngineUIRenderer* BackGroundRenderer;
	GameEngineUIRenderer* DayNightRenderer;
	GameEngineUIRenderer* TimeRenderer;

	GameEngineUIRenderer* NightChangeClockRenderer;
	GameEngineUIRenderer* NightChangeTimeRenderer;

	GameEngineUIRenderer* DayCountRenderer;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;

	float4 UI_Pos;
	float4 Day_Pos;
	float4 Time_Pos;

	float4 Clock_Pos;
	float4 ClockTime_Pos;

	float4 DayCount_Pos;
};

