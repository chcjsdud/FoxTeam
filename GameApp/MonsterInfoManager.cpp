#include "PreCompile.h"
#include "MonsterInfoManager.h"

#include <GameEngineBase/GameEngineRandom.h>

#include "PlayerInfoManager.h"
#include "MonsterInfoManager.h"

#include "GameServer.h"
#include "GameClient.h"
#include "CreationCommandPacket.h"

bool MonsterInfoManager::CreationPacketFlag = false;

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
	// 0. 지역별 레퍼런스 정보 생성 및 생성패킷정보 셋팅, 생성패킷전송
	LoadSpawnPointMeshByRegion();

	return true;
}

void MonsterInfoManager::LoadSpawnPointMeshByRegion()
{
	int LocationCount = static_cast<int>(Location::MAX);
	for (int LocationNum = 0; LocationNum < LocationCount; ++LocationNum)
	{
		RefInfoByRegion& CurRefInfo = RefInfoByRegion_[LocationNum];

		Location CurRegion = static_cast<Location>(LocationNum);
		RefInfoByRegion_[LocationNum].RegionType_ = CurRegion;

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
		std::string SpawnRefMeshFilePath = SpawnPointPath.PathToPlusFileName(SpawnPosFileName);

		// 오래된선창(ESCAPE_DOCK)은 일반적으로 진입이 불가하므로 몬스터스폰위치에서 제외처리
		if (Location::ESCAPE_DOCK == static_cast<Location>(LocationNum))
		{
#ifdef _DEBUG
			GameEngineDebug::OutPutDebugString("경로생성 중 오래된선창은 일반적으로 진입이 불가하므로 제외처리\n");
#endif // _DEBUG

			continue;
		}

		// 최종경로
		CurRefInfo.FullSpawnFilePath_ = SpawnRefMeshFilePath;
	}

	// Thread 할당하여 스폰위치좌표참조를 위한 지역메쉬 로드
	auto Func = [&]()
	{
		for (RefInfoByRegion& CurRefInfo : RefInfoByRegion_)
		{
			// 오래된선창(ESCAPE_DOCK)은 일반적으로 진입이 불가하므로 몬스터스폰위치에서 제외처리
			if (Location::ESCAPE_DOCK == CurRefInfo.RegionType_)
			{
#ifdef _DEBUG
				GameEngineDebug::OutPutDebugString("메쉬로드 중 오래된선창은 일반적으로 진입이 불가하므로 제외처리\n");
#endif // _DEBUG
	
				continue;
			}
	
			CurRefInfo.SpawnPointMesh_ = GameEngineFBXMeshManager::GetInst().Load(CurRefInfo.FullSpawnFilePath_);
		}
	};

	// 스레드 작업할당
	std::thread Threads(Func);

	// 해당 스레드 사용중이라면
	if (true == Threads.joinable())
	{
		// 해당 스레드 종료까지 대기
		Threads.join();

		// 1. 현재맵의 지역별 몬스터개체수등 정보를 생성
		int LocationCount = static_cast<int>(Location::MAX);
		for (int LocationNum = 0; LocationNum < LocationCount; ++LocationNum)
		{
			// 1-1. 참고정보 생성
			CreateReferenceInfomation(static_cast<Location>(LocationNum));
		}

		// 2. 참고정보를 토대로 몬스터 기본정보 생성
		CreateBasicMonsterInfos();

		// 3. 생성패킷 정보 저장
		MonsterInfoManager::GetInstance()->SetMonsterInfos(AllMonsters_);

		// 종료후 생성패킷 전송가능 Flag On(서버에서 한번처리 후 다시 Off처리된다)
		CreationPacketFlag = true;
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

	// Debug Complie Logging(주석처리)
#ifdef _DEBUG
	//std::string CurLocationName = LoggingTypeToString(_Location);
	//GameEngineDebug::OutPutDebugString(CurLocationName + "의 정보 생성을 시작했습니다." + "Region Number: " + std::to_string(LocationNum) + "\n");
#endif // _DEBUG

	// 지역별 정보생성시작
	RefInfoByRegion& CurLocationInfo = RefInfoByRegion_[LocationNum];

	// 몬스터 타입별 최대생성수 정보 저장
	SaveCreationCountByRegion(CurLocationInfo);
}

void MonsterInfoManager::SaveCreationCountByRegion(RefInfoByRegion& _ResultInfo)
{
	// 지역의 몬스터타입별 최대생성개체수 지정
	Location CurRegion = _ResultInfo.RegionType_;
	switch (CurRegion)
	{
		case Location::DOCK:			// 항구
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 0;
			break;
		}
		case Location::POND:			// 연못
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 2;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 4;
			break;
		}
		case Location::BEACH:			// 모래사장
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 3;
			break;
		}
		case Location::UPTOWN:			// 고급주택가
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 2;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 6;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 6;
			break;
		}
		case Location::ALLEY:			// 골목길
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 8;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 0;
			break;
		}
		case Location::HOTEL:			// 호텔
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 0;
			break;
		}
		case Location::AVENUE:			// 번화가
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 5;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 6;
			break;
		}
		case Location::HOSPITAL:		// 병원
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 0;
			break;
		}
		case Location::TEMPLE:			// 절
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 2;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 3;
			break;
		}
		case Location::ARCHERY_RANGE:	// 양궁장
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 3;
			break;
		}
		case Location::CEMETERY:		// 묘지
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 2;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 4;
			break;
		}
		case Location::FOREST:			// 숲
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 2;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 2;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 4;
			break;
		}
		case Location::FACTORY:			// 공장
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 0;
			break;
		}
		case Location::CHAPEL:			// 성당
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 3;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 8;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 3;
			break;
		}
		case Location::SCHOOL:			// 학교
		{
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::WOLF)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BEAR)] = 0;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BAT)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::DOG)] = 4;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::CHICKEN)] = 6;
			_ResultInfo.MonsterCount_[static_cast<int>(MonsterType::BOAR)] = 0;
			break;
		}
		case Location::RESERCH_CENTRE:	// 연구소
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

	// 지역별 최대 생성몬스터수 합산
	for (int MonsterNum = 0; MonsterNum < static_cast<int>(MonsterType::MAX); ++MonsterNum)
	{
		_ResultInfo.TotalMonsterCnt_ += _ResultInfo.MonsterCount_[MonsterNum];
	}

	// 각 지역의 몬스터타입별 최대생성개체수를 합산하여 저장 = 전체지역에 대한 최대생성가능몬스터개체수(생성몬스터총갯수)
	MaxCreationCount_ += _ResultInfo.TotalMonsterCnt_;
}

void MonsterInfoManager::CreateBasicMonsterInfos()
{
	int CreationIndex = 0;
	Location PrevRegionType = Location::NONE;

	// 0-0. 현재맵에 배치(생성)하려는 몬스터 목록 작성시작!!!
	for (int RegionNum = 0; RegionNum < static_cast<int>(Location::MAX); ++RegionNum)
	{
		// 0-1. 현재 지역의 참고정보 Get
		RefInfoByRegion CurRefInfo = RefInfoByRegion_[RegionNum];

		// 1-0. 현재 지역의 몬스터타입수만큼 반복
		int MonsterTypeCount = static_cast<int>(MonsterType::MAX); 
		for (int MonsterTypeNum = 0; MonsterTypeNum < MonsterTypeCount; ++MonsterTypeNum)
		{
			// Initalize Local Value
			bool IsGroup = false;
			float4 RandomSpawnPos = float4::ZERO;
			GameEngineRandom Random;

			// 1-1. 현재 지역의 생성해야하는 몬스터제한갯수만큼 반복
			MonsterType CurMonsterType = static_cast<MonsterType>(MonsterTypeNum);
			int CountByRegion = CurRefInfo.MonsterCount_[MonsterTypeNum];
			for (int RegionMonsterNum = 0; RegionMonsterNum < CountByRegion; ++RegionMonsterNum)
			{
				MonsterInfo NewMonsterInfo = {};

				NewMonsterInfo.Index_ = CreationIndex++;										// 몬스터 생성 인덱스(탐색용)
				NewMonsterInfo.RegionType_ = CurRefInfo.RegionType_;							// 몬스터 스폰 지역 타입
				NewMonsterInfo.MonsterType_ = CurMonsterType;									// 몬스터 타입
				NewMonsterInfo.IsGroup_ = 0;													// 그룹생성여부(1: 그룹으로생성, 0: 단독생성)
				NewMonsterInfo.GroupCount_ = 0;													// 그룹생성일때 같은위치에 생성해야하는 야생동물수

				// MonsterType::WOLF는 그룹생성
				if (MonsterType::WOLF == CurMonsterType)
				{
					NewMonsterInfo.IsGroup_ = 1;
					NewMonsterInfo.GroupCount_ = 2;
				}

				// 스폰위치 랜덤지정
				// 단, 그룹생성이 필요한 몬스터는 첫지정스폰위치에서 X좌표 or Z좌표가 +-10 범위내에 위치하도록 설정
				int RandomIndex = Random.RandomInt(0, static_cast<int>(CurRefInfo.SpawnPointMesh_->GetAllMeshMap()[0].Vertexs.size()) - 1);
				if (1 == NewMonsterInfo.IsGroup_)
				{
					// 그룹생성 종료
					if (true == IsGroup)
					{
						// 그룹생성에의한 스폰위치 지정
						float4 GroupSpawnPos = float4(10.0f, 0.0f, 10.0f);
						NewMonsterInfo.SpawnPosition_ = RandomSpawnPos + GroupSpawnPos;

						// Flag & Position 초기화
						RandomSpawnPos = float4::ZERO;
						IsGroup = false;
					}
					// 그룹생성 시작
					else
					{
						// Search SpawnPos
						RandomSpawnPos = CurRefInfo.SpawnPointMesh_->GetAllMeshMap()[0].Vertexs[RandomIndex].POSITION;

						// 이미 지정된 위치에 다시 스폰하려할때 스폰좌표 재지정
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
					// 그룹생성이 아닌 몬스터의 스폰위치 지정
					RandomSpawnPos = CurRefInfo.SpawnPointMesh_->GetAllMeshMap()[0].Vertexs[RandomIndex].POSITION;

					// 이미 지정된 위치에 다시 스폰하려할때 스폰좌표 재지정
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

				// 이전 지역정보 저장
				PrevRegionType = CurRefInfo.RegionType_;

				// 지정완료된 위치좌표 저장
				DesignatedSpawnPos_.insert(RandomSpawnPos);

				// 최종. 몬스터생성정보(패킷정보)목록에 추가
				AllMonsters_.push_back(NewMonsterInfo);
			}
		}
	}


	// 확인용
	for (int i = 0; i < static_cast<int>(AllMonsters_.size()); ++i)
	{
		for (int j = 0; j < static_cast<int>(AllMonsters_.size()); ++j)
		{
			if (i == j)
			{
				continue;
			}

			if (AllMonsters_[i].SpawnPosition_ == AllMonsters_[j].SpawnPosition_)
			{
				int a = 0;
			}
		}
	}
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
