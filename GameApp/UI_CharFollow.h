#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;

//캐릭터를 쫓아다니는 UI들을 모아둔 클래스입니다
class GameEngineUIRenderer;
class UI_CharFollow : public GameEngineActor
{
public:
	UI_CharFollow();
	~UI_CharFollow();


public:
	GameEngineUIRenderer* HPBarRenderer;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	UI_CharFollow(const UI_CharFollow& _Other) = delete;
	UI_CharFollow(UI_CharFollow&& _Other) noexcept = delete;
	UI_CharFollow& operator=(const UI_CharFollow& _Other) = delete;
	UI_CharFollow& operator=(UI_CharFollow&& _Other) noexcept = delete;
};

