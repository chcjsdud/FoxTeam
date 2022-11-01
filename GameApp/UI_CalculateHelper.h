#pragma once
#include "GameEngine/GameEngineUIRenderer.h"

using namespace std;

//

class UI_CalculateHelper
{

public:
	UI_CalculateHelper();
	~UI_CalculateHelper();

public:


private:
	UI_CalculateHelper(const UI_CalculateHelper& _Other) = delete;
	UI_CalculateHelper(UI_CalculateHelper&& _Other) noexcept = delete;
	UI_CalculateHelper& operator=(const UI_CalculateHelper& _Other) = delete;
	UI_CalculateHelper& operator=(UI_CalculateHelper&& _Other) noexcept = delete;
};

