#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;

//ĳ���͸� �Ѿƴٴϴ� UI���� ��Ƶ� Ŭ�����Դϴ�
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

