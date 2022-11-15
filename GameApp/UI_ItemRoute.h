#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// ¼³¸í :
class GameEngineUIRenderer;
class ItemBase;
class UI_ItemRoute : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_ItemRoute();
	~UI_ItemRoute();

	// delete Function
	UI_ItemRoute(const UI_ItemRoute& _Other) = delete;
	UI_ItemRoute(UI_ItemRoute&& _Other) noexcept = delete;
	UI_ItemRoute& operator=(const UI_ItemRoute& _Other) = delete;
	UI_ItemRoute& operator=(UI_ItemRoute&& _Other) noexcept = delete;

	bool UIOn;

public:

	int SlotMouseCollisionCheck();

protected:
	GameEngineUIRenderer* Renderer;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;

private:

#pragma region ½½·Ô float4 ÇÇº¿°ª

	float4 BackGroundPos;
	float4 BasicSlotPos;
	float4 SlotXPivot;
	float4 SlotYPivot;
	float4 SlotSize;

#pragma endregion

#pragma region ½½·Ô °ü·Ã ¸ðÀ½

	GameEngineCollision* Slot0Collision;
	GameEngineCollision* Slot1Collision;
	GameEngineCollision* Slot2Collision;
	GameEngineCollision* Slot3Collision;
	GameEngineCollision* Slot4Collision;
	GameEngineCollision* Slot5Collision;
	GameEngineCollision* Slot6Collision;
	GameEngineCollision* Slot7Collision;
	GameEngineCollision* Slot8Collision;
	GameEngineCollision* Slot9Collision;


	GameEngineUIRenderer* Slot0_BGRenderer;
	GameEngineUIRenderer* Slot1_BGRenderer;
	GameEngineUIRenderer* Slot2_BGRenderer;
	GameEngineUIRenderer* Slot3_BGRenderer;
	GameEngineUIRenderer* Slot4_BGRenderer;
	GameEngineUIRenderer* Slot5_BGRenderer;
	GameEngineUIRenderer* Slot6_BGRenderer;
	GameEngineUIRenderer* Slot7_BGRenderer;
	GameEngineUIRenderer* Slot8_BGRenderer;
	GameEngineUIRenderer* Slot9_BGRenderer;

	GameEngineUIRenderer* Slot0_IconRenderer;
	GameEngineUIRenderer* Slot1_IconRenderer;
	GameEngineUIRenderer* Slot2_IconRenderer;
	GameEngineUIRenderer* Slot3_IconRenderer;
	GameEngineUIRenderer* Slot4_IconRenderer;
	GameEngineUIRenderer* Slot5_IconRenderer;
	GameEngineUIRenderer* Slot6_IconRenderer;
	GameEngineUIRenderer* Slot7_IconRenderer;
	GameEngineUIRenderer* Slot8_IconRenderer;
	GameEngineUIRenderer* Slot9_IconRenderer;

#pragma endregion
	
};

