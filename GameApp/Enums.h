#pragma once

enum class CollisionGroup
{
	None = 0,
	Player,
	PlayerAttack,
	MonsterSight,
	Monster,
	MonsterAttack,
	PlayerSight,
	Item,


	MAX = 999

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
				// ....
	MAX
};

#pragma endregion

