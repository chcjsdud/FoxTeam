#include "Precompile.h"
#include "GameTimeController.h"

#include <GameEngine/GameEngineCore.h>

#include "GameServer.h"
#include "MonsterStateChangePacket.h"

#include "PlayerInfoManager.h"
#include "LumiaLevel.h"
#include "PlayerUIController.h"

GameTimeController* GameTimeController::GetInstance()
{
	static GameTimeController instance;
	return &instance;
}

tm GameTimeController::GetTotalGameTimeToMinute()
{
	// �Ҽ����ڸ��������� ��/�ʷ� ��ȯ
	int CurMin = 0;
	int CurSec = 0;
	int CurTimeToInt = static_cast<int>(CurGameTime_);
	CurMin = CurTimeToInt / 60;							// ��/60 = ��
	CurSec = CurTimeToInt % 60;							// ��%60 = ��

	tm ReturnGameTime = {};
	ReturnGameTime.tm_min = CurMin;
	ReturnGameTime.tm_sec = CurSec;

	return ReturnGameTime;
}

tm GameTimeController::GetTotalGameTimeToHour()
{
	// �Ҽ����ڸ��������� ��/��/�ʷ� ��ȯ
	int CurHour = 0;
	int CurMin = 0;
	int CurSec = 0;
	int CurTimeToInt = static_cast<int>(CurGameTime_);
	CurMin = CurTimeToInt / 60;							// ��/60 = ��
	CurHour = CurMin / 60;								// ��/60 = ��
	CurSec = CurTimeToInt % 60;							// ��/��/���̹Ƿ� ��%60 = ��
	CurMin = CurMin % 60;								// ��/��/���̹Ƿ� ��%60 = ��

	tm ReturnGameTime = {};
	ReturnGameTime.tm_hour = CurHour;
	ReturnGameTime.tm_min = CurMin;
	ReturnGameTime.tm_sec = CurSec;

	return ReturnGameTime;
}

float GameTimeController::GetTotalGameTimeToSec()
{
	return CurGameTime_;
}

tm GameTimeController::GetRemainTimeToMinute()
{
	// �Ҽ����ڸ��������� ��/�ʷ� ��ȯ
	int CurMin = 0;
	int CurSec = 0;
	int CurTimeToInt = static_cast<int>(DayAndNightTime_);
	CurMin = CurTimeToInt / 60;							// ��/60 = ��
	CurSec = CurTimeToInt % 60;							// ��%60 = ��

	tm ReturnGameTime = {};
	ReturnGameTime.tm_min = CurMin;
	ReturnGameTime.tm_sec = CurSec;

	return ReturnGameTime;
}

tm GameTimeController::GetRemainTimeToHour()
{
	// �Ҽ����ڸ��������� ��/��/�ʷ� ��ȯ
	int CurHour = 0;
	int CurMin = 0;
	int CurSec = 0;
	int CurTimeToInt = static_cast<int>(DayAndNightTime_);
	CurMin = CurTimeToInt / 60;							// ��/60 = ��
	CurHour = CurMin / 60;								// ��/60 = ��
	CurSec = CurTimeToInt % 60;							// ��/��/���̹Ƿ� ��%60 = ��
	CurMin = CurMin % 60;								// ��/��/���̹Ƿ� ��%60 = ��

	tm ReturnGameTime = {};
	ReturnGameTime.tm_hour = CurHour;
	ReturnGameTime.tm_min = CurMin;
	ReturnGameTime.tm_sec = CurSec;

	return ReturnGameTime;
}

float GameTimeController::GetRemainTimeToSec()
{
	return DayAndNightTime_;
}

void GameTimeController::HostInitialize()
{
	// ������ ��/�� �ð� �ʱ�ȭ
	CreateDailytimes();

	// �������ý��� ����


	// ����Ÿ�Ժ� ù����������� �ʱ�ȭ
	CreateMonsterFirstAppearInfo();
}

void GameTimeController::HostUpdate(float _DeltaTime)
{
	// ���������� ����


	// �� ����Ÿ�Ժ� ù����ð� üũ �� �̺�Ʈ �� ó��
	UpdateMonsterFirstAppearTime(_DeltaTime);

	// �������࿡ ���� �ð����� �� ��/����ȯ, ��������
	CalcGameTime(_DeltaTime);
}

void GameTimeController::GuestUpdate(float _GameTime, DayAndNightType _Type, int _CurDay, float _DayAndNightTime)
{
	CurDay_ = _CurDay;
	CurDayOrNight_ = _Type;
	CurGameTime_ = _GameTime;
	DayAndNightTime_ = _DayAndNightTime;
}

void GameTimeController::CreateDailytimes()
{
	// 4�� ��/��ð� ����
	// -> 4�ϸ��� �ݺ�
	DailyTime OneDay = {};
	//OneDay.DayTime_ = 170.0f;
	//OneDay.NightTime_ = 140.0f;

	OneDay.DayTime_ = 10.0f;
	OneDay.NightTime_ = 10.0f;

	DailyTime TwoDay = {};
	//TwoDay.DayTime_ = 140.0f;
	//TwoDay.NightTime_ = 140.0f;

	TwoDay.DayTime_ = 10.0f;
	TwoDay.NightTime_ = 10.0f;

	DailyTime ThreeDay = {};
	//ThreeDay.DayTime_ = 125.0f;
	//ThreeDay.NightTime_ = 125.0f;

	ThreeDay.DayTime_ = 10.0f;
	ThreeDay.NightTime_ = 10.0f;

	DailyTime FourDay = {};
	FourDay.DayTime_ = 110.0f;
	FourDay.NightTime_ = 110.0f;

	Dailytimes_.push_back(OneDay);
	Dailytimes_.push_back(TwoDay);
	Dailytimes_.push_back(ThreeDay);
	Dailytimes_.push_back(FourDay);

	// 0���� ������ �����̹Ƿ� ��/�� ��ȯ�ð� Dailytimes_[0]�� ���ð����� ����
	DayAndNightTime_ = Dailytimes_[0].DayTime_;
}

void GameTimeController::CalcGameTime(float _DeltaTime)
{
	// DeltaTime ����(�� ��������ð�)
	CurGameTime_ += _DeltaTime;

	// ���������� ���� ��������ȯ �� ��������
	DayAndNightTime_ -= _DeltaTime;							// ��->�� or ��->�� ��ȯ�ð� ����
	if (0.0f >= DayAndNightTime_)							// ��/�� ��ȯ�ð� ���޽�
	{
		int DayAndNightIndex = CurDay_ % 4;
		if (DayAndNightType::DAY == CurDayOrNight_)			// ���� ���϶�
		{

			// ��̾Ʒ����� �ٿ�ĳ���� �� �ش� ����Ÿ�� ù����ó��


			// ��ȯ�Ǿ����Ƿ� Ÿ���� ������ ����
			CurDayOrNight_ = DayAndNightType::NIGHT;

			//�� FOW ����
			GameEngineLevel* CurLevel = GameEngineCore::CurrentLevel();
			if (nullptr == CurLevel)
			{
				GameEngineDebug::MsgBoxError("���� ������ ���緹���� ã���� �����ϴ�!!!!");
				return;
			}
			LumiaLevel* PlayLevel = dynamic_cast<LumiaLevel*>(CurLevel);
			PlayLevel->GetCharacterActorList()[PlayerInfoManager::GetInstance()->GetMyNumber()]->GetUIController()->GetFOW()->SetFOW(true);

			// ���� ���϶� ��ȯ�ð��� ������ȯ�ð����� ����
			DayAndNightTime_ = Dailytimes_[DayAndNightIndex].NightTime_;
		}
		else if (DayAndNightType::NIGHT == CurDayOrNight_)	// ���� ���϶�
		{
			// ��ȯ�Ǿ����Ƿ� Ÿ���� ������ ����
			CurDayOrNight_ = DayAndNightType::DAY;

			// ���� ���϶� ��ȯ�ð��� ������ȯ�ð����� ����
			DayAndNightTime_ = Dailytimes_[DayAndNightIndex].DayTime_;

			//�� FOW ����
			GameEngineLevel* CurLevel = GameEngineCore::CurrentLevel();
			if (nullptr == CurLevel)
			{
				GameEngineDebug::MsgBoxError("���� ������ ���緹���� ã���� �����ϴ�!!!!");
				return;
			}
			LumiaLevel* PlayLevel = dynamic_cast<LumiaLevel*>(CurLevel);
			PlayLevel->GetCharacterActorList()[PlayerInfoManager::GetInstance()->GetMyNumber()]->GetUIController()->GetFOW()->SetFOW(false);

			// ��->������ ��ȯ�Ǿ����Ƿ� ���������� ���������� ������ ����
			PrevDay_ = CurDay_;
			CurDay_ += 1;
		}
	}
}

void GameTimeController::CreateMonsterFirstAppearInfo()
{
	// !!! ���ͳθ����� �ð��� �ʴ��� ���� !!!
	// Ex) 00:40 -> 40��, 02:20 -> 2�� 20��(140��)

	// WOLF(����): 1���� �� 00:40
	MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].IsAppear_ = false;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].AppearTime_ = 40.0f;

	// BEAR(��): 1���� �� 02:20
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].IsAppear_ = false;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].AppearTime_ = 140.0f;

	// BAT(����): 1���� �� 01:40
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].IsAppear_ = false;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].AppearTime_ = 100.0f;

	// DOG(�鰳): 1���� �� 00:40
	MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].IsAppear_ = false;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].AppearTime_ = 40.0f;

	// CHICKEN(��): ���ӽ��۰����ÿ� ����
	MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].IsAppear_ = false;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].AppearTime_ = 1.0f;

	// BOAR(�����): 1���� �� 01:40
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].IsAppear_ = false;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].AppearTime_ = 100.0f;
}

void GameTimeController::UpdateMonsterFirstAppearTime(float _DeltaTime)
{
	// �� ���� Ÿ�Ժ� ù��������, ù����ð��� ���� ù���������ȯ ó��
	if (false == AllMonsterAppearEnd_)
	{
		// 0���� ���ӽ����϶�
		// CHICKEN(��): ���ӽ��۰����ÿ� ����(����-Ŭ�� ����ȭ ó�������� 1.0���� �����̸� �ο�)
		if (false == MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].IsAppear_)
		{
			MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].AppearTime_ -= _DeltaTime;
			if (0.0f >= MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].AppearTime_)
			{
				// ���ӽ��۰� ����ʿ� ��ġ�� ��(����)���� �������� �����϶�� ����� ����
				FirstAppearMonsters(MonsterType::CHICKEN);

				// ����Ϸ�� ���� Flag On
				MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].IsAppear_ = true;
			}
		}

		// 1���� ���϶�
		if (1 == CurDay_ && DayAndNightType::DAY == CurDayOrNight_)
		{
			// WOLF(����): 1���� �� 00:40
			if (false == MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].IsAppear_)
			{
				MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].AppearTime_ -= _DeltaTime;
				if (0.0f >= MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].AppearTime_)
				{
					// �����ǽ��� ��� ����
					FirstAppearMonsters(MonsterType::WOLF);

					// ����Ϸ�� ���� Flag On
					MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].IsAppear_ = true;
				}
			}

			// BAT(����): 1���� �� 01:40
			if (false == MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].IsAppear_)
			{
				MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].AppearTime_ -= _DeltaTime;
				if (0.0f >= MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].AppearTime_)
				{
					// �����ǽ��� ��� ����
					FirstAppearMonsters(MonsterType::BAT);

					// ����Ϸ�� ���� Flag On
					MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].IsAppear_ = true;
				}
			}

			// DOG(�鰳): 1���� �� 00:40
			if (false == MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].IsAppear_)
			{
				MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].AppearTime_ -= _DeltaTime;
				if (0.0f >= MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].AppearTime_)
				{
					// �����ǽ��� ��� ����
					FirstAppearMonsters(MonsterType::DOG);

					// ����Ϸ�� ���� Flag On
					MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].IsAppear_ = true;
				}
			}

			// BOAR(�����): 1���� �� 01:40
			if (false == MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].IsAppear_)
			{
				MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].AppearTime_ -= _DeltaTime;
				if (0.0f >= MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].AppearTime_)
				{
					// �����ǽ��� ��� ����
					FirstAppearMonsters(MonsterType::BOAR);

					// ����Ϸ�� ���� Flag On
					MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].IsAppear_ = true;
				}
			}
		}

		// 1���� ���϶�
		if (1 == CurDay_ && DayAndNightType::NIGHT == CurDayOrNight_)
		{
			// BEAR(��): 1���� �� 02:20
			if (false == MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].IsAppear_)
			{
				MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].AppearTime_ -= _DeltaTime;
				if (0.0f >= MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].AppearTime_)
				{
					// �����ǽ��� ��� ����
					FirstAppearMonsters(MonsterType::BEAR);

					// ����Ϸ�� ���� Flag On
					MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].IsAppear_ = true;
				}
			}
		}

#pragma region �׽�Ʈ��
		//TestTime_ -= _DeltaTime;
		//if (0.0f >= TestTime_)
		//{
		//	if (MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].IsAppear_ == false)
		//	{
		//		FirstAppearMonsters(MonsterType::WOLF);
		//		FirstAppearMonsters(MonsterType::BEAR);
		//		FirstAppearMonsters(MonsterType::BAT);
		//		FirstAppearMonsters(MonsterType::DOG);
		//		FirstAppearMonsters(MonsterType::CHICKEN);
		//		FirstAppearMonsters(MonsterType::BOAR);
		//		MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].IsAppear_ = true;
		//		MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].IsAppear_ = true;
		//		MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].IsAppear_ = true;
		//		MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].IsAppear_ = true;
		//		MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].IsAppear_ = true;
		//		MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].IsAppear_ = true;
		//	}
		//}
#pragma endregion
	}
}

void GameTimeController::FirstAppearMonsters(MonsterType _MonsterType)
{
	// ù��������, ù����ð��ʰ��� �� ����Ÿ���� ù������·� ��ȯó��
	GameEngineLevel* CurLevel = GameEngineCore::CurrentLevel();
	if (nullptr == CurLevel)
	{
		GameEngineDebug::MsgBoxError("���� ������ ���緹���� ã���� �����ϴ�!!!!");
		return;
	}

	// ��̾Ʒ����� �ٿ�ĳ���� �� �ش� ����Ÿ�� ù����ó��
	LumiaLevel* PlayLevel = dynamic_cast<LumiaLevel*>(CurLevel);
	PlayLevel->HostMonsterFirstAppearBatchProcessing(_MonsterType);

	// ���� ù������� �ϰ���ȯ ��Ŷ����
	MonsterStateChangePacket ChangeAppearStatePacket;
	ChangeAppearStatePacket.SetIndex(-1);
	ChangeAppearStatePacket.SetMonsterType(_MonsterType);
	ChangeAppearStatePacket.SetMonsterStateType(MonsterStateType::APPEAR);
	ChangeAppearStatePacket.SetTargetIndex(-1);
	GameServer::GetInstance()->Send(&ChangeAppearStatePacket);

	// ��� ���Ͱ� ù����Ϸ�� AllMonsterAppearEnd_ Flag On
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
	, TestTime_(1.0f)
{
}

GameTimeController::~GameTimeController()
{
}
