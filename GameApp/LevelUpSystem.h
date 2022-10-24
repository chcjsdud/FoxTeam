#pragma once
#include "Enums.h"

// 레벨업시스템의 타입
enum class LevelUPSystemType
{
	POWER,
	HP,
	HPRegen,
	SP,
	SPRegen,
	Defence,
	AtkSpeed,
	Critical,
	MoveSpeed,
	EyeSight,
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
	std::map<JobType, std::map<LevelUPSystemType, float>> CharacterLevelUpInfo_;			// 캐릭터직업별 레벨업정보
	std::map<MonsterType, std::map<LevelUPSystemType, float>> MonsterLevelUpInfo_;			// 몬스터타입별 레벨업정보
};

