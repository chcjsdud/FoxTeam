#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// 설명 :
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

protected:
	GameEngineUIRenderer* Renderer;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;

	//이현
private:
	Player* Player_;

public:
	void SetPlayer(Player* _Player)
	{
		Player_ = _Player;
	}
	
};

