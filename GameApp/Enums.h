#pragma once

enum class CollisionGroup
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

	//���� �߰�
	Unit,


	MAX = 999

};

enum class ObjectType
{
	Player_,
	Monster_,
	Item_


};

#pragma region SJH
enum class CharacterType // ĳ���� Ÿ��
{
	NONE = -1,
	AI,
	MAIN,
	MAX
};

enum class JobType // ����Ÿ��
{
	NONE = -1,
	YUKI,		// ��Ű(�⺻)
	FIORA,		// �ǿ���
	ZAHIR,		// ������
	NADINE,		// ����
				// ....
				MAX
};

#pragma endregion


enum class ItemType
{
	None,
	Equipment,
	Useable,
	Misc
};

enum class BuffType
{
	None,
	SomeBuff1,
	SomeBuff2,
	SomeBuff3,
	SomeBuff4,
	SomeBuff5,
	SomeBuff6,
	SomeBuff7,
	SomeBuff8,
	Max
};

//enum class AnimationType
//{
//	None,
//	Idle,
//	Attack,
//	Walk,
//	SKill1,
//	SKill2,
//	SKill3,
//	SKill4,
//	Reat,
//	Death,
//};