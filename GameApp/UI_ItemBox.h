#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// 설명 :
class GameEngineUIRenderer;
class Player;
class UI_ItemBox : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_ItemBox();
	~UI_ItemBox();

	// delete Function
	UI_ItemBox(const UI_ItemBox& _Other) = delete;
	UI_ItemBox(UI_ItemBox&& _Other) noexcept = delete;
	UI_ItemBox& operator=(const UI_ItemBox& _Other) = delete;
	UI_ItemBox& operator=(UI_ItemBox&& _Other) noexcept = delete;

	bool UIOn;

public:
	void SetPlayer(Player* _Player)
	{
		Player_ = _Player;
	}
	
protected:
	GameEngineUIRenderer* ItemBoxBackGround_Renderer;
	
	//박스냐 시체냐 보급이냐에 따라 달라지는 것
	//종류가 몇개 없어서 통 이미지로 대체될 수도 있음
	GameEngineUIRenderer* BoxtypeFont_Renderer;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;

	//이현
private:
	Player* Player_;

#pragma region 아이템 출력용 렌더러 모음

//	GameEngineUIRenderer* DefenseValue_Renderer;
//	GameEngineUIRenderer* AttackSpeedValue_Renderer;
//	GameEngineUIRenderer* MoveSpeedValue_Renderer;
//	GameEngineUIRenderer* CriticalValue_Renderer;

#pragma endregion 

	/*
	참고사항
	아이템 박스의 위치는 항상 고정, 같은 종류의 박스면 항상 고정이다


	받아야 하는 것들

	*아이템 박스를 열었냐 아니냐의 여부

	*아이템 박스에 들어 있는 아이템들의 정보
	-> 정보를 토대로 아이템이 무슨 아이템이 들어있는지 체크

	*아이템 박스의 종류->기본상자/시체/보급
	->시체의 경우 장비칸 8칸+인벤토리 8칸해서 16칸짜리 아이템 정보가 담긴 박스가 나옴
	-> 우선순위 낮음, 일단은 8칸짜리 필드에서 루팅하는 8칸 짜리 박스부터 제작



	들어갈 기능

	* UI_ItemBox의 ItemRemove() 함수
	박스에서 아이템렌더러를 Off한다


	* UI_Inventory의 ItemAttach() 함수
	인벤토리에 아이템 렌더러를 On한다


	

	* Box_to_Inventory() 함수?
	박스에서 아이템이 이동햇을때 
	



	
	*/

};

