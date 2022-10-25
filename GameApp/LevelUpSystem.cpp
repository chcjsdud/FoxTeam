#include "Precompile.h"
#include "LevelUpSystem.h"

#include <iostream>
#include <fstream>

LevelUpSystem* LevelUpSystem::GetInstance()
{
	static LevelUpSystem instance;
	return &instance;
}

void LevelUpSystem::LoadLevelInfomationFile()
{
	GameEngineDirectory LevelFilesDir;
	LevelFilesDir.MoveParent("FoxTeam");
	LevelFilesDir.MoveChild("Resources");
	LevelFilesDir.MoveChild("CSVFiles");

	AllFiles_ = LevelFilesDir.GetAllFile();

	int FileCount = static_cast<int>(AllFiles_.size());
	for (int FileNum = 0; FileNum < FileCount; ++FileNum)
	{
		// 확인용
		std::string CurFileName = AllFiles_[FileNum].GetFileName();
		std::string CurFilePath = AllFiles_[FileNum].GetFullPath();

		// 00. File Open
		std::ifstream fs(CurFilePath);
		if (true == fs.fail())
		{
			GameEngineDebug::MsgBoxError(CurFileName + "파일읽기에 실패하였습니다!!!!!");
			continue;
		}

		// 00-1. 첫줄은 구분자이기때문에 무시하기위해 미리 getline으로 읽어 파일의 커서를 이동
		std::string FirstLine;
		std::getline(fs, FirstLine);

		// 01. File Line & Cell Read -> Create List
		int LineCount = 0;
		std::map<int, std::vector<std::string>> LineAndCellList;

		std::string Line;
		while (std::getline(fs, Line))
		{
			std::string Cell;
			std::stringstream LineStrean(Line);
			while (std::getline(LineStrean, Cell, ','))
			{
				LineAndCellList[LineCount].push_back(Cell);
			}

			++LineCount;
		}

		// 02. File Close
		fs.close();

		// 03. 정보 저장
		// 단, 몬스터 or 캐릭터를 구분하여 목록작성
		int LineAndCellListCount = static_cast<int>(LineAndCellList.size());
		for (int LineNum = 0; LineNum < LineAndCellListCount; ++LineNum)
		{
			// 0001. 캐릭터정보라면 JobType, 몬스터정보라면 MonsterType
			if (std::string::npos != CurFileName.find("character_"))
			{
				// 캐릭터 타입
				JobType CurJobType = JobType::NONE;
				int JobTypeInt = -1;
				std::stringstream TypeString(LineAndCellList[LineNum][0]);
				TypeString >> JobTypeInt;
				CurJobType = static_cast<JobType>(JobTypeInt);

				// 캐릭터명
				CharacterLevelData_[CurJobType].TypeName_ = LineAndCellList[LineNum][1];

				// 공격력
				float AtkPower = 0.0f;
				std::stringstream AtkPowerString(LineAndCellList[LineNum][2]);
				AtkPowerString >> AtkPower;
				CharacterLevelData_[CurJobType].AttackPower_ = AtkPower;

				// 체력
				float HP = 0.0f;
				std::stringstream HPString(LineAndCellList[LineNum][3]);
				HPString >> HP;
				CharacterLevelData_[CurJobType].HP_ = HP;

				// 체력재생
				float HPRegeneration = 0.0f;
				std::stringstream HPRegenerationString(LineAndCellList[LineNum][4]);
				HPRegenerationString >> HPRegeneration;
				CharacterLevelData_[CurJobType].HPRegeneration_ = HPRegeneration;

				// 스태미나
				float SP = 0.0f;
				std::stringstream SPString(LineAndCellList[LineNum][5]);
				SPString >> SP;
				CharacterLevelData_[CurJobType].SP_ = SP;

				// 스태미나재생
				float SPRegeneration = 0.0f;
				std::stringstream SPRegenerationString(LineAndCellList[LineNum][6]);
				SPRegenerationString >> SPRegeneration;
				CharacterLevelData_[CurJobType].SPRegeneration_ = SPRegeneration;

				// 방어력
				float Defence = 0.0f;
				std::stringstream DefenceString(LineAndCellList[LineNum][7]);
				DefenceString >> Defence;
				CharacterLevelData_[CurJobType].Defence_ = Defence;

				// 공격속도
				float AttackSpeed = 0.0f;
				std::stringstream AttackSpeedString(LineAndCellList[LineNum][8]);
				AttackSpeedString >> AttackSpeed;
				CharacterLevelData_[CurJobType].AttackSpeed_ = AttackSpeed;

				// 치명타
				float CriticalChance = 0.0f;
				std::stringstream CriticalChanceString(LineAndCellList[LineNum][9]);
				CriticalChanceString >> CriticalChance;
				CharacterLevelData_[CurJobType].CriticalChance_ = CriticalChance;

				// 이동속도
				float MovementSpeed = 0.0f;
				std::stringstream MovementSpeedString(LineAndCellList[LineNum][10]);
				MovementSpeedString >> MovementSpeed;
				CharacterLevelData_[CurJobType].MovementSpeed_ = MovementSpeed;

				// 시야
				float VisionRange = 0.0f;
				std::stringstream VisionRangeString(LineAndCellList[LineNum][11]);
				VisionRangeString >> VisionRange;
				CharacterLevelData_[CurJobType].VisionRange_ = VisionRange;
			}
			else if (std::string::npos != CurFileName.find("monster_"))
			{
				// 몬스터 타입
				MonsterType CurMonsterType = MonsterType::NONE;
				int MonsterTypeInt = -1;
				std::stringstream MonsterTypeString(LineAndCellList[LineNum][0]);
				MonsterTypeString >> MonsterTypeInt;
				CurMonsterType = static_cast<MonsterType>(MonsterTypeInt);

				// 캐릭터명
				MonsterLevelData_[CurMonsterType].TypeName_ = LineAndCellList[LineNum][1];

				// 공격력
				float AtkPower = 0.0f;
				std::stringstream AtkPowerString(LineAndCellList[LineNum][2]);
				AtkPowerString >> AtkPower;
				MonsterLevelData_[CurMonsterType].AttackPower_ = AtkPower;

				// 체력
				float HP = 0.0f;
				std::stringstream HPString(LineAndCellList[LineNum][3]);
				HPString >> HP;
				MonsterLevelData_[CurMonsterType].HP_ = HP;

				// 체력재생
				float HPRegeneration = 0.0f;
				std::stringstream HPRegenerationString(LineAndCellList[LineNum][4]);
				HPRegenerationString >> HPRegeneration;
				MonsterLevelData_[CurMonsterType].HPRegeneration_ = HPRegeneration;

				// 스태미나
				float SP = 0.0f;
				std::stringstream SPString(LineAndCellList[LineNum][5]);
				SPString >> SP;
				MonsterLevelData_[CurMonsterType].SP_ = SP;

				// 스태미나재생
				float SPRegeneration = 0.0f;
				std::stringstream SPRegenerationString(LineAndCellList[LineNum][6]);
				SPRegenerationString >> SPRegeneration;
				MonsterLevelData_[CurMonsterType].SPRegeneration_ = SPRegeneration;

				// 방어력
				float Defence = 0.0f;
				std::stringstream DefenceString(LineAndCellList[LineNum][7]);
				DefenceString >> Defence;
				MonsterLevelData_[CurMonsterType].Defence_ = Defence;

				// 공격속도
				float AttackSpeed = 0.0f;
				std::stringstream AttackSpeedString(LineAndCellList[LineNum][8]);
				AttackSpeedString >> AttackSpeed;
				MonsterLevelData_[CurMonsterType].AttackSpeed_ = AttackSpeed;

				// 치명타
				float CriticalChance = 0.0f;
				std::stringstream CriticalChanceString(LineAndCellList[LineNum][9]);
				CriticalChanceString >> CriticalChance;
				MonsterLevelData_[CurMonsterType].CriticalChance_ = CriticalChance;

				// 이동속도
				float MovementSpeed = 0.0f;
				std::stringstream MovementSpeedString(LineAndCellList[LineNum][10]);
				MovementSpeedString >> MovementSpeed;
				MonsterLevelData_[CurMonsterType].MovementSpeed_ = MovementSpeed;

				// 시야
				float VisionRange = 0.0f;
				std::stringstream VisionRangeString(LineAndCellList[LineNum][11]);
				VisionRangeString >> VisionRange;
				MonsterLevelData_[CurMonsterType].VisionRange_ = VisionRange;
			}
		}
	}
}

void LevelUpSystem::AllUnitLevelUP()
{
	// 현재 게임에 배치된 모든 유닛에게 레벨업 명령을 내린다!!!




}

LevelUpSystem::LevelUpSystem()
{
}

LevelUpSystem::~LevelUpSystem()
{
}
