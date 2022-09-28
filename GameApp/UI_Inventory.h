#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// ¼³¸í :
class GameEngineUIRenderer;
class Player;
class UI_Inventory : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_Inventory();
	~UI_Inventory();

	// delete Function
	UI_Inventory(const UI_Inventory& _Other) = delete;
	UI_Inventory(UI_Inventory&& _Other) noexcept = delete;
	UI_Inventory& operator=(const UI_Inventory& _Other) = delete;
	UI_Inventory& operator=(UI_Inventory&& _Other) noexcept = delete;

	bool UIOn;

public:
	void SetPlayer(Player* _Player)
	{
		Player_ = _Player;
	}

protected:
	GameEngineUIRenderer* Renderer;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;

private:
	Player* Player_;

#pragma region ½½·Ô float4 ÇÇº¿°ª

	float4 BackGroundPos;
	float4 BasicSlotPos;
	float4 SlotXPivot;
	float4 SlotYPivot;
	float4 SlotSize;

#pragma endregion
	
};

