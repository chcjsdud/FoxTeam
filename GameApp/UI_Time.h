#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;

class UI_Time : public GameEngineActor
{
public:
	UI_Time();
	~UI_Time();

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	UI_Time(const UI_Time& _Other) = delete;
	UI_Time(UI_Time&& _Other) noexcept = delete;
	UI_Time& operator=(const UI_Time& _Other) = delete;
	UI_Time& operator=(UI_Time&& _Other) noexcept = delete;
};

