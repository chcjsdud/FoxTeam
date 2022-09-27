#pragma once

enum class eCollisionGroup
{
	None = 0,
	MousePointer,
	Player,
	PlayerGround,
	PlayerSight,
	PlayerAttack,
	MonsterSight,
	Monster,
	MonsterAttack,
	Item,
	ItemBox,
	UI,

	Unit,
	MouseRay,

	MAX = 999
};

enum class JobType // ����Ÿ��
{
	NONE = -1,
	YUKI,		// ��Ű(�⺻)
	FIORA,		// �ǿ���
	ZAHIR,		// ������
	NADINE,		// ����
	HYUNWOO,
	JACKIE,
	RIO,
	AYA,
	// ....
	MAX
};


enum class Location
{
	NONE = -1,
	DOCK,			// �ױ�
	POND,			// ����
	BEACH,			// �𷡻���
	UPTOWN,			// ��� ���ð�
	ALLEY,			// ����
	HOTEL,			// ȣ��
	AVENUE,			// ��ȭ��
	HOSPITAL,		// ����
	TEMPLE,			// ��
	ARCHERY_RANGE,	// �����
	CEMETERY,		// ����
	FOREST,			// ��
	FACTORY,		// ����
	CHAPEL,			// ����
	SCHOOL,			// �б�
	RESERCH_CENTRE,	// ������
	ESCAPE_DOCK,	// ������ ��â
	MAX
};

#pragma endregion


enum class ItemType
{
	NONE = -1,
	EQUIPMENT,
	USEABLE,
	MISC,
	MAX
};

enum class EquipmentType
{
	NONE = -1,
	WEAPON,
	HEAD,
	CHEST,
	ARM,
	LEG,
	ACCESSORY,
	MAX
};