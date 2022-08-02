#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// 설명 :
class GameEngineUIRenderer;
class Player;
class UI_CharPicture : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_CharPicture();
	~UI_CharPicture();

	// delete Function
	UI_CharPicture(const UI_CharPicture& _Other) = delete;
	UI_CharPicture(UI_CharPicture&& _Other) noexcept = delete;
	UI_CharPicture& operator=(const UI_CharPicture& _Other) = delete;
	UI_CharPicture& operator=(UI_CharPicture&& _Other) noexcept = delete;

	bool UIOn;

public:
	void SetPlayer(Player* _Player)
	{
		Player_ = _Player;
	}

protected:
	GameEngineUIRenderer* Renderer;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;

	//이현
private:
	Player* Player_;

	
};

