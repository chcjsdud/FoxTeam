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
	// 0. 지역별 네비메쉬 로드 및 지역별참고정보에 메쉬 지정
	LoadSpawnPointMeshByRegion();

	// 1. 현재맵의 지역별 몬스터개체수등 정보를 생성
	int LocationCount = static_cast<int>(Location::MAX);
	for (int LocationNum = 0; LocationNum < LocationCount; ++LocationNum)
	{
		// 1-1. 참고정보 생성
		CreateReferenceInfomation(static_cast<Location>(LocationNum));
	}
	
	// 2. 참고정보를 토대로 몬스터 기본정보 생성
	CreateBasicMonsterInfos();

	return true;
}

void MonsterInfoManager::LoadSpawnPointMeshByRegion()
{
	int LocationCount = static_cast<int>(Location::MAX);
	for (int LocationNum = 0; LocationNum < LocationCount; ++LocationNum)
	{
		Location CurRegion = static_cast<Location>(LocationNum);

		// 오래된선창은 일반적으로 진입이 불가하므로 스폰위치에서 제외처리
		if (Location::ESCAPE_DOCK == CurRegion)
		{
#ifdef _DEBUG
			GameEngineDebug::OutPutDebugString("오래된선창은 일반적으로 진입이 불가하므로 스폰위치에서 제외처리\n");
			continue;
#endif // _DEBUG
		}

		// 경로편집
		GameEngineDirectory SpawnPointPath;
		SpawnPointPath.MoveParent("FoxTeam");
		SpawnPointPath.MoveChild("Resources");
		SpawnPointPath.MoveChild("FBX");
		SpawnPointPath.MoveChild("Monster");
		SpawnPointPath.MoveChild("SpawnPoint");

		// 파일명 편집
		std::string SpawnPosFileName = "Monster_SpawnPoint_";
		SpawnPosFileName += ConversionTypeToString(CurRegion);
		SpawnPosFileName += ".fbx";

		// 최종경로
		std::string FullPath = SpawnPointPath.PathToPlusFileName(SpawnPosFileName);

		// 파일로드 및 포인터 저장
		GameEngineFBXMesh* LoadMesh = GameEngineFBXMeshManager::GetInst().Load(FullPath);
		if (nullptr == LoadMesh)
		{
#ifdef _DEBUG
			std::string ErrorMsg = "해당 파일이 존재하지않습니다!!! (파일명: " + FullPath + ")\n";
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
	// 지역별 몬스터개체수, 지역좌표범위, A* 노드(그리드)인덱스, ... 정보를 생성
	int LocationNum = static_cast<int>(_Location);
	if (0 > LocationNum || static_cast<int>(Location::MAX) <= LocationNum)
	{
		GameEngineDebug::MsgBoxError("잘못된 지역타입이 수신되었습니다!!!" + std::to_string(LocationNum));
		return;
	}

	// Debug Complie Logging
#ifdef _DEBUG
	std::string CurLocationName = LoggingTypeToString(_Location);
	GameEngineDebug::OutPutDebugString(CurLocationName + "의 정보 생성을 시작했습니다." + "Region Number: " + std::to_string(LocationNum) + "\n");
#endif // _DEBUG

	// 지역별 정보생성시작
	RefInfoByRegion& CurLocationInfo = RefInfoByRegion_[LocationNum];
	CurLocationInfo.Region_ = _Location;

	// 몬스터 타입별 최대생성수 정보 저장
	SaveCreationCountByRegion(CurLocationInfo);
}

void MonsterInfoManager::SaveCreationCountByRegion(RefInfoByRegion& _ResultInfo)
{
	int MaxCreationCount = 0;

	// 지역의 몬스터타입별 최대생성개체수 지정
	//WOLF,						// 늑대
	//BEAR,						// 곰
	//BAT,						// 박쥐
	//DOG,						// 들개
	//CHICKEN,					// 닭
	//BOAR,						// 멧돼지



	// 각 지역의 몬스터타입별 최대생성개체수를 합산하여 저장 = 최대생성가능몬스터개체수(생성몬스터총갯수)
	MaxCreationCount_ = MaxCreationCount;
}

void MonsterInfoManager::CreateBasicMonsterInfos()
{
	//// 몬스터 생성정보 작성 시작(생성지역당 생성되는 몬스터타입과 수는 고정)
	////int Index_;									// 몬스터 생성 인덱스(탐색용)
	////Location AreaType_;							// 몬스터 스폰 지역 타입(탐색용)
	////MonsterType MonsterType_;						// 몬스터 타입
	////float4 SpawnPosition_;						// 스폰 위치
	////int IsGroup_;									// 그룹생성여부(1: 그룹으로생성, 0: 단독생성)
	////int GroupCount_;								// 그룹생성일때 같은위치에 생성해야하는 야생동물수
	//// -> 위의 정보 모두 해당 함수에서 결정



}

#pragma region Logging & Conversion Function
std::string MonsterInfoManager::LoggingTypeToString(Location _Type)
{
	std::string ReturnString;

	switch (_Type)
	{
		case Location::DOCK:
		{
			ReturnString = "항구(DOCK)";
			break;
		}
		case Location::POND:
		{
			ReturnString = "연못(POND)";
			break;
		}
		case Location::BEACH:
		{
			ReturnString = "모래사장(BEACH)";
			break;
		}
		case Location::UPTOWN:
		{
			ReturnString = "고급 주택가(UPTOWN)";
			break;
		}
		case Location::ALLEY:
		{
			ReturnString = "골목길(ALLEY)";
			break;
		}
		case Location::HOTEL:
		{
			ReturnString = "호텔(HOTEL)";
			break;
		}
		case Location::AVENUE:
		{
			ReturnString = "번화가(AVENUE)";
			break;
		}
		case Location::HOSPITAL:
		{
			ReturnString = "병원(HOSPITAL)";
			break;
		}
		case Location::TEMPLE:
		{
			ReturnString = "절(TEMPLE)";
			break;
		}
		case Location::ARCHERY_RANGE:
		{
			ReturnString = "양궁장(ARCHERY_RANGE)";
			break;
		}
		case Location::CEMETERY:
		{
			ReturnString = "묘지(CEMETERY)";
			break;
		}
		case Location::FOREST:
		{
			ReturnString = "숲(FOREST)";
			break;
		}
		case Location::FACTORY:
		{
			ReturnString = "공장(FACTORY)";
			break;
		}
		case Location::CHAPEL:
		{
			ReturnString = "성당(CHAPEL)";
			break;
		}
		case Location::SCHOOL:
		{
			ReturnString = "학교(SCHOOL)";
			break;
		}
		case Location::RESERCH_CENTRE:
		{
			ReturnString = "연구소(RESERCH_CENTRE)";
			break;
		}
		case Location::ESCAPE_DOCK:
		{
			ReturnString = "오래된 선창(ESCAPE_DOCK)";
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
