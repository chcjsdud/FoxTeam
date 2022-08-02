#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// ���� :
class GameEngineUIRenderer;
class Player;
class UI_Equip : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_Equip();
	~UI_Equip();

	// delete Function
	UI_Equip(const UI_Equip& _Other) = delete;
	UI_Equip(UI_Equip&& _Other) noexcept = delete;
	UI_Equip& operator=(const UI_Equip& _Other) = delete;
	UI_Equip& operator=(UI_Equip&& _Other) noexcept = delete;

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

	//����
private:
	Player* Player_;

	
};

