#pragma once
#include <GameEngine/GameEngineActor.h>
// �з� : 
// �뵵 : �κ񷹺��� �� UI���͸� ��Ƶδ� ��
// ���� : 


class UI_CharPicture;
class UI_Equip;
class UI_Inventory;
class UI_ItemBox;
class UI_Skill;
class UI_Status;


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

