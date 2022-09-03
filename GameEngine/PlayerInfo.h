#pragma once

/// <summary>
// 서버 인터페이스가 벡터 값으로 들고 있을 캐릭터 슬롯입니다.
// 각 패킷들의 변수는 Execute() 에서 해당 PlayerInfo 의 값들을 채워 주는 식으로 운용할 예정입니다.
/// </summary>
struct PlayerInfo
{
	int character_;
	int startPoint_;
	bool isReady_;
};