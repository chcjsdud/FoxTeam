#pragma once

enum class RenderOrder
{
	BACKDROP,
	MAP,
	MONSTER,
	PLAYER,
	UIPANEL0,
	UIPANEL1,
	UIPANEL2,
	UI,
	UIICON,
	UICOL,
	MOUSE,
	MAX
};

// UI 버튼 상태
enum class ButtonState
{
	Normal,				// 기본상태
	MouseOver,			// 마우스오버상태
	Click,				// 마우스클릭상태
	Disabled,			// 비활성화상태
	Max
};

// UI ProgressBar Direct
enum class ProgressBarDirect
{
	BottomToTop,		// Bottom -> Top
	TopToBottom,		// Top -> Bottom
	RightToLeft,		// Right -> Left
	LeftToRight			// Left -> Right
};

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