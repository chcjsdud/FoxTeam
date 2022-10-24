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

		// ĳ���������ΰ��
		if (std::string::npos != CurFileName.find("character_"))
		{



			int a = 0;
		}
		// ���������ΰ��
		else if (std::string::npos != CurFileName.find("monster_"))
		{




			int a = 0;
		}
		else
		{
			GameEngineDebug::MsgBoxError(CurFilePath + "�߸��� Ÿ���� ���������� �ε��Ϸ��õ��߽��ϴ�!!!!");
			continue;
		}

		//// File Open & Read
		//std::ifstream fs(CurFilePath);
		//if (true == fs.fail())
		//{
		//	GameEngineDebug::MsgBoxError(CurFileName + "�����б⿡ �����Ͽ����ϴ�!!!!!");
		//	continue;
		//}

		//// ù��° �� �б� : �������� �����ڰ� �������
		//std::string GubunCell;
		//std::string GubunLine;
		//std::getline(fs, GubunLine, '\n');
		//std::stringstream LineStream(GubunLine);
		//while (std::getline(LineStream, GubunCell, ','))
		//{
		//	AllFileData_[CurFileName][GubunCell];
		//}

		//int GubunCellCount = static_cast<int>(AllFileData_[CurFileName].size());
		//for (int CellNum = 0; CellNum < GubunCellCount; ++CellNum)
		//{
		//	std::string CellName = AllFileData_[CurFileName].at();

		//	std::string Cell;
		//	std::string Line;
		//	while (std::getline(fs, Line, '\n'))
		//	{
		//		std::stringstream LineStream(Line);
		//		while (std::getline(LineStream, Cell, ','))
		//		{
		//			AllFileData_[CurFileName][LineName].push_back(Cell);
		//		}
		//	}
		//}

		//// File Close
		//fs.close();
	}
}

LevelUpSystem::LevelUpSystem()
{
}

LevelUpSystem::~LevelUpSystem()
{
}
