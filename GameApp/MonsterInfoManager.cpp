#include "PreCompile.h"
#include "MonsterInfoManager.h"

#include <GameEngineBase/GameEngineRandom.h>

#include "PlayerInfoManager.h"
#include "MonsterInfoManager.h"

#include "GameServer.h"
#include "GameClient.h"
#include "CreationCommandPacket.h"

bool MonsterInfoManager::FirstCreationPacketFlag = false;
bool MonsterInfoManager::SecondCreationPacketFlag = false;

MonsterInfoManager* MonsterInfoManager::GetInstance()
{
	static MonsterInfoManager instance;
	return &instance;
}

void MonsterInfoManager::AddMonsterInfo(MonsterInfo _MonsterInfo)
{
	AllMonsters_.push_back(_MonsterInfo);
}

void MonsterInfoManager::SetMonsterInfos(std::vector<MonsterInfo> _MonsterInfos)
{
	AllMonsters_ = _MonsterInfos;
}

bool MonsterInfoManager::CreatMonsterInfomation()
{
	GameEngineDirectory SpawnMonsterInfoFilePath;
	SpawnMonsterInfoFilePath.MoveParent("FoxTeam");
	SpawnMonsterInfoFilePath.MoveChild("Resources");
	SpawnMonsterInfoFilePath.MoveChild("SaveFiles");

	std::string FullPath = SpawnMonsterInfoFilePath.PathToPlusFileName("SpawnMonsterList.save");

	// ������ ����� ������ �����ϴ��� üũ
	std::filesystem::path FileSysFullPath = FullPath;
	GameEngineFile SaveFile(FileSysFullPath);

	// ����1: �����������Ѵٸ� ���������� ����ִ� ������Ŷ ����
	if (true == SaveFile.IsExist())
	{
		// ���������� ���� ���Ϸε� �� ���� ������ ����ִ� ������Ŷ����
		LoadMonsterInfoFile(FullPath);
	}
	// ����2: ������ ���������ʴٸ� ���ο� ������ ������ ���������� ��� ���� ������Ŷ����
	// -> ��Ŷ���۽� ���ѷε��ɸ��� ���߻�!!!!!!!!!!!!!!!!!!!
	else
	{
		// ������ ���۷��� ���� ���� �� ������Ŷ���� ����, ������Ŷ����
		LoadSpawnPointMeshByRegion(FullPath);
	}

	return true;
}

void MonsterInfoManager::LoadMonsterInfoFile()
{
	// ���Ϸε� ���� �� ��������
	GameEngineDirectory SpawnMonsterInfoFilePath;
	SpawnMonsterInfoFilePath.MoveParent("FoxTeam");
	SpawnMonsterInfoFilePath.MoveChild("Resources");
	SpawnMonsterInfoFilePath.MoveChild("SaveFiles");
	std::string FullPath = SpawnMonsterInfoFilePath.PathToPlusFileName("SpawnMonsterList.save");

	// �ش� ����� ���� �ε�
	GameEngineFile LoadFile(FullPath, "rb");

	// ���� �Ѱ���
	int TotMonsterCount = 0;
	LoadFile.Read(TotMonsterCount);
	for (int MonsterNum = 0; MonsterNum < TotMonsterCount; ++MonsterNum)
	{
		MonsterInfo NewMonsterInfo = {};

		// �����ε���
		LoadFile.Read(NewMonsterInfo.Index_);

		// ����Ÿ��
		int ReadRegion = 0;
		LoadFile.Read(ReadRegion);
		NewMonsterInfo.RegionType_ = static_cast<Location>(ReadRegion);

		// ����Ÿ��
		int ReadType = 0;
		LoadFile.Read(ReadType);
		NewMonsterInfo.MonsterType_ = static_cast<MonsterType>(ReadType);

		// ������ġ
		LoadFile.Read(NewMonsterInfo.SpawnPosition_);

		AllMonsters_.push_back(NewMonsterInfo);
	}

	LoadFile.Close();
}

void MonsterInfoManager::SaveMonsterInfoFile()
{
	MonsterInfoManager* mm = MonsterInfoManager::GetInstance();

	// ���Ϸε� ���� �� ��������
	GameEngineDirectory SpawnMonsterInfoFilePath;
	SpawnMonsterInfoFilePath.MoveParent("FoxTeam");
	SpawnMonsterInfoFilePath.MoveChild("Resources");
	SpawnMonsterInfoFilePath.MoveChild("SaveFiles");
	std::string FullPath = SpawnMonsterInfoFilePath.PathToPlusFileName("SpawnMonsterList.save");

	// �ش� ����� ���� ������� Open
	GameEngineFile SaveFile(FullPath, "wb");

	// ���� �Ѱ���
	SaveFile.Write(mm->GetCurMonsterListSize());
	for (int MonsterNum = 0; MonsterNum < mm->GetCurMonsterListSize(); ++MonsterNum)
	{
		MonsterInfo CurMonsterInfo = mm->GetAllMonsterListValue()[MonsterNum];

		SaveFile.Write(CurMonsterInfo.Index_);
		SaveFile.Write(static_cast<int>(CurMonsterInfo.RegionType_));
		SaveFile.Write(static_cast<int>(CurMonsterInfo.MonsterType_));
		SaveFile.Write(CurMonsterInfo.SpawnPosition_);
	}

	SaveFile.Close();
}

void MonsterInfoManager::LoadMonsterInfoFile(const std::string& _FullPath)
{
	MonsterInfoManager* mm = MonsterInfoManager::GetInstance();

	// �ش� ����� ���� �ε�
	GameEngineFile LoadFile(_FullPath, "rb");
	
	// ���� �Ѱ���
	int TotMonsterCount = 0;
	LoadFile.Read(TotMonsterCount);
	for (int MonsterNum = 0; MonsterNum < TotMonsterCount; ++MonsterNum)
	{
		MonsterInfo NewMonsterInfo = {};

		// �����ε���
		LoadFile.Read(NewMonsterInfo.Index_);

		// ����Ÿ��
		int ReadRegion = 0;
		LoadFile.Read(ReadRegion);
		NewMonsterInfo.RegionType_ = static_cast<Location>(ReadRegion);

		// ����Ÿ��
		int ReadType = 0;
		LoadFile.Read(ReadType);
		NewMonsterInfo.MonsterType_ = static_cast<MonsterType>(ReadType);

		// ������ġ
		LoadFile.Read(NewMonsterInfo.SpawnPosition_);

		AllMonsters_.push_back(NewMonsterInfo);
	}

	LoadFile.Close();

	// Flag On
	SecondCreationPacketFlag = true;
}

void MonsterInfoManager::LoadSpawnPointMeshByRegion(const std::string& _FullPath)
{
	int LocationCount = static_cast<int>(Location::MAX);
	for (int LocationNum = 0; LocationNum < LocationCount; ++LocationNum)
	{
		RefInfoByRegion& CurRefInfo = RefInfoByRegion_[LocationNum];

		Location CurRegion = static_cast<Location>(LocationNum);
		RefInfoByRegion_[LocationNum].RegionType_ = CurRegion;

		// �������
		GameEngineDirectory SpawnPointPath;
		SpawnPointPath.MoveParent("FoxTeam");
		SpawnPointPath.MoveChild("Resources");
		SpawnPointPath.MoveChild("FBX");
		SpawnPointPath.MoveChild("Monster");
		SpawnPointPath.MoveChild("SpawnPoint");

		// ���ϸ� ����
		std::string SpawnPosFileName = "Monster_SpawnPoint_";
		SpawnPosFileName += ConversionTypeToString(CurRegion);
		SpawnPosFileName += ".fbx";

		// �������
		std::string SpawnRefMeshFilePath = SpawnPointPath.PathToPlusFileName(SpawnPosFileName);

		// �����ȼ�â(ESCAPE_DOCK)�� �Ϲ������� ������ �Ұ��ϹǷ� ���ͽ�����ġ���� ����ó��
		if (Location::ESCAPE_DOCK == static_cast<Location>(LocationNum))
		{
#ifdef _DEBUG
			GameEngineDebug::OutPutDebugString("��λ��� �� �����ȼ�â�� �Ϲ������� ������ �Ұ��ϹǷ� ����ó��\n");
#endif // _DEBUG

			continue;
		}

		// �������
		CurRefInfo.FullSpawnFilePath_ = SpawnRefMeshFilePath;
	}

	// Thread �Ҵ��Ͽ� ������ġ��ǥ������ ���� �����޽� �ε�
	auto Func = [&]()
	{
		for (RefInfoByRegion& CurRefInfo : RefInfoByRegion_)
		{
			// �����ȼ�â(ESCAPE_DOCK)�� �Ϲ������� ������ �Ұ��ϹǷ� ���ͽ�����ġ���� ����ó��
			if (Location::ESCAPE_DOCK == CurRefInfo.RegionType_)
			{
#ifdef _DEBUG
				GameEngineDebug::OutPutDebugString("�޽��ε� �� �����ȼ�â�� �Ϲ������� ������ �Ұ��ϹǷ� ����ó��\n");
#endif // _DEBUG
	
				continue;
			}
	
			CurRefInfo.SpawnPointMesh_ = GameEngineFBXMeshManager::GetInst().Load(CurRefInfo.FullSpawnFilePath_);
		}
	};

	// ������ �۾��Ҵ�
	std::thread Threads(Func);

	// �ش� ������ ������̶��
	if (true == Threads.joinable())
	{
		// �ش� ������ ������� ���
		Threads.join();

		// �ش� ������ ���� ��ó��
		// 1. ������� ������ ���Ͱ�ü���� ������ ����
		int LocationCount = static_cast<int>(Location::MAX);
		for (int LocationNum = 0; LocationNum < LocationCount; ++LocationNum)
		{
			// 1-1. �������� ����
			CreateReferenceInfomation(static_cast<Location>(LocationNum));
		}

		// 2. ���������� ���� ���� �⺻���� ����
		CreateBasicMonsterInfos();

		// 3. �������� �������ϻ���
		SaveMonsterInfoFile(_FullPath);

		// ������ ������Ŷ ���۰��� Flag On(�������� �ѹ�ó�� �� �ٽ� Offó���ȴ�)
		FirstCreationPacketFlag = true;
	}
}

void MonsterInfoManager::CreateReferenceInfomation(Location _Location)
{
	// ������ ���Ͱ�ü��, ������ǥ����, A* ���(�׸���)�ε���, ... ������ ����
	int LocationNum = static_cast<int>(_Location);
	if (0 > LocationNum || static_cast<int>(Location::MAX) <= LocationNum)
	{
		GameEngineDebug::MsgBoxError("�߸��� ����Ÿ���� ���ŵǾ����ϴ�!!!" + std::to_string(LocationNum));
		return;
	}

	// Debug Complie Logging(�ּ�ó��)
#ifdef _DEBUG
	//std::string CurLocationName = LoggingTypeToString(_Location);
	//GameEngineDebug::OutPutDebugString(CurLocationName + "�� ���� ������ �����߽��ϴ�." + "Region Number: " + std::to_string(LocationNum) + "\n");
#endif // _DEBUG

	// ������ ������������
	RefInfoByRegion& CurLocationInfo = RefInfoByRegion_[LocationNum];

	// ���� Ÿ�Ժ� �ִ������ ���� ����
	SaveCreationCountByRegion(CurLocationInfo);
}

void MonsterInfoManager::SaveCreationCountByRegion(RefInfoByRegion& _ResultInfo)
{
	// ������ ����Ÿ�Ժ� �ִ������ü�� ����
	Location CurRegion = _ResultInfo.RegionType_;
	switch (CurRegion)
	{
		case Location::DOCK:			// �ױ�
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 0;
			break;
		}
		case Location::POND:			// ����
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 2;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 4;
			break;
		}
		case Location::BEACH:			// �𷡻���
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 3;
			break;
		}
		case Location::UPTOWN:			// ������ð�
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 2;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 6;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 6;
			break;
		}
		case Location::ALLEY:			// ����
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 8;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 0;
			break;
		}
		case Location::HOTEL:			// ȣ��
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 0;
			break;
		}
		case Location::AVENUE:			// ��ȭ��
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 5;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 6;
			break;
		}
		case Location::HOSPITAL:		// ����
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 0;
			break;
		}
		case Location::TEMPLE:			// ��
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 2;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 3;
			break;
		}
		case Location::ARCHERY_RANGE:	// �����
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 3;
			break;
		}
		case Location::CEMETERY:		// ����
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 2;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 4;
			break;
		}
		case Location::FOREST:			// ��
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 2;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 2;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 4;
			break;
		}
		case Location::FACTORY:			// ����
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 0;
			break;
		}
		case Location::CHAPEL:			// ����
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 8;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 3;
			break;
		}
		case Location::SCHOOL:			// �б�
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 6;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 0;
			break;
		}
		case Location::RESERCH_CENTRE:	// ������
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 0;
			break;
		}
	}

	// ������ �ִ� �������ͼ� �ջ�
	for (int MonsterNum = 0; MonsterNum < static_cast<int>(MonsterType::MAX); ++MonsterNum)
	{
		_ResultInfo.TotalMonsterCnt_ += _ResultInfo.MonsterCount_[MonsterNum];
	}

	// �� ������ ����Ÿ�Ժ� �ִ������ü���� �ջ��Ͽ� ���� = ��ü������ ���� �ִ�������ɸ��Ͱ�ü��(���������Ѱ���)
	MaxCreationCount_ += _ResultInfo.TotalMonsterCnt_;
}

void MonsterInfoManager::CreateBasicMonsterInfos()
{
	int CreationIndex = 0;
	bool GroupCheckFlag = false;
	int GroupCount = 2;
	Location PrevRegionType = Location::NONE;

	// 0-0. ����ʿ� ��ġ(����)�Ϸ��� ���� ��� �ۼ�����!!!
	for (int RegionNum = 0; RegionNum < static_cast<int>(Location::MAX); ++RegionNum)
	{
		// 0-1. ���� ������ �������� Get
		RefInfoByRegion CurRefInfo = RefInfoByRegion_[RegionNum];

		// 1-0. ���� ������ ����Ÿ�Լ���ŭ �ݺ�
		int MonsterTypeCount = static_cast<int>(MonsterType::MAX); 
		for (int MonsterTypeNum = 0; MonsterTypeNum < MonsterTypeCount; ++MonsterTypeNum)
		{
			// Initalize Local Value
			bool IsGroup = false;
			float4 RandomSpawnPos = float4::ZERO;
			GameEngineRandom Random;

			// 1-1. ���� ������ �����ؾ��ϴ� �������Ѱ�����ŭ �ݺ�
			MonsterType CurMonsterType = static_cast<MonsterType>(MonsterTypeNum);
			int CountByRegion = CurRefInfo.MonsterCount_[MonsterTypeNum];
			for (int RegionMonsterNum = 0; RegionMonsterNum < CountByRegion; ++RegionMonsterNum)
			{
				MonsterInfo NewMonsterInfo = {};

				NewMonsterInfo.Index_ = CreationIndex++;										// ���� ���� �ε���(Ž����)
				NewMonsterInfo.RegionType_ = CurRefInfo.RegionType_;							// ���� ���� ���� Ÿ��
				NewMonsterInfo.MonsterType_ = CurMonsterType;									// ���� Ÿ��

				// MonsterType::WOLF�� �׷����
				if (MonsterType::WOLF == CurMonsterType)
				{
					GroupCheckFlag = true;
				}

				// ������ġ ��������
				// ��, �׷������ �ʿ��� ���ʹ� ù����������ġ���� X��ǥ or Z��ǥ�� +10 �������� ��ġ�ϵ��� ����
				int RandomIndex = Random.RandomInt(0, static_cast<int>(CurRefInfo.SpawnPointMesh_->GetAllMeshMap()[0].Vertexs.size()) - 1);
				if (true == GroupCheckFlag)
				{
					// �׷���� ����
					if (true == IsGroup)
					{
						// �׷���������� ������ġ ����
						float4 GroupSpawnPos = float4(10.0f, 0.0f, 10.0f);
						NewMonsterInfo.SpawnPosition_ = RandomSpawnPos + GroupSpawnPos;

						// Flag & Position �ʱ�ȭ
						RandomSpawnPos = float4::ZERO;
						IsGroup = false;
						GroupCheckFlag = false;
					}
					// �׷���� ����
					else
					{
						// Search SpawnPos
						RandomSpawnPos = CurRefInfo.SpawnPointMesh_->GetAllMeshMap()[0].Vertexs[RandomIndex].POSITION;

						// �̹� ������ ��ġ�� �ٽ� �����Ϸ��Ҷ� ������ǥ ������
						if (DesignatedSpawnPos_.end() != DesignatedSpawnPos_.find(RandomSpawnPos) &&
							PrevRegionType == CurRefInfo.RegionType_)
						{
							RandomIndex = Random.RandomInt(0, static_cast<int>(CurRefInfo.SpawnPointMesh_->GetAllMeshMap()[0].Vertexs.size()) - 1);
							RandomSpawnPos = CurRefInfo.SpawnPointMesh_->GetAllMeshMap()[0].Vertexs[RandomIndex].POSITION;
						}

						// Group Creation Flag On & Spawn Position Setting
						NewMonsterInfo.SpawnPosition_ = RandomSpawnPos;
						IsGroup = true;
					}
				}
				else
				{
					// �׷������ �ƴ� ������ ������ġ ����
					RandomSpawnPos = CurRefInfo.SpawnPointMesh_->GetAllMeshMap()[0].Vertexs[RandomIndex].POSITION;

					// �̹� ������ ��ġ�� �ٽ� �����Ϸ��Ҷ� ������ǥ ������
					if (DesignatedSpawnPos_.end() != DesignatedSpawnPos_.find(RandomSpawnPos) &&
						PrevRegionType == CurRefInfo.RegionType_)
					{
						RandomIndex = Random.RandomInt(0, static_cast<int>(CurRefInfo.SpawnPointMesh_->GetAllMeshMap()[0].Vertexs.size()) - 1);
						RandomSpawnPos = CurRefInfo.SpawnPointMesh_->GetAllMeshMap()[0].Vertexs[RandomIndex].POSITION;
					}

					// Spawn Position Setting
					NewMonsterInfo.SpawnPosition_ = RandomSpawnPos;
					RandomSpawnPos = float4::ZERO;
				}

				// ���� �������� ����
				PrevRegionType = CurRefInfo.RegionType_;

				// �����Ϸ�� ��ġ��ǥ ����
				DesignatedSpawnPos_.insert(RandomSpawnPos);

				// ����. ���ͻ�������(��Ŷ����)��Ͽ� �߰�
				AllMonsters_.push_back(NewMonsterInfo);
			}
		}
	}
}

void MonsterInfoManager::SaveMonsterInfoFile(const std::string& _FullPath)
{
	MonsterInfoManager* mm = MonsterInfoManager::GetInstance();

	// �ش� ����� ���� ������� Open
	GameEngineFile SaveFile(_FullPath, "wb");

	// ���� �Ѱ���
	SaveFile.Write(mm->GetCurMonsterListSize());
	for (int MonsterNum = 0; MonsterNum < mm->GetCurMonsterListSize(); ++MonsterNum)
	{
		MonsterInfo CurMonsterInfo = mm->GetAllMonsterListValue()[MonsterNum];

		SaveFile.Write(CurMonsterInfo.Index_);
		SaveFile.Write(static_cast<int>(CurMonsterInfo.RegionType_));
		SaveFile.Write(static_cast<int>(CurMonsterInfo.MonsterType_));
		SaveFile.Write(CurMonsterInfo.SpawnPosition_);
	}

	SaveFile.Close();
}

#pragma region Logging & Conversion Function
std::string MonsterInfoManager::LoggingTypeToString(Location _Type)
{
	std::string ReturnString;

	switch (_Type)
	{
		case Location::DOCK:
		{
			ReturnString = "�ױ�(DOCK)";
			break;
		}
		case Location::POND:
		{
			ReturnString = "����(POND)";
			break;
		}
		case Location::BEACH:
		{
			ReturnString = "�𷡻���(BEACH)";
			break;
		}
		case Location::UPTOWN:
		{
			ReturnString = "��� ���ð�(UPTOWN)";
			break;
		}
		case Location::ALLEY:
		{
			ReturnString = "����(ALLEY)";
			break;
		}
		case Location::HOTEL:
		{
			ReturnString = "ȣ��(HOTEL)";
			break;
		}
		case Location::AVENUE:
		{
			ReturnString = "��ȭ��(AVENUE)";
			break;
		}
		case Location::HOSPITAL:
		{
			ReturnString = "����(HOSPITAL)";
			break;
		}
		case Location::TEMPLE:
		{
			ReturnString = "��(TEMPLE)";
			break;
		}
		case Location::ARCHERY_RANGE:
		{
			ReturnString = "�����(ARCHERY_RANGE)";
			break;
		}
		case Location::CEMETERY:
		{
			ReturnString = "����(CEMETERY)";
			break;
		}
		case Location::FOREST:
		{
			ReturnString = "��(FOREST)";
			break;
		}
		case Location::FACTORY:
		{
			ReturnString = "����(FACTORY)";
			break;
		}
		case Location::CHAPEL:
		{
			ReturnString = "����(CHAPEL)";
			break;
		}
		case Location::SCHOOL:
		{
			ReturnString = "�б�(SCHOOL)";
			break;
		}
		case Location::RESERCH_CENTRE:
		{
			ReturnString = "������(RESERCH_CENTRE)";
			break;
		}
		case Location::ESCAPE_DOCK:
		{
			ReturnString = "������ ��â(ESCAPE_DOCK)";
			break;
		}
	}

	return ReturnString;
}

std::string MonsterInfoManager::ConversionTypeToString(Location _Type)
{
	std::string ReturnString;

	switch (_Type)
	{
		case Location::DOCK:
		{
			ReturnString = "DOCK";
			break;
		}
		case Location::POND:
		{
			ReturnString = "POND";
			break;
		}
		case Location::BEACH:
		{
			ReturnString = "BEACH";
			break;
		}
		case Location::UPTOWN:
		{
			ReturnString = "UPTOWN";
			break;
		}
		case Location::ALLEY:
		{
			ReturnString = "ALLEY";
			break;
		}
		case Location::HOTEL:
		{
			ReturnString = "HOTEL";
			break;
		}
		case Location::AVENUE:
		{
			ReturnString = "AVENUE";
			break;
		}
		case Location::HOSPITAL:
		{
			ReturnString = "HOSPITAL";
			break;
		}
		case Location::TEMPLE:
		{
			ReturnString = "TEMPLE";
			break;
		}
		case Location::ARCHERY_RANGE:
		{
			ReturnString = "ARCHERY";
			break;
		}
		case Location::CEMETERY:
		{
			ReturnString = "CEMETERY";
			break;
		}
		case Location::FOREST:
		{
			ReturnString = "FOREST";
			break;
		}
		case Location::FACTORY:
		{
			ReturnString = "FACTORY";
			break;
		}
		case Location::CHAPEL:
		{
			ReturnString = "CHAPEL";
			break;
		}
		case Location::SCHOOL:
		{
			ReturnString = "SCHOOL";
			break;
		}
		case Location::RESERCH_CENTRE:
		{
			ReturnString = "RESERCH_CENTRE";
			break;
		}
		case Location::ESCAPE_DOCK:
		{
			ReturnString = "ESCAPE_DOCK";
			break;
		}
	}

	return ReturnString;
}

#pragma endregion

MonsterInfoManager::MonsterInfoManager()
	: MaxCreationCount_(0)
	, RefInfoByRegion_{}
{
}

MonsterInfoManager::~MonsterInfoManager()
{
}
