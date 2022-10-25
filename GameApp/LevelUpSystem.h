#pragma once
#include "Enums.h"

// 분류 : 데이터
// 용도 : 
// 설명 : 각 유닛 타입별 레벨업 정보를 정의
struct LevelData
{
	std::string TypeName_;						// 각 유닛의 고유명(Ex. Yuki, Rio, ...)
	float AttackPower_;							// 공격력
	float HP_;									// 체력
	float HPRegeneration_;						// 체력 재생
	float SP_;									// 스태미나
	float SPRegeneration_;						// 스태미나 재생
	float Defence_;								// 방어력
	float AttackSpeed_;							// 공격속도
	float CriticalChance_;						// 치명타
	float MovementSpeed_;						// 이동속도
	float VisionRange_;							// 시야
};

// 분류 : 시스템
// 용도 : 
// 설명 : 게임시간에 따른 레벨업 처리기능 제공
class LevelUpSystem
{
public: // Static Value & Function
	static LevelUpSystem* GetInstance();

public: // 레벨업관련 파일 로드
	void LoadLevelInfomationFile();

public: // 일괄처리
	void AllUnitLevelUP();

protected:
private:

public:
	LevelUpSystem();
	~LevelUpSystem();

protected:
	LevelUpSystem(const LevelUpSystem& _other) = delete;
	LevelUpSystem(LevelUpSystem&& _other) = delete;

private:
	LevelUpSystem& operator=(const LevelUpSystem& _other) = delete;
	LevelUpSystem& operator=(const LevelUpSystem&& _other) = delete;

public:
protected:
private:
	std::vector<GameEngineFile> AllFiles_;													// 지정된 경로의 모든 파일목록

private:
	std::map<JobType, LevelData> CharacterLevelData_;										// 캐릭터들의 레벨업 정보목록
	std::map<MonsterType, LevelData> MonsterLevelData_;										// 몬스터들의 레벨업 정보목록
};

