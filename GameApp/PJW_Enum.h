#pragma once


// 인 게임 내 콜리젼의 속성 (스킬인지, 몸통인지, 마우스 포인터인지...) 를 나타내는 ENUM 값.

enum InGameCollisionType
{
	MIN = 0,
	MousePointer,
	Map,

	Player1_Body,
	Player1_AttackRange,

	Player2_Body,
	Player2_AttackRange,

	MAX = 999
};