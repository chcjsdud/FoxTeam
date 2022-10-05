#include "PreCompile.h"
#include "MonsterInfoManager.h"

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
	// 0. ������ �׺�޽� �ε� �� ���������������� �޽� ����
	LoadSpawnPointMeshByRegion();

	// 1. ������� ������ ���Ͱ�ü���� ������ ����
	int LocationCount = static_cast<int>(Location::MAX);
	for (int LocationNum = 0; LocationNum < LocationCount; ++LocationNum)
	{
		// 1-1. �������� ����
		CreateReferenceInfomation(static_cast<Location>(LocationNum));
	}
	
	// 2. ���������� ���� ���� �⺻���� ����
	CreateBasicMonsterInfos();

	return true;
}

void MonsterInfoManager::LoadSpawnPointMeshByRegion()
{
	int LocationCount = static_cast<int>(Location::MAX);
	for (int LocationNum = 0; LocationNum < LocationCount; ++LocationNum)
	{
		Location CurRegion = static_cast<Location>(LocationNum);

		// �����ȼ�â�� �Ϲ������� ������ �Ұ��ϹǷ� ������ġ���� ����ó��
		if (Location::ESCAPE_DOCK == CurRegion)
		{
#ifdef _DEBUG
			GameEngineDebug::OutPutDebugString("�����ȼ�â�� �Ϲ������� ������ �Ұ��ϹǷ� ������ġ���� ����ó��\n");
			continue;
#endif // _DEBUG
		}

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
		std::string FullPath = SpawnPointPath.PathToPlusFileName(SpawnPosFileName);

		// ���Ϸε� �� ������ ����
		GameEngineFBXMesh* LoadMesh = GameEngineFBXMeshManager::GetInst().Load(FullPath);
		if (nullptr == LoadMesh)
		{
#ifdef _DEBUG
			std::string ErrorMsg = "�ش� ������ ���������ʽ��ϴ�!!! (���ϸ�: " + FullPath + ")\n";
			GameEngineDebug::OutPutDebugString(ErrorMsg);
#endif // _DEBUG
			continue;
		}
		else
		{
			RefInfoByRegion_[LocationNum].SpawnPoints_ = LoadMesh;
		}
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

	// Debug Complie Logging
#ifdef _DEBUG
	std::string CurLocationName = LoggingTypeToString(_Location);
	GameEngineDebug::OutPutDebugString(CurLocationName + "�� ���� ������ �����߽��ϴ�." + "Region Number: " + std::to_string(LocationNum) + "\n");
#endif // _DEBUG

	// ������ ������������
	RefInfoByRegion& CurLocationInfo = RefInfoByRegion_[LocationNum];
	CurLocationInfo.Region_ = _Location;

	// ���� Ÿ�Ժ� �ִ������ ���� ����
	SaveCreationCountByRegion(CurLocationInfo);
}

void MonsterInfoManager::SaveCreationCountByRegion(RefInfoByRegion& _ResultInfo)
{
	int MaxCreationCount = 0;

	// ������ ����Ÿ�Ժ� �ִ������ü�� ����
	//WOLF,						// ����
	//BEAR,						// ��
	//BAT,						// ����
	//DOG,						// �鰳
	//CHICKEN,					// ��
	//BOAR,						// �����



	// �� ������ ����Ÿ�Ժ� �ִ������ü���� �ջ��Ͽ� ���� = �ִ�������ɸ��Ͱ�ü��(���������Ѱ���)
	MaxCreationCount_ = MaxCreationCount;
}

void MonsterInfoManager::CreateBasicMonsterInfos()
{
	//// ���� �������� �ۼ� ����(���������� �����Ǵ� ����Ÿ�԰� ���� ����)
	////int Index_;									// ���� ���� �ε���(Ž����)
	////Location AreaType_;							// ���� ���� ���� Ÿ��(Ž����)
	////MonsterType MonsterType_;						// ���� Ÿ��
	////float4 SpawnPosition_;						// ���� ��ġ
	////int IsGroup_;									// �׷��������(1: �׷����λ���, 0: �ܵ�����)
	////int GroupCount_;								// �׷�����϶� ������ġ�� �����ؾ��ϴ� �߻�������
	//// -> ���� ���� ��� �ش� �Լ����� ����



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
