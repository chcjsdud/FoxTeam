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

	//이현 추가
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
enum class CharacterType // 캐릭터 타입
{
	NONE = -1,
	AI,
	MAIN,
	MAX
};

enum class JobType // 직업타입
{
	NONE = -1,
	YUKI,		// 유키(기본)
	FIORA,		// 피오라
	ZAHIR,		// 자히르
	NADINE,		// 나딘
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
	DOCK,			// 항구
	POND,			// 연못
	BEACH,			// 모래사장
	UPTOWN,			// 고급 주택가
	ALLEY,			// 골목길
	HOTEL,			// 호텔
	AVENUE,			// 번화가
	HOSPITAL,		// 병원
	TEMPLE,			// 절
	ARCHERY_RANGE,	// 양궁장
	CEMETERY,		// 묘지
	FOREST,			// 숲
	FACTORY,		// 공장
	CHAPEL,			// 성당
	SCHOOL,			// 학교
	RESERCH_CENTRE,	// 연구소
	ESCAPE_DOCK,	// 오래된 선창
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