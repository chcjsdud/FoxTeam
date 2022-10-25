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
		// Ȯ�ο�
		std::string CurFileName = AllFiles_[FileNum].GetFileName();
		std::string CurFilePath = AllFiles_[FileNum].GetFullPath();

		// 00. File Open
		std::ifstream fs(CurFilePath);
		if (true == fs.fail())
		{
			GameEngineDebug::MsgBoxError(CurFileName + "�����б⿡ �����Ͽ����ϴ�!!!!!");
			continue;
		}

		// 00-1. ù���� �������̱⶧���� �����ϱ����� �̸� getline���� �о� ������ Ŀ���� �̵�
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

		// 03. ���� ����
		// ��, ���� or ĳ���͸� �����Ͽ� ����ۼ�
		int LineAndCellListCount = static_cast<int>(LineAndCellList.size());
		for (int LineNum = 0; LineNum < LineAndCellListCount; ++LineNum)
		{
			// 0001. ĳ����������� JobType, ����������� MonsterType
			if (std::string::npos != CurFileName.find("character_"))
			{
				// ĳ���� Ÿ��
				JobType CurJobType = JobType::NONE;
				int JobTypeInt = -1;
				std::stringstream TypeString(LineAndCellList[LineNum][0]);
				TypeString >> JobTypeInt;
				CurJobType = static_cast<JobType>(JobTypeInt);

				// ĳ���͸�
				CharacterLevelData_[CurJobType].TypeName_ = LineAndCellList[LineNum][1];

				// ���ݷ�
				float AtkPower = 0.0f;
				std::stringstream AtkPowerString(LineAndCellList[LineNum][2]);
				AtkPowerString >> AtkPower;
				CharacterLevelData_[CurJobType].AttackPower_ = AtkPower;

				// ü��
				float HP = 0.0f;
				std::stringstream HPString(LineAndCellList[LineNum][3]);
				HPString >> HP;
				CharacterLevelData_[CurJobType].HP_ = HP;

				// ü�����
				float HPRegeneration = 0.0f;
				std::stringstream HPRegenerationString(LineAndCellList[LineNum][4]);
				HPRegenerationString >> HPRegeneration;
				CharacterLevelData_[CurJobType].HPRegeneration_ = HPRegeneration;

				// ���¹̳�
				float SP = 0.0f;
				std::stringstream SPString(LineAndCellList[LineNum][5]);
				SPString >> SP;
				CharacterLevelData_[CurJobType].SP_ = SP;

				// ���¹̳����
				float SPRegeneration = 0.0f;
				std::stringstream SPRegenerationString(LineAndCellList[LineNum][6]);
				SPRegenerationString >> SPRegeneration;
				CharacterLevelData_[CurJobType].SPRegeneration_ = SPRegeneration;

				// ����
				float Defence = 0.0f;
				std::stringstream DefenceString(LineAndCellList[LineNum][7]);
				DefenceString >> Defence;
				CharacterLevelData_[CurJobType].Defence_ = Defence;

				// ���ݼӵ�
				float AttackSpeed = 0.0f;
				std::stringstream AttackSpeedString(LineAndCellList[LineNum][8]);
				AttackSpeedString >> AttackSpeed;
				CharacterLevelData_[CurJobType].AttackSpeed_ = AttackSpeed;

				// ġ��Ÿ
				float CriticalChance = 0.0f;
				std::stringstream CriticalChanceString(LineAndCellList[LineNum][9]);
				CriticalChanceString >> CriticalChance;
				CharacterLevelData_[CurJobType].CriticalChance_ = CriticalChance;

				// �̵��ӵ�
				float MovementSpeed = 0.0f;
				std::stringstream MovementSpeedString(LineAndCellList[LineNum][10]);
				MovementSpeedString >> MovementSpeed;
				CharacterLevelData_[CurJobType].MovementSpeed_ = MovementSpeed;

				// �þ�
				float VisionRange = 0.0f;
				std::stringstream VisionRangeString(LineAndCellList[LineNum][11]);
				VisionRangeString >> VisionRange;
				CharacterLevelData_[CurJobType].VisionRange_ = VisionRange;
			}
			else if (std::string::npos != CurFileName.find("monster_"))
			{
				// ���� Ÿ��
				MonsterType CurMonsterType = MonsterType::NONE;
				int MonsterTypeInt = -1;
				std::stringstream MonsterTypeString(LineAndCellList[LineNum][0]);
				MonsterTypeString >> MonsterTypeInt;
				CurMonsterType = static_cast<MonsterType>(MonsterTypeInt);

				// ĳ���͸�
				MonsterLevelData_[CurMonsterType].TypeName_ = LineAndCellList[LineNum][1];

				// ���ݷ�
				float AtkPower = 0.0f;
				std::stringstream AtkPowerString(LineAndCellList[LineNum][2]);
				AtkPowerString >> AtkPower;
				MonsterLevelData_[CurMonsterType].AttackPower_ = AtkPower;

				// ü��
				float HP = 0.0f;
				std::stringstream HPString(LineAndCellList[LineNum][3]);
				HPString >> HP;
				MonsterLevelData_[CurMonsterType].HP_ = HP;

				// ü�����
				float HPRegeneration = 0.0f;
				std::stringstream HPRegenerationString(LineAndCellList[LineNum][4]);
				HPRegenerationString >> HPRegeneration;
				MonsterLevelData_[CurMonsterType].HPRegeneration_ = HPRegeneration;

				// ���¹̳�
				float SP = 0.0f;
				std::stringstream SPString(LineAndCellList[LineNum][5]);
				SPString >> SP;
				MonsterLevelData_[CurMonsterType].SP_ = SP;

				// ���¹̳����
				float SPRegeneration = 0.0f;
				std::stringstream SPRegenerationString(LineAndCellList[LineNum][6]);
				SPRegenerationString >> SPRegeneration;
				MonsterLevelData_[CurMonsterType].SPRegeneration_ = SPRegeneration;

				// ����
				float Defence = 0.0f;
				std::stringstream DefenceString(LineAndCellList[LineNum][7]);
				DefenceString >> Defence;
				MonsterLevelData_[CurMonsterType].Defence_ = Defence;

				// ���ݼӵ�
				float AttackSpeed = 0.0f;
				std::stringstream AttackSpeedString(LineAndCellList[LineNum][8]);
				AttackSpeedString >> AttackSpeed;
				MonsterLevelData_[CurMonsterType].AttackSpeed_ = AttackSpeed;

				// ġ��Ÿ
				float CriticalChance = 0.0f;
				std::stringstream CriticalChanceString(LineAndCellList[LineNum][9]);
				CriticalChanceString >> CriticalChance;
				MonsterLevelData_[CurMonsterType].CriticalChance_ = CriticalChance;

				// �̵��ӵ�
				float MovementSpeed = 0.0f;
				std::stringstream MovementSpeedString(LineAndCellList[LineNum][10]);
				MovementSpeedString >> MovementSpeed;
				MonsterLevelData_[CurMonsterType].MovementSpeed_ = MovementSpeed;

				// �þ�
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
	// ���� ���ӿ� ��ġ�� ��� ���ֿ��� ������ ����� ������!!!




}

LevelUpSystem::LevelUpSystem()
{
}

LevelUpSystem::~LevelUpSystem()
{
}
