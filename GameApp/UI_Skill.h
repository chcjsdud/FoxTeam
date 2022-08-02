#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// ���� :
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
	//���ҽ��� �������� ���� ����� ����
	//map<string, GameEngineUIRenderer*> UIRendererMap;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;

	//����
private:
	Player* Player_;

	
};

