#pragma once
#include <GameEngine/GameEngineActor.h>

// Ό³Έν :
class GameEngineUIRenderer;
class TopUI : public GameEngineActor
{
public:
	// constrcuter destructer
	TopUI();
	~TopUI();

	// delete Function
	TopUI(const TopUI& _Other) = delete;
	TopUI(TopUI&& _Other) noexcept = delete;
	TopUI& operator=(const TopUI& _Other) = delete;
	TopUI& operator=(TopUI&& _Other) noexcept = delete;

protected:
	GameEngineUIRenderer* TopRenderer;
	float Time;

private:
	void Start() override;
	void Update(float _Time) override;

};

