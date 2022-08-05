#include "PreCompile.h"
#include "PlayerController.h"

#include "GameMouse.h"

#include "UI_Skill.h"
#include "LH_LockOnUI.h"

#include "LH_Player.h"

PlayerController::PlayerController() // default constructer 디폴트 생성자
{

}

PlayerController::~PlayerController() // default destructer 디폴트 소멸자
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
		GameEngineDebug::MsgBoxError("존재하지 않는 플레이어");
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
		GameEngineDebug::MsgBoxError("캐릭터 이름을 지어주지 않았습니다.");
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
		//마우스 collision 체크해서 공격인지 뭔지 판별


		//isClick_ = true;
		if (true)
		{
			IsAttack_ = true;
			TargetActor_;
		}
		else
		{
			TargetActor_ = nullptr;
			TempMousePos_ = testGameMouse_->GetTransform()->GetWorldPosition(); // 미완성 코드
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
		if (IsAttack_ = true) // 마우스의 충돌 처리가 해결해줄것
		{
			MainPlayer_->Unit_SetTarget(TargetActor_);

			// 여기부턴 이제 공격 사거리 계산이 필요함
			// 공격 명령이 하달되었으면 상대방 위치까지 추적하여 공격 하도록 유도해야함,
			// State와 별개로 AI State가 필요할거 같기도 함??

			/*
			가만히 있는 Idle
			적을 추적하는 Chase
			추적 완료후 Attack
			공격 완료 직전 거리를 벗어나면 다시 Chase
			Stop 시에 Idle로 돌아가고 다시 하달

			Idle 시에도 적이 시야에 들어오면 자동 공격 (하나?)


			*/
		}
		//우선 마우스로 부터 좌표를 받아오는게 먼저
		MainPlayer_->Unit_SetTargetDir(TmepMouseDir_);
		MainPlayer_->Unit_SetTargetPos(TempMousePos_);
		MainPlayer_->Unit_SetMove(true);

		//타겟과의 거리가 매우 짧으면 리턴 시킬것

		//이미 움직이는 중인데 다시 state를 갱신시키지 않기 위해
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
