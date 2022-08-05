#include "PreCompile.h"
#include "PlayerController.h"

#include "GameMouse.h"

#include "UI_Skill.h"
#include "LH_LockOnUI.h"

#include "LH_Player.h"

PlayerController::PlayerController() // default constructer ����Ʈ ������
{

}

PlayerController::~PlayerController() // default destructer ����Ʈ �Ҹ���
{

}

void PlayerController::InitInput()
{
	if (false == GameEngineInput::GetInst().IsKey("Key_LB"))
	{
		GameEngineInput::GetInst().CreateKey("Key_LB", VK_LBUTTON); 
		GameEngineInput::GetInst().CreateKey("Key_RB", VK_RBUTTON); 
		GameEngineInput::GetInst().CreateKey("Key_Stop", 'S'); 
	}
}

void PlayerController::UIInit()
{
	//SkillUI_ = GetLevel()->CreateActor<UI_Skill>();
	//SkillUI_->GetTransform()->SetWorldPosition(float4(0.0f, 0.0f, 0.0f, 0.f));
	//SkillUI_->SetPlayer(MainActor_);

	//LockOnUI_ = GetLevel()->CreateActor<LockOnUI>();
	//LockOnUI_->GetTransform()->SetWorldPosition(float4(0.0f, 0.0f, 0.0f));
	//LockOnUI_->SetPlayer(MainActor_);
	//LockOnUI_->Off();

	//Inventory_ = GetLevel()->CreateActor<Inventory>();
	//Inventory_->SetPlayer(this);
}

void PlayerController::SwitchMainPlayer(std::string _Name)
{
	Player* Playerptr = PlayerMap_.find(_Name)->second;
	
	if (Playerptr == nullptr)
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� �÷��̾�");
		return;
	}

	if (Playerptr == MainPlayer_)
	{
		return;
	}

	MainPlayer_->Player_OffUI();
	MainPlayer_->Player_SetIscontrolled(false);

	MainPlayer_ = Playerptr;

	MainPlayer_->Player_OnUI();
	MainPlayer_->Player_SetIscontrolled(true);
}

void PlayerController::ChangePlayerState(std::string _State)
{
	if (MainPlayer_->Unit_GetStateName() != _State)
	{
		MainPlayer_->Unit_ChangeState(_State);
	}
}

void PlayerController::AddPlayerMap(std::string _Name, Player* _Player)
{
	PlayerMap_.insert(std::make_pair(_Name, _Player));

	if (MainPlayer_ == nullptr)
	{
		MainPlayer_ = _Player;
	}
}

void PlayerController::AddPlayerMap(Player* _Player)
{
	if (_Player->GetName() == "")
	{
		GameEngineDebug::MsgBoxError("ĳ���� �̸��� �������� �ʾҽ��ϴ�.");
	}

	PlayerMap_.insert(std::make_pair(_Player->GetName(), _Player));

	if (MainPlayer_ == nullptr)
	{
		MainPlayer_ = _Player;
	}
}

void PlayerController::Start()
{
	InitInput();

	{
		CameraState_.CreateState<PlayerController>("EternalReturn", this, nullptr, &PlayerController::CameraUpdate_EternalReturn, nullptr);
		CameraState_.ChangeState("EternalReturn");
	}
}

void PlayerController::MouseUpdate()
{
	if (Key_RB_ == true)
	{
		//���콺 collision üũ�ؼ� �������� ���� �Ǻ�


		//isClick_ = true;
		if (true)
		{
			IsAttack_ = true;
			TargetActor_;
		}
		else
		{
			TargetActor_ = nullptr;
			TempMousePos_ = testGameMouse_->GetTransform()->GetWorldPosition(); // �̿ϼ� �ڵ�
			IsAttack_ = false;
		}

	}
	else
	{
		isClick_ = false;
	}
}

void PlayerController::PlayerStateUpdate()
{
	if (Key_RB_ == true)
	{
		if (IsAttack_ = true) // ���콺�� �浹 ó���� �ذ����ٰ�
		{
			MainPlayer_->Unit_SetTarget(TargetActor_);

			// ������� ���� ���� ��Ÿ� ����� �ʿ���
			// ���� ����� �ϴ޵Ǿ����� ���� ��ġ���� �����Ͽ� ���� �ϵ��� �����ؾ���,
			// State�� ������ AI State�� �ʿ��Ұ� ���⵵ ��??

			/*
			������ �ִ� Idle
			���� �����ϴ� Chase
			���� �Ϸ��� Attack
			���� �Ϸ� ���� �Ÿ��� ����� �ٽ� Chase
			Stop �ÿ� Idle�� ���ư��� �ٽ� �ϴ�

			Idle �ÿ��� ���� �þ߿� ������ �ڵ� ���� (�ϳ�?)


			*/
		}
		//�켱 ���콺�� ���� ��ǥ�� �޾ƿ��°� ����
		MainPlayer_->Unit_SetTargetDir(TmepMouseDir_);
		MainPlayer_->Unit_SetTargetPos(TempMousePos_);
		MainPlayer_->Unit_SetMove(true);

		//Ÿ�ٰ��� �Ÿ��� �ſ� ª���� ���� ��ų��

		//�̹� �����̴� ���ε� �ٽ� state�� ���Ž�Ű�� �ʱ� ����
		ChangePlayerState("Walk");
	}

	if (Key_Stop_ == true)
	{
		MainPlayer_->Unit_SetTargetDir(float4::ZERO);
		MainPlayer_->Unit_SetTargetPos(float4::ZERO);
		MainPlayer_->Unit_SetMove(false);

		ChangePlayerState("Idle");
	}
}

void PlayerController::KeyStateUpdate()
{
	Key_RB_ = GameEngineInput::GetInst().Down("Key_RB");
	Key_LB_ = GameEngineInput::GetInst().Down("Key_LB");
	Key_Stop_ = GameEngineInput::GetInst().Down("Key_Stop");
}

void PlayerController::CameraUpdate(float _DeltaTime)
{
	CameraState_.Update(_DeltaTime);
}

void PlayerController::Update(float _DeltaTime)
{
	KeyStateUpdate();
	MouseUpdate();

	PlayerStateUpdate();
	CameraUpdate(_DeltaTime);
}

void PlayerController::CameraUpdate_EternalReturn(float _DeltaTime)
{
	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}
	else
	{
		float4 CamPos = MainPlayer_->GetTransform()->GetWorldPosition();
		CamPos.y += 700.f;
		CamPos.z -= 550;

		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldPosition(CamPos);
		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldRotationDegree({ 60.f,0.f,0.f });

	}
}
