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
	// �����������
	Objects_.clear();
	Monsters_.clear();
	Characters_.clear();
}

void Controller::Initialize()
{
	// ���� ���� ����
	CurrentLevel_ = GameEngineCore::CurrentLevel();

	// �� �ε�
	LoadMap();

	// ��ġ�Ϸ��� ������Ʈ ����
	// Ex) �����۹ڽ�, ...
	CreateObject();

	// ��ġ�Ϸ��� ���� ����
	CreateMonster();

	// ��ġ�Ϸ��� ĳ���� ����(AI, MAIN)
	CreateCharacter();

	// ������ ��� ���� �ʿ� ��ġ
	AllActorArrange();

	// �Ϸ�Ǹ� ���ӽ���!!!!
}

void Controller::LoadMap()
{
	// �ʷε�


	// �׺���̼Ǹ޽� �ε�?
}

void Controller::CreateObject()
{
	// ������Ʈ Ÿ�Ժ� ����
	// ������ �Ĺֿ� ������Ʈ ������?



	// ���Ĺ� ������Ʈ�ΰ�� ��������� ������ġ
	// ������Ʈ�� ������ ������ ������������ �˰���������


}

void Controller::CreateMonster()
{
	// �Ϲݸ��� Ÿ�Ժ� ����
	// 


	// ��������� ���� ��ġ
	// ���ʹ� ������ ������ ������������ �˰���������


}

void Controller::CreateCharacter()
{
	// ���������� ĳ���� ������ ���������ϸ� 15������ �����Ѵ�
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
				//TODO : ��� �ּ� �ĳ���

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

		// ��� �߰�
		if (nullptr != NewCharacter)
		{
			Characters_.push_back(NewCharacter);
			NewCharacter = nullptr;
		}
	}

	// �ʱⰪ : ���� ù��° ĳ���Ͱ� ����ĳ����
	Characters* MainPlayer = Characters_.front();
	if (nullptr != MainPlayer)
	{
		Characters::SetMainPlayer(MainPlayer);
	}
}

void Controller::AllActorArrange()
{
	// �����Ϸ�� ��� ���� �ʿ� ��ġ ����
	ArrangeObjects();
	ArrangeMonsters();
	ArrangeCharacters();
}

void Controller::ArrangeObjects()
{
	// �ʹ��� GET


	// �ʹ����� ��ġ����

}

void Controller::ArrangeMonsters()
{
	// �ʹ��� GET


}

void Controller::ArrangeCharacters()
{
	// �ʹ��� GET

	// ������ ��� ĳ���͸� �ʿ� ��ġ



	// ��ġ�Ϸ��� �����÷��̾�� ī�޶� ��Ŀ��


	// �����÷��̾��� ������ UI�� ����




}

void Controller::MainPlayerSwitching(Characters* _SelCharacter)
{
	if (nullptr == Characters::GetMainPlayer())
	{
		GameEngineDebug::MsgBox("���� �����÷��̾�� ������ ĳ���Ͱ� ���������ʽ��ϴ�!!!!");
		return;
	}

	// ����Ī �� ��� ĳ���� ������ ��ȯ




	// ����Ī
	for (auto& Player : Characters_)
	{
		if (Player == _SelCharacter)
		{
			// ���� �����÷��̾ �����·� ������ AI�� ��ȯ
			Characters* CurMainPlayer = Characters::GetMainPlayer();

			// ������ ��ȯ

			// AI ��ȯ
			CurMainPlayer->SetCharacterType(CharacterType::AI);

			// ���õ� AI�� �����·� ������ �����÷��̾�� ��ȯ
			// ������ ��ȯ

			// MAIN ��ȯ �� �����÷��̾� ����
			Characters::SetMainPlayer(_SelCharacter);

			break;
		}
	}

	// ����Ī �Ϸ��� ���� �����÷��̾�� ī�޶� ��Ŀ��



	// ���� �����÷��̾��� ������ �̿��Ͽ� UI ���� ����



}
