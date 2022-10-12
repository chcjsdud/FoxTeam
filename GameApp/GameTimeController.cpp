#include "Precompile.h"
#include "GameTimeController.h"

#include <GameEngine/GameEngineCore.h>

#include "GameServer.h"
#include "MonsterStateChangePacket.h"

#include "LumiaLevel.h"

GameTimeController* GameTimeController::GetInstance()
{
	static GameTimeController instance;
	return &instance;
}

tm GameTimeController::GetCurrentGameTimeToMinute()
{
	// 소수점자리빼버리고 분/초로 반환
	int CurMin = 0;
	int CurSec = 0;
	int CurTimeToInt = static_cast<int>(CurGameTime_);
	CurMin = CurTimeToInt / 60;							// 초/60 = 분
	CurSec = CurTimeToInt % 60;							// 초%60 = 초

	tm ReturnGameTime = {};
	ReturnGameTime.tm_min = CurMin;
	ReturnGameTime.tm_sec = CurSec;

	return ReturnGameTime;
}

tm GameTimeController::GetCurrentGameTimeToHour()
{
	// 소수점자리빼버리고 시/분/초로 반환
	int CurHour = 0;
	int CurMin = 0;
	int CurSec = 0;
	int CurTimeToInt = static_cast<int>(CurGameTime_);
	CurMin = CurTimeToInt / 60;							// 초/60 = 분
	CurHour = CurMin / 60;								// 분/60 = 시
	CurSec = CurTimeToInt % 60;							// 시/분/초이므로 초%60 = 초
	CurMin = CurMin % 60;								// 시/분/초이므로 분%60 = 분

	tm ReturnGameTime = {};
	ReturnGameTime.tm_hour = CurHour;
	ReturnGameTime.tm_min = CurMin;
	ReturnGameTime.tm_sec = CurSec;

	return ReturnGameTime;
}

float GameTimeController::GetCurrentGameTimeToSec()
{
	return CurGameTime_;
}

void GameTimeController::Initialize()
{
	// 일차별 낮/밤 시간 초기화
	CreateDailytimes();

	// 레벨업시스템 관련


	// 몬스터타입별 첫등장관련정보 초기화
	CreateMonsterFirstAppearInfo();
}

void GameTimeController::Update(float _DeltaTime)
{
	// 레벨업관련 갱신


	// 각 몬스터타입별 첫등장시간 체크 및 이벤트 등 처리
	UpdateMonsterFirstAppearTime(_DeltaTime);

	// 게임진행에 따른 시간누적 및 낮/밤전환, 일차증가
	CalcGameTime(_DeltaTime);
}

void GameTimeController::CreateDailytimes()
{
	// 4일 낮/밤시간 정의
	// -> 4일마다 반복
	DailyTime OneDay = {};
	OneDay.DayTime_ = 170.0f;
	OneDay.NightTime_ = 140.0f;

	DailyTime TwoDay = {};
	TwoDay.DayTime_ = 140.0f;
	TwoDay.NightTime_ = 140.0f;

	DailyTime ThreeDay = {};
	ThreeDay.DayTime_ = 125.0f;
	ThreeDay.NightTime_ = 125.0f;

	DailyTime FourDay = {};
	FourDay.DayTime_ = 110.0f;
	FourDay.NightTime_ = 110.0f;

	Dailytimes_.push_back(OneDay);
	Dailytimes_.push_back(TwoDay);
	Dailytimes_.push_back(ThreeDay);
	Dailytimes_.push_back(FourDay);

	// 0일차 낮부터 시작이므로 낮/밤 전환시간 Dailytimes_[0]의 낮시간으로 설정
	DayAndNightTime_ = Dailytimes_[0].DayTime_;
}

void GameTimeController::CalcGameTime(float _DeltaTime)
{
	// DeltaTime 누적
	CurGameTime_ += _DeltaTime;

	// 현재일차에 따라 낮과밤전환 및 일차증가
	DayAndNightTime_ -= _DeltaTime;							// 낮->밤 or 밤->낮 전환시간 감소
	if (0.0f >= DayAndNightTime_)							// 낮/밤 전환시간 도달시
	{
		int DayAndNightIndex = CurDay_ % 4;
		if (DayAndNightType::DAY == CurDayOrNight_)			// 현재 낮일때
		{
			// 전환되었으므로 타입을 밤으로 설정
			CurDayOrNight_ = DayAndNightType::NIGHT;

			// 현재 밤일때 전환시간을 현재전환시간으로 설정
			DayAndNightTime_ = Dailytimes_[DayAndNightIndex].NightTime_;
		}
		else if (DayAndNightType::NIGHT == CurDayOrNight_)	// 현재 밤일때
		{
			// 전환되었으므로 타입을 낮으로 설정
			CurDayOrNight_ = DayAndNightType::DAY;

			// 현재 낮일때 전환시간을 현재전환시간으로 설정
			DayAndNightTime_ = Dailytimes_[DayAndNightIndex].DayTime_;

			// 밤->낮으로 전환되었으므로 현재일차를 이전일차에 저장후 증가
			PrevDay_ = CurDay_;
			CurDay_ += 1;
		}
	}
}

void GameTimeController::CreateMonsterFirstAppearInfo()
{
	// !!! 이터널리턴의 시간은 초단위 기준 !!!
	// Ex) 00:40 -> 40초, 02:20 -> 2분 20초(140초)

	// WOLF(늑대): 1일차 낮 00:40
	MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].IsAppear_ = false;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].DayType_ = DayAndNightType::DAY;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].AppearDay_ = 1;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].AppearTime_ = 40.0f;
	
	// BEAR(곰): 1일차 밤 02:20
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].IsAppear_ = false;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].DayType_ = DayAndNightType::NIGHT;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].AppearDay_ = 1;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].AppearTime_ = 140.0f;

	// BAT(박쥐): 1일차 낮 01:40
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].IsAppear_ = false;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].DayType_ = DayAndNightType::DAY;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].AppearDay_ = 1;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].AppearTime_ = 100.0f;
	
	// DOG(들개): 1일차 낮 00:40
	MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].IsAppear_ = false;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].DayType_ = DayAndNightType::DAY;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].AppearDay_ = 1;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].AppearTime_ = 40.0f;
	
	// CHICKEN(닭): 게임시작과동시에 등장
	MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].IsAppear_ = false;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].DayType_ = DayAndNightType::NONE;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].AppearDay_ = 0;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].AppearTime_ = 0.0f;

	// BOAR(멧돼지): 1일차 낮 01:40
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].IsAppear_ = false;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].DayType_ = DayAndNightType::DAY;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].AppearDay_ = 1;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].AppearTime_ = 100.0f;
}

void GameTimeController::UpdateMonsterFirstAppearTime(float _DeltaTime)
{
	// 각 몬스터 타입별 첫등장조건, 첫등장시간에 따른 첫등장상태전환 처리
	if (false == AllMonsterAppearEnd_)
	{
		// 0일차 게임시작일때
		// CHICKEN(닭): 게임시작과동시에 등장
		if (false == MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].IsAppear_)
		{
			// 게임시작과 현재맵에 배치된 닭(몬스터)에게 등장모션을 실행하라는 명령을 전달
			FirstAppearMonsters(MonsterType::CHICKEN);

			// 등장완료로 인한 Flag On
			MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].IsAppear_ = true;
		}

		// 1일차 낮일때
		if (1 == CurDay_ && DayAndNightType::DAY == CurDayOrNight_)
		{
			// WOLF(늑대): 1일차 낮 00:40
			if (false == MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].IsAppear_)
			{
				MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].AppearTime_ -= _DeltaTime;
				if (0.0f >= MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].AppearTime_)
				{
					// 등장모션실행 명령 전달
					FirstAppearMonsters(MonsterType::WOLF);

					// 등장완료로 인한 Flag On
					MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].IsAppear_ = true;
				}
			}

			// BAT(박쥐): 1일차 낮 01:40
			if (false == MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].IsAppear_)
			{
				MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].AppearTime_ -= _DeltaTime;
				if (0.0f >= MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].AppearTime_)
				{
					// 등장모션실행 명령 전달
					FirstAppearMonsters(MonsterType::BAT);

					// 등장완료로 인한 Flag On
					MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].IsAppear_ = true;
				}
			}

			// DOG(들개): 1일차 낮 00:40
			if (false == MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].IsAppear_)
			{
				MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].AppearTime_ -= _DeltaTime;
				if (0.0f >= MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].AppearTime_)
				{
					// 등장모션실행 명령 전달
					FirstAppearMonsters(MonsterType::DOG);

					// 등장완료로 인한 Flag On
					MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].IsAppear_ = true;
				}
			}

			// BOAR(멧돼지): 1일차 낮 01:40
			if (false == MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].IsAppear_)
			{
				MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].AppearTime_ -= _DeltaTime;
				if (0.0f >= MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].AppearTime_)
				{
					// 등장모션실행 명령 전달
					FirstAppearMonsters(MonsterType::BOAR);

					// 등장완료로 인한 Flag On
					MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].IsAppear_ = true;
				}
			}
		}

		// 1일차 밤일때
		if (1 == CurDay_ && DayAndNightType::NIGHT == CurDayOrNight_)
		{
			// BEAR(곰): 1일차 밤 02:20
			if (false == MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].IsAppear_)
			{
				MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].AppearTime_ -= _DeltaTime;
				if (0.0f >= MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].AppearTime_)
				{
					// 등장모션실행 명령 전달
					FirstAppearMonsters(MonsterType::BEAR);

					// 등장완료로 인한 Flag On
					MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].IsAppear_ = true;
				}
			}
		}
	}
}

void GameTimeController::FirstAppearMonsters(MonsterType _MonsterType)
{
	// 첫등장조건, 첫등장시간초과시 각 몬스터타입의 첫등장상태로 전환처리
	GameEngineLevel* CurLevel = GameEngineCore::CurrentLevel();
	if (nullptr == CurLevel)
	{
		GameEngineDebug::MsgBoxError("현재 게임의 현재레벨을 찾을수 없습니다!!!!");
		return;
	}

	// 루미아레벨로 다운캐스팅 후 해당 몬스터타입 첫등장처리
	LumiaLevel* PlayLevel = dynamic_cast<LumiaLevel*>(CurLevel);
	PlayLevel->HostMonsterFirstAppear(_MonsterType);

	// 몬스터 첫등장상태 일괄변환 패킷전송
	MonsterStateChangePacket ChangeAppearStatePacket;
	ChangeAppearStatePacket.SetMonsterType(_MonsterType);
	ChangeAppearStatePacket.SetMonsterStateType(MonsterStateType::APPEAR);
	GameServer::GetInstance()->Send(&ChangeAppearStatePacket);

	// 모든 몬스터가 첫등장완료시 AllMonsterAppearEnd_ Flag On
	if (MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].IsAppear_ == true &&
		MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].IsAppear_ == true &&
		MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].IsAppear_ == true &&
		MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].IsAppear_ == true &&
		MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].IsAppear_ == true &&
		MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].IsAppear_ == true)
	{
		AllMonsterAppearEnd_ = true;
	}
}

GameTimeController::GameTimeController()
	: DayAndNightTime_(0.0f)
	, CurGameTime_(0.0f)
	, PrevDay_(0)
	, CurDay_(0)
	, CurDayOrNight_(DayAndNightType::DAY)
	, AllMonsterAppearEnd_(false)
	, MonsterFirstAppearList_{}
{
}

GameTimeController::~GameTimeController()
{
}
