#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// Ό³Έν :
class GameEngineUIRenderer;
class Player;
class RockOnUI : public GameEngineActor
{
public:
	// constrcuter destructer
	RockOnUI();
	~RockOnUI();

	// delete Function
	RockOnUI(const RockOnUI& _Other) = delete;
	RockOnUI(RockOnUI&& _Other) noexcept = delete;
	RockOnUI& operator=(const RockOnUI& _Other) = delete;
	RockOnUI& operator=(RockOnUI&& _Other) noexcept = delete;

	bool UIOn;

protected:
	GameEngineUIRenderer* TopRenderer;
	map<string, GameEngineUIRenderer*> UIRendererMap;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;

private:
	Player* Player_;

public:
	void SetPlayer(Player* _Player)
	{
		Player_ = _Player;
	}
	
};

