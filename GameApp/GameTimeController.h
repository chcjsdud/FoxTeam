#pragma once
#include "Enums.h"

// 일차별 낮/밤시간
struct DailyTime
{
	float DayTime_;					// 낮시간
	float NightTime_;				// 밤시간
};

// 몬스터 첫등장에 필요한 정보
struct MonsterFirstAppearInfo
{
	bool IsAppear_;					// 첫등장완료 Flag(true: 첫등장완료)
	DayAndNightType DayType_;		// 첫등장이 낮인지 밤인지 판단
	int AppearDay_;					// 첫등장일차
	float AppearTime_;				// 첫등장시간(젠시간)
};

// 분류 : 컨트롤러
// 용도 : 
// 설명 : 게임내 시간관련 제어가 필요한 대상에 대한 이벤트처리 기능을 제공하며 제어하는 싱글톤 객체
class Monsters;
class GameTimeController
{
public: // Static Value & Function
	static GameTimeController* GetInstance();

public: // Inline Get Function
	inline DayAndNightType GetCurrentDayType() const
	{
		return CurDayOrNight_;
	}

	inline int GetCurrentDay() const
	{
		return CurDay_;
	}

public: // Inline Set Function

public: // Public Get Function
	tm GetCurrentGameTimeToMinute();														// 현재 게임시간을 분/초로 계산하여 반환(tm 구조체로 반환)
	tm GetCurrentGameTimeToHour();															// 현재 게임시간을 시/분/초로 계산하여 반환(tm 구조체로 반환)
	float GetCurrentGameTimeToSec();														// 현재 게임시간을 초로 반환(그대로 반환)
	
public: // Public Set Function
public: // Public Function
	void Initialize();																		// Player Level Update()에서 해당함수를 호출: Controller Initialize
	void Update(float _DeltaTime);															// Player Level Update()에서 해당함수를 호출: Controller Update

protected: // Protected Function
private: // Private Fcuntion

private: // Private Function : GameTime Related Function
	void CreateDailytimes();																// 일차별 낮과밤 시간목록
	void CalcGameTime(float _DeltaTime);													// 게임진행시간에 따른 낮/밤 전환, 일차 전환 등 처리

private: // Private Function : Monster Related Function
	void CreateMonsterFirstAppearInfo();													// 몬스터타입별 첫등장에 필요한 정보 생성
	void UpdateMonsterFirstAppearTime(float _DeltaTime);									// 몬스터타입별 첫등장에 필요한 조건에 따른 첫등장타임 체크
	void FirstAppearMonsters(MonsterType _MonsterType);										// 특정시간에 따른 최초등장해야하는 몬스터 타입을 한번에 등장처리

public:
	GameTimeController();
	~GameTimeController();

protected:
	GameTimeController(const GameTimeController& _other) = delete;
	GameTimeController(GameTimeController&& _other) = delete;

private:
	GameTimeController& operator=(const GameTimeController& _other) = delete;
	GameTimeController& operator=(const GameTimeController&& _other) = delete;

public:
protected:
private:
	float DayAndNightTime_;																	// 낮과밤이 바뀌는 시점(초)
	float CurGameTime_;																		// 현재 게임진행시간(누적) : 낮과밤이 변경되는시점에 초기화
	int PrevDay_;																			// 이전 일차
	int CurDay_;																			// 현재 일차 : 밤->낮으로 넘어가는 시점에 증가
	DayAndNightType CurDayOrNight_;															// 현재 낮인가 밤인가 타입

private: // 게임시간 관련
	std::vector<DailyTime> Dailytimes_;														// 일차별 낮/밤시간

private: // 몬스터첫등장 관련
	bool AllMonsterAppearEnd_;																// 모든 몬스터타입에 관련하여 첫등장 상태 전환완료시 On(true: 모든몬스터첫등장완료)
	MonsterFirstAppearInfo MonsterFirstAppearList_[static_cast<int>(MonsterType::MAX)];		// 몬스터 최초 등장관리(첫등장에만 관여하며 이후 몬스터사망 후 리젠은 각각의 몬스터에서 제어)
};

