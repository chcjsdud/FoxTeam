#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// Ό³Έν :
class GameEngineUIRenderer;
class Player;
class UI_HPBars : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_HPBars();
	~UI_HPBars();

	// delete Function
	UI_HPBars(const UI_HPBars& _Other) = delete;
	UI_HPBars(UI_HPBars&& _Other) noexcept = delete;
	UI_HPBars& operator=(const UI_HPBars& _Other) = delete;
	UI_HPBars& operator=(UI_HPBars&& _Other) noexcept = delete;

	bool UIOn;

public:

protected:
	GameEngineUIRenderer* HPBar_Renderer;
	GameEngineUIRenderer* SPBar_Renderer;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;

private:
	float4 HpBarPos;
	float4 HpBarSize;

	float4 SpBarPos;
	float4 SpBarSize;
	
};

