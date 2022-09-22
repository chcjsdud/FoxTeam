#pragma once
#include <GameEngine/GameEngineActor.h>
// 분류 : 
// 용도 : 로비레벨에 들어갈 UI액터를 모아두는 곳
// 설명 : 


class UI_CharPicture;
class UI_Equip;
class UI_Inventory;
class UI_ItemBox;
class UI_Skill;
class UI_Status;


class LumiaUIController : public GameEngineActor
{
public:
	LumiaUIController(); // default constructer 디폴트 생성자
	~LumiaUIController(); // default destructer 디폴트 소멸자
	LumiaUIController(const LumiaUIController& _other) = delete; // default Copy constructer 디폴트 복사생성자
	LumiaUIController(LumiaUIController&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	LumiaUIController& operator=(const LumiaUIController& _other) = delete; // default Copy operator 디폴트 대입 연산자
	LumiaUIController& operator=(const LumiaUIController&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void InitUI();
	//UI컨트롤러가 내 캐릭터가 뭔지 정보를 가지고있는게 좋을듯

public:


protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	UI_CharPicture* charPicture_UI;
	UI_Equip* equip_UI;
	UI_Inventory* inventory_UI;
	UI_Skill* skill_UI;
	UI_Status* status_UI;

};

