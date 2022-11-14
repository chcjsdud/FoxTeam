#pragma once
#include <GameEngine/GameEngineActor.h>
#include "CharacterStat.h"
#include "MonsterStateInfo.h"

using namespace std;

//캐릭터를 쫓아다니는 UI들을 모아둔 클래스입니다
class GameEngineUIRenderer;
class UI_DamagePopUp : public GameEngineActor
{
public:
	UI_DamagePopUp();
	~UI_DamagePopUp();
	
	void SetChar(JobType _Char)
	{
		MyChar = _Char;
	}


public:
	GameEngineUIRenderer* HPBar_Renderer;

	float4 HpBarPos;
	float4 HpBarSize;

	float Time_;

	bool UIOn;

	JobType MyChar;

	void DamageFontAppear(string _DamageValue, float4 _Pos);
	void DamageFontAppearMonster(string _DamageValue, float4 _Pos);

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	UI_DamagePopUp(const UI_DamagePopUp& _Other) = delete;
	UI_DamagePopUp(UI_DamagePopUp&& _Other) noexcept = delete;
	UI_DamagePopUp& operator=(const UI_DamagePopUp& _Other) = delete;
	UI_DamagePopUp& operator=(UI_DamagePopUp&& _Other) noexcept = delete;
};

