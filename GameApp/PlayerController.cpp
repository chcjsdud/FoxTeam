#include "PreCompile.h"
#include "PlayerController.h"

#include "GameMouse.h"

#include "UI_Skill.h"
#include "LH_LockOnUI.h"

#include "LH_Player.h"

PlayerController::PlayerController()
	: MainPlayer_(nullptr)
	, GameMouse_(nullptr)
	, Key_RB_(false)
	, Key_LB_(false)
	, Key_Stop_(false)
	, Key_Attack_(false)
	, IsAttack_(false)
	, IsMove_(false)
	, Mouse_NavoCol_(false)
	, Mouse_TargetActor_(nullptr)
	//, Mouse_ActorCol_(false)
	, Mouse_UICol_(false)
{

}

PlayerController::~PlayerController() // default destructer ����Ʈ �Ҹ���
{

}

void PlayerController::Start()
{
	GameMouse_ = GetLevel()->CreateActor<LH_Mouse>();

	InitInput();

	{
		CameraState_.CreateState<PlayerController>("EternalReturn", this, nullptr, &PlayerController::CameraUpdate_EternalReturn, nullptr);
		CameraState_.ChangeState("EternalReturn");
	}
}

void PlayerController::InitInput()
{
	if (false == GameEngineInput::GetInst().IsKey("Key_LB"))
	{
		GameEngineInput::GetInst().CreateKey("Key_LB", VK_LBUTTON);
		GameEngineInput::GetInst().CreateKey("Key_RB", VK_RBUTTON);
		GameEngineInput::GetInst().CreateKey("Key_Stop", 'S');
		GameEngineInput::GetInst().CreateKey("Key_Attack", 'A');
	}
}

void PlayerController::Update(float _DeltaTime)
{
	KeyStateUpdate(); // ���콺, Ű���� ���� ���¸� bool ������ ������Ʈ��
	MouseUpdate(); // ���콺 ��ư�� �������� �ȴ������� ���ο� UI,Actor, Map �� �浹�� �˻���

	PlayerStateUpdate(); // �� ���콺 �浹, Ű ���� ���θ� ���� MainPlayer�� State�� ������
	CameraUpdate(_DeltaTime); // ������ ������ MainPlayer �������� ī�޶� ������Ʈ��
}

void PlayerController::KeyStateUpdate()
{
	Key_RB_ = GameEngineInput::GetInst().Down("Key_RB");
	Key_LB_ = GameEngineInput::GetInst().Down("Key_LB");
	Key_Stop_ = GameEngineInput::GetInst().Down("Key_Stop");
	Key_Attack_ = GameEngineInput::GetInst().Down("Key_Attack");
}

void PlayerController::MouseUpdate()
{
	TempMousePos_ = GameMouse_->Mouse_GetMousePos();

	// 1. ���콺�� Ŭ���� ��µ�
	if (Key_RB_ == true)
	{
		TmepMouseDir_ = MainPlayer_->GetTransform()->GetWorldPosition() - TempMousePos_;

		{
			// 1-1. UI �� ������?
			Mouse_UICol_ = GameMouse_->Mouse_GetCollision(CollisionGroup::UI);

			if (Mouse_UICol_ == false)
			{
				// 1-2. UI�� �ƴϸ� Actor�� ������?
				Mouse_TargetActor_ = GameMouse_->Mouse_GetCollisionActor(CollisionGroup::Monster);
			}

			else if (Mouse_TargetActor_ == nullptr)
			{
				//Mouse_NavoCol_ = GameMouse_->Mouse_GetCollisionMesh();
			}
		}

		// 2. �װ� UI ���
		if (Mouse_UICol_ == true)
		{
			IsAttack_ = false;
			IsMove_ = false;
		}
		// 2-1. �װ� ���Ͷ��
		else if (Mouse_TargetActor_ != nullptr)
		{
			MainPlayer_->Unit_SetTarget(Mouse_TargetActor_);
			IsAttack_ = true;
			IsMove_ = false;

			// ���͸� Ŭ���������, Unit ���ο��� Ÿ���� ��ǥ�� ������ ������ �����
		}
		// 2-2. �װ� ���̶��
		else if (Mouse_NavoCol_ == true)
		{
			MainPlayer_->Unit_SetTarget(nullptr);
			IsAttack_ = false;
			IsMove_ = true;

			//���콺���� ��ǥ ���ͼ� �Ѱ��ֱ�

			MainPlayer_->Unit_SetTargetPos(TempMousePos_);
			MainPlayer_->Unit_SetTargetDir(TmepMouseDir_);
		}
		// 2-3. ���콺�� Ŭ�� ��µ� �װ� ���͵� �ƴϰ� �ʵ� �ƴϰ� UI�� �ƴ϶��
		else
		{
			MainPlayer_->Unit_SetTarget(nullptr);
			IsAttack_ = false;
			IsMove_ = false;

			MainPlayer_->Unit_SetTargetPos(float4::ZERO);
			MainPlayer_->Unit_SetTargetDir(float4::ZERO);
		}

	}
	else
	{
		IsMove_ = false;
	}
}

void PlayerController::PlayerStateUpdate()
{
	//���콺 Ŭ���� ������ ���ø� ������ �˰��� �ʿ�

	if (Key_RB_ == true)
	{
		if (IsAttack_ = true)
		{
			MainPlayer_->Unit_SetTarget(Mouse_TargetActor_);

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
		//MainPlayer_->Unit_SetTargetDir(TmepMouseDir_);
		//MainPlayer_->Unit_SetTargetPos(TempMousePos_);
		//MainPlayer_->Unit_SetMove(true);

		//Ÿ�ٰ��� �Ÿ��� �ſ� ª���� ���� ��ų��

		//�̹� �����̴� ���ε� �ٽ� state�� ���Ž�Ű�� �ʱ� ����
		ChangePlayerState("Walk");
	}

	if (Key_Stop_ == true)
	{
		//MainPlayer_->Unit_SetTargetDir(float4::ZERO);
		//MainPlayer_->Unit_SetTargetPos(float4::ZERO);
		//MainPlayer_->Unit_SetMove(false);

		ChangePlayerState("Idle");
	}
}

void PlayerController::CameraUpdate(float _DeltaTime)
{
	CameraState_.Update(_DeltaTime);
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
	MainPlayer_->Unit_AIController_On();

	MainPlayer_ = Playerptr;

	MainPlayer_->Player_OnUI();
	MainPlayer_->Player_SetIscontrolled(true);
	MainPlayer_->Unit_AIController_Off();
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
