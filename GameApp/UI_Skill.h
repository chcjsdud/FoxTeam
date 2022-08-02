#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// 설명 :
class GameEngineUIRenderer;
class Player;
class UI_Skill : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_Skill();
	~UI_Skill();

	// delete Function
	UI_Skill(const UI_Skill& _Other) = delete;
	UI_Skill(UI_Skill&& _Other) noexcept = delete;
	UI_Skill& operator=(const UI_Skill& _Other) = delete;
	UI_Skill& operator=(UI_Skill&& _Other) noexcept = delete;

	bool UIOn;

public:
	void SetPlayer(Player* _Player)
	{
		Player_ = _Player;
	}

protected:
	GameEngineUIRenderer* BackGroundRenderer;
	//리소스가 많아지면 맵을 사용할 예정
	//map<string, GameEngineUIRenderer*> UIRendererMap;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;

	//이현
private:
	Player* Player_;

	
};

