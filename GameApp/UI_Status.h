#pragma once
#include <GameEngine/GameEngineActor.h>
#include "CharacterStat.h"

using namespace std;
// ���� :
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
	void SetStatus(CharacterStat* _Mystat);
	
protected:
	GameEngineUIRenderer* MainStatusRenderer;
	GameEngineUIRenderer* SubStatusRenderer;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;

	//����
private:

#pragma region �������ͽ� �� ��¿� ������+��ġ

	GameEngineUIRenderer* AttackValue_Renderer;
	GameEngineUIRenderer* DefenseValue_Renderer;
	GameEngineUIRenderer* AttackSpeedValue_Renderer;
	GameEngineUIRenderer* MoveSpeedValue_Renderer;
	GameEngineUIRenderer* CriticalValue_Renderer;

	float4 Value_XPivot;
	float4 Value_YPivot;

	float4 AttackValue_Pos;
	float4 DefenseValue_Pos;
	float4 AttackSpeedValue_Pos;
	float4 MoveSpeedValue_Pos;
	float4 CriticalValue_Pos;

#pragma endregion 


};

