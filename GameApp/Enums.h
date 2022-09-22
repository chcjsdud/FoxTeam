#pragma once

// Fox Team
namespace FT
{
	namespace Char
	{
		const float MOVE_FINISH_CHECK_DISTANCE = 10.f;

		const float DEFAULT_CRITICAL_DAMAGE_AMPLIFICATION = 1.65f;
		const float DEFAULT_VISION_RANGE_DAY = 815.f;
		const float DEFAULT_VISION_RANGE_NIGHT = 570.f;

	};

	namespace Map
	{
		const float MAX_HEIGHT = 1000.f;
	};
};

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
	MouseRay,

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