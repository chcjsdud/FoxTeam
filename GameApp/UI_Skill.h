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
	void SetJobType(JobType _myjob);

	void SetSkillType(JobType _myjob, bool _Skilltype);

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

#pragma region 아이콘 렌더러

	GameEngineUIRenderer* Icon_Q;
	GameEngineUIRenderer* Icon_W;
	GameEngineUIRenderer* Icon_E;
	GameEngineUIRenderer* Icon_R;
	GameEngineUIRenderer* Icon_Passive;
	GameEngineUIRenderer* Icon_Weapon;

#pragma endregion

#pragma region float4 모음

	float4 Skill_BackGroundPos;
	float4 Icon_Q_Pos;
	float4 Icon_W_Pos;
	float4 Icon_E_Pos;
	float4 Icon_R_Pos;
	float4 Icon_Passive_Pos;
	float4 Icon_Weapon_Pos;

#pragma endregion
};

