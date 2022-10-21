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
	GameEngineUIRenderer* FontRenderer;
	float Time;
	float BasicAlpha;


private:
	void Start() override;
	void Update(float _Time) override;

	float4 UI_Pos;
	float4 Font_Pos;

};

