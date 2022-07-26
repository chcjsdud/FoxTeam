#include "PreCompile.h"
#include "Controller.h"

#include "Characters.h"
#include "Yuki.h"

#include <GameEngineBase/GameEngineRandom.h>
#include <GameEngine/GameEngineCore.h>

Controller* Controller::Inst = new Controller();

Controller::Controller() 
	: CurrentLevel_(nullptr)
	, MapSize_(float4::ZERO)
{
}

Controller::~Controller()
{
	// 관리목록제거
	Objects_.clear();
	Monsters_.clear();
	Characters_.clear();
}

void Controller::Initialize()
{
	// 현재 레벨 지정
	CurrentLevel_ = GameEngineCore::CurrentLevel();

	// 맵 로드
	LoadMap();

	// 배치하려는 오브젝트 생성
	// Ex) 아이템박스, ...
	CreateObject();

	// 배치하려는 몬스터 생성
	CreateMonster();

	// 배치하려는 캐릭터 생성(AI, MAIN)
	CreateCharacter();

	// 생성된 모든 액터 맵에 배치
	AllActorArrange();

	// 완료되면 게임시작!!!!
}

void Controller::LoadMap()
{
	// 맵로드


	// 네비게이션메쉬 로드?
}

void Controller::CreateObject()
{
	// 오브젝트 타입별 생성
	// 아이템 파밍용 오브젝트 같은거?



	// 템파밍 오브젝트인경우 드랍아이템 랜덤배치
	// 오브젝트는 본인이 보유한 아이템정보를 알고있을꺼임


}

void Controller::CreateMonster()
{
	// 일반몬스터 타입별 생성
	// 


	// 드랍아이템 랜덤 배치
	// 몬스터는 본인이 보유한 아이템정보를 알고있을꺼임


}

void Controller::CreateCharacter()
{
	// 생성가능한 캐릭터 직업을 랜덤지정하며 15마리를 생성한다
	Characters* NewCharacter = nullptr;
	GameEngineRandom Rand;

	for (int i = 0; i < 15; ++i)
	{
		int RandJobType = Rand.RandomInt(0, static_cast<int>(JobType::MAX) - 1);
		switch (RandJobType)
		{
			case static_cast<int>(JobType::YUKI):
			{
				Yuki* NewYuki = CurrentLevel_->CreateActor<Yuki>();
				//NewYuki->Initialize();
				//TODO : 잠시 주석 쳐놓음

				NewCharacter = dynamic_cast<Characters*>(NewYuki);
				break;
			}
			case static_cast<int>(JobType::FIORA):
			{
				//Fiora* NewFiora = CurrentLevel_->CreateActor<Fiora>();
				//NewFiora->Initialize();
				//NewCharacter = dynamic_cast<Characters*>(NewFiora);

				int a = 0;
				break;
			}
			case static_cast<int>(JobType::ZAHIR):
			{
				//Zahir* NewZahir = CurrentLevel_->CreateActor<Zahir>();
				//NewZahir->Initialize();
				//NewCharacter = dynamic_cast<Characters*>(NewZahir);

				int a = 0;
				break;
			}
			case static_cast<int>(JobType::NADINE):
			{
				//Nadine* NewNadine = CurrentLevel_->CreateActor<Nadine>();
				//NewNadine->Initialize();
				//NewCharacter = dynamic_cast<Characters*>(NewNadine);

				int a = 0;
				break;
			}
		}

		// 목록 추가
		if (nullptr != NewCharacter)
		{
			Characters_.push_back(NewCharacter);
			NewCharacter = nullptr;
		}
	}

	// 초기값 : 가장 첫번째 캐릭터가 메인캐릭터
	Characters* MainPlayer = Characters_.front();
	if (nullptr != MainPlayer)
	{
		Characters::SetMainPlayer(MainPlayer);
	}
}

void Controller::AllActorArrange()
{
	// 생성완료된 모든 액터 맵에 배치 시작
	ArrangeObjects();
	ArrangeMonsters();
	ArrangeCharacters();
}

void Controller::ArrangeObjects()
{
	// 맵범위 GET


	// 맵범위내 배치시작

}

void Controller::ArrangeMonsters()
{
	// 맵범위 GET


}

void Controller::ArrangeCharacters()
{
	// 맵범위 GET

	// 생성된 모든 캐릭터를 맵에 배치



	// 배치완료후 메인플레이어에게 카메라 포커싱


	// 메인플레이어의 정보를 UI에 전달




}

void Controller::MainPlayerSwitching(Characters* _SelCharacter)
{
	if (nullptr == Characters::GetMainPlayer())
	{
		GameEngineDebug::MsgBox("현재 메인플레이어로 지정된 캐릭터가 존재하지않습니다!!!!");
		return;
	}

	// 스위칭 전 모든 캐릭터 대기상태 전환




	// 스위칭
	for (auto& Player : Characters_)
	{
		if (Player == _SelCharacter)
		{
			// 현재 메인플레이어를 대기상태로 돌리고 AI로 전환
			Characters* CurMainPlayer = Characters::GetMainPlayer();

			// 대기상태 전환

			// AI 전환
			CurMainPlayer->SetCharacterType(CharacterType::AI);

			// 선택된 AI를 대기상태로 돌리고 메인플레이어로 전환
			// 대기상태 전환

			// MAIN 전환 후 메인플레이어 지정
			Characters::SetMainPlayer(_SelCharacter);

			break;
		}
	}

	// 스위칭 완료후 현재 메인플레이어에게 카메라 포커싱



	// 현재 메인플레이어의 정보를 이용하여 UI 정보 갱신



}
