#pragma once

enum class CollisionGroup
{
	None = 0,
	MousePointer,
	Player,
	PlayerAttack,
	MonsterSight,
	Monster,
	MonsterAttack,
	PlayerSight,
	Item,
	UI,


	MAX = 999

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

