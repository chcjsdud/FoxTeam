#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// 설명 :
class GameEngineUIRenderer;
class Player;
class UI_Status : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_Status();
	~UI_Status();

	// delete Function
	UI_Status(const UI_Status& _Other) = delete;
	UI_Status(UI_Status&& _Other) noexcept = delete;
	UI_Status& operator=(const UI_Status& _Other) = delete;
	UI_Status& operator=(UI_Status&& _Other) noexcept = delete;

	bool UIOn;

public:
	void SetPlayer(Player* _Player)
	{
		Player_ = _Player;
	}
	
protected:
	GameEngineUIRenderer* MainStatusRenderer;
	GameEngineUIRenderer* SubStatusRenderer;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;

	//이현
private:
	Player* Player_;

#pragma region 스테이터스 값 출력용 렌더러 모음

	GameEngineUIRenderer* AttackValue_Renderer;
	GameEngineUIRenderer* DefenseValue_Renderer;
	GameEngineUIRenderer* AttackSpeedValue_Renderer;
	GameEngineUIRenderer* MoveSpeedValue_Renderer;
	GameEngineUIRenderer* CriticalValue_Renderer;

#pragma endregion 


};

