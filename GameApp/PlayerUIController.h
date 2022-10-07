#pragma once
#include <GameEngine/GameEngineActor.h>
// �з� : 
// �뵵 : �κ񷹺��� �� UI���͸� ��Ƶδ� ��
// ���� : 


#include "UI_CharPicture.h"
#include "UI_Equip.h"
#include "UI_Inventory.h"
#include "UI_Skill.h"
#include "UI_Status.h"


class PlayerUIController : public GameEngineActor
{
public:
	PlayerUIController(); // default constructer ����Ʈ ������
	~PlayerUIController(); // default destructer ����Ʈ �Ҹ���
	PlayerUIController(const PlayerUIController& _other) = delete; // default Copy constructer ����Ʈ ���������
	PlayerUIController(PlayerUIController&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	PlayerUIController& operator=(const PlayerUIController& _other) = delete; // default Copy operator ����Ʈ ���� ������
	PlayerUIController& operator=(const PlayerUIController&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void InitUI();
	//UI��Ʈ�ѷ��� �� ĳ���Ͱ� ���� ������ �������ִ°� ������

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

