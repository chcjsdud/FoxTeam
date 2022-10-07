#pragma once
#include <GameEngine/GameEngineActor.h>
// 분류 : 
// 용도 : 로비레벨에 들어갈 UI액터를 모아두는 곳
// 설명 : 


#include "UI_CharPicture.h"
#include "UI_Equip.h"
#include "UI_Inventory.h"
#include "UI_Skill.h"
#include "UI_Status.h"


class PlayerUIController : public GameEngineActor
{
public:
	PlayerUIController(); // default constructer 디폴트 생성자
	~PlayerUIController(); // default destructer 디폴트 소멸자
	PlayerUIController(const PlayerUIController& _other) = delete; // default Copy constructer 디폴트 복사생성자
	PlayerUIController(PlayerUIController&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	PlayerUIController& operator=(const PlayerUIController& _other) = delete; // default Copy operator 디폴트 대입 연산자
	PlayerUIController& operator=(const PlayerUIController&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void InitUI();
	//UI컨트롤러가 내 캐릭터가 뭔지 정보를 가지고있는게 좋을듯

	UI_CharPicture* GetCharPictureUI()
	{
		return charPicture_UI;
	}

	UI_Equip* GetEquipUI()
	{
		return equip_UI;
	}

	UI_Inventory* GetInventoryUI()
	{
		return inventory_UI;
	}

	UI_Skill* GetSkillUI()
	{
		return skill_UI;
	}

	UI_Status* GetStatusUI()
	{
		return status_UI;
	}

	void SetJobType(JobType _MyJob);

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
	
	JobType MyJob;

	bool Rio_bLongBow_;
};

