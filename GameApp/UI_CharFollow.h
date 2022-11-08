#pragma once
#include <GameEngine/GameEngineActor.h>
#include "CharacterStat.h"
#include "MonsterStateInfo.h"

using namespace std;

//ĳ���͸� �Ѿƴٴϴ� UI���� ��Ƶ� Ŭ�����Դϴ�
class GameEngineUIRenderer;
class GameEngineProgressBarRenderer;
class UI_CharFollow : public GameEngineActor
{
public:
	UI_CharFollow();
	~UI_CharFollow();

	void SetFollowInfo(float4 _Pos, CharacterStat* _Stat);
	void SetFollowInfoMonster(float4 _Pos, MonsterStateInfo _Stat);


public:
	GameEngineProgressBarRenderer* HPBar_Renderer;
	GameEngineProgressBarRenderer* SPBar_Renderer;
	GameEngineProgressBarRenderer* EmptyBar_Renderer;

	float4 HpBarPos;
	float4 HpBarSize;

	float4 SpBarPos;
	float4 SpBarSize;

	float4 EmptyBarPos;
	float4 EmptyBarSize;

	bool UIOn;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	UI_CharFollow(const UI_CharFollow& _Other) = delete;
	UI_CharFollow(UI_CharFollow&& _Other) noexcept = delete;
	UI_CharFollow& operator=(const UI_CharFollow& _Other) = delete;
	UI_CharFollow& operator=(UI_CharFollow&& _Other) noexcept = delete;
};

