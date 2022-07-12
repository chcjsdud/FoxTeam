#pragma once
#include "GameEngineGUI.h"
#include "GameEngineRenderTarget.h"
#include <unordered_map>

// Ό³Έν :
class GameEngineLevelControlWindow : public GameEngineGUIWindow
{
public:
	// constrcuter destructer
	GameEngineLevelControlWindow();
	~GameEngineLevelControlWindow();

	// delete Function
	GameEngineLevelControlWindow(const GameEngineLevelControlWindow& _Other) = delete;
	GameEngineLevelControlWindow(GameEngineLevelControlWindow&& _Other) noexcept = delete;
	GameEngineLevelControlWindow& operator=(const GameEngineLevelControlWindow& _Other) = delete;
	GameEngineLevelControlWindow& operator=(GameEngineLevelControlWindow&& _Other) noexcept = delete;

protected:
	void OnGUI() override;


private:

};

