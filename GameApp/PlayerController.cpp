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

PlayerController::~PlayerController() // default destructer 디폴트 소멸자
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
	KeyStateUpdate(); // 마우스, 키보드 조작 상태를 bool 값으로 업데이트함
	MouseUpdate(); // 마우스 버튼을 눌렀는지 안눌렀는지 여부와 UI,Actor, Map 등 충돌을 검사함

	PlayerStateUpdate(); // 위 마우스 충돌, 키 조작 여부를 통해 MainPlayer의 State를 변경함
	CameraUpdate(_DeltaTime); // 모든것이 끝나고 MainPlayer 기준으로 카메라를 업데이트함
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

	// 1. 마우스가 클릭이 됬는데
	if (Key_RB_ == true)
	{
		TmepMouseDir_ = MainPlayer_->GetTransform()->GetWorldPosition() - TempMousePos_;

		{
			// 1-1. UI 를 눌렀나?
			Mouse_UICol_ = GameMouse_->Mouse_GetCollision(CollisionGroup::UI);

			if (Mouse_UICol_ == false)
			{
				// 1-2. UI가 아니면 Actor를 눌렀나?
				Mouse_TargetActor_ = GameMouse_->Mouse_GetCollisionActor(CollisionGroup::Monster);
			}

			else if (Mouse_TargetActor_ == nullptr)
			{
				//Mouse_NavoCol_ = GameMouse_->Mouse_GetCollisionMesh();
			}
		}

		// 2. 그게 UI 라면
		if (Mouse_UICol_ == true)
		{
			IsAttack_ = false;
			IsMove_ = false;
		}
		// 2-1. 그게 엑터라면
		else if (Mouse_TargetActor_ != nullptr)
		{
			MainPlayer_->Unit_SetTarget(Mouse_TargetActor_);
			IsAttack_ = true;
			IsMove_ = false;

			// 엑터를 클릭했을경우, Unit 내부에서 타겟의 좌표와 방향을 스스로 계산함
		}
		// 2-2. 그게 맵이라면
		else if (Mouse_NavoCol_ == true)
		{
			MainPlayer_->Unit_SetTarget(nullptr);
			IsAttack_ = false;
			IsMove_ = true;

			//마우스에서 좌표 얻어와서 넘겨주기

			MainPlayer_->Unit_SetTargetPos(TempMousePos_);
			MainPlayer_->Unit_SetTargetDir(TmepMouseDir_);
		}
		// 2-3. 마우스가 클릭 됬는데 그게 엑터도 아니고 맵도 아니고 UI도 아니라면
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
	//마우스 클릭과 별도로 지시를 내려줄 알고리즘 필요

	if (Key_RB_ == true)
	{
		if (IsAttack_ = true)
		{
			MainPlayer_->Unit_SetTarget(Mouse_TargetActor_);

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
		//MainPlayer_->Unit_SetTargetDir(TmepMouseDir_);
		//MainPlayer_->Unit_SetTargetPos(TempMousePos_);
		//MainPlayer_->Unit_SetMove(true);

		//타겟과의 거리가 매우 짧으면 리턴 시킬것

		//이미 움직이는 중인데 다시 state를 갱신시키지 않기 위해
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
		GameEngineDebug::MsgBoxError("존재하지 않는 플레이어");
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
		GameEngineDebug::MsgBoxError("캐릭터 이름을 지어주지 않았습니다.");
	}

	PlayerMap_.insert(std::make_pair(_Player->GetName(), _Player));

	if (MainPlayer_ == nullptr)
	{
		MainPlayer_ = _Player;
	}
}
