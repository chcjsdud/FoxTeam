#pragma once
#include <GameEngine/GameEngineActor.h>
#include "CharacterStat.h"
#include "MonsterStateInfo.h"

using namespace std;

//캐릭터를 쫓아다니는 UI들을 모아둔 클래스입니다
class GameEngineUIRenderer;
class GameEngineProgressBarRenderer;
class UI_DamagePopUp : public GameEngineActor
{
public:
	UI_DamagePopUp();
	~UI_DamagePopUp();

	void SetFollowInfo(float4 _Pos, CharacterStat* _Stat);
	void SetFollowInfoMonster(float4 _Pos, MonsterStateInfo _Stat);
	
	void SetChar(JobType _Char)
	{
		MyChar = _Char;
	}


public:
	GameEngineUIRenderer* HPBar_Renderer;

	float4 HpBarPos;
	float4 HpBarSize;

	bool UIOn;

	JobType MyChar;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	UI_DamagePopUp(const UI_DamagePopUp& _Other) = delete;
	UI_DamagePopUp(UI_DamagePopUp&& _Other) noexcept = delete;
	UI_DamagePopUp& operator=(const UI_DamagePopUp& _Other) = delete;
	UI_DamagePopUp& operator=(UI_DamagePopUp&& _Other) noexcept = delete;
};

