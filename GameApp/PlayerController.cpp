#include "PreCompile.h"
#include "PlayerController.h"

#include "GameMouse.h"

#include "UI_Skill.h"
#include "LH_LockOnUI.h"

#include "LH_Player.h"

PlayerController::PlayerController()
	: MainPlayer_(nullptr)
	, GameMouse_(nullptr)
	, Mouse_NaviCol_(false)
	//, Controller_Target_ID_(0)
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

	InitKeyState();

	{
		CameraState_.CreateState<PlayerController>("EternalReturn", this, nullptr, &PlayerController::CameraUpdate_EternalReturn, nullptr);
		CameraState_.ChangeState("EternalReturn");
	}
}

void PlayerController::ControllerInit()
{
}

void PlayerController::InitKeyState()
{
	GameEngineInput::GetInst().CreateKey("Key_LB", VK_LBUTTON);
	GameEngineInput::GetInst().CreateKey("Key_RB", VK_RBUTTON);

	GameEngineInput::GetInst().CreateKey("Key_A", 'A');
	GameEngineInput::GetInst().CreateKey("Key_S", 'S');
	GameEngineInput::GetInst().CreateKey("Key_H", 'H');

	GameEngineInput::GetInst().CreateKey("Key_X", 'X');
	GameEngineInput::GetInst().CreateKey("Key_F", 'F');
	GameEngineInput::GetInst().CreateKey("Key_C", 'C');
	GameEngineInput::GetInst().CreateKey("Key_V", 'V');
	GameEngineInput::GetInst().CreateKey("Key_B", 'B');
	GameEngineInput::GetInst().CreateKey("Key_M", 'M');
	GameEngineInput::GetInst().CreateKey("Key_P", 'P');
	GameEngineInput::GetInst().CreateKey("Key_Y", 'Y');

	GameEngineInput::GetInst().CreateKey("Key_1", 0x31);
	GameEngineInput::GetInst().CreateKey("Key_2", 0x32);
	GameEngineInput::GetInst().CreateKey("Key_3", 0x33);
	GameEngineInput::GetInst().CreateKey("Key_4", 0x34);
	GameEngineInput::GetInst().CreateKey("Key_5", 0x35);
	GameEngineInput::GetInst().CreateKey("Key_6", 0x36);
	GameEngineInput::GetInst().CreateKey("Key_7", 0x37);
	GameEngineInput::GetInst().CreateKey("Key_8", 0x38);
	GameEngineInput::GetInst().CreateKey("Key_9", 0x39);
	GameEngineInput::GetInst().CreateKey("Key_0", 0x30);

	GameEngineInput::GetInst().CreateKey("Key_Q", 'Q');
	GameEngineInput::GetInst().CreateKey("Key_W", 'W');
	GameEngineInput::GetInst().CreateKey("Key_E", 'E');
	GameEngineInput::GetInst().CreateKey("Key_R", 'R');
	GameEngineInput::GetInst().CreateKey("Key_D", 'D');

	GameEngineInput::GetInst().CreateKey("Key_Tab", VK_TAB);

	GameEngineInput::GetInst().CreateKey("Key_ESC", VK_ESCAPE);
}

void PlayerController::Update(float _DeltaTime)
{
	//  플레이어 컨트롤러 업데이트는 크게 3단계
	// 1. 키 입력 업데이트
	// 2. 마우스가 무엇을 클릭했는가(지형, Actor, UI) 업데이트
	// 3. 키, 마우스 클릭에 걸맞게 플레이어 State 업데이트

	KeyStateUpdate(_DeltaTime); // 마우스, 키보드 조작 상태를 bool 값으로 업데이트함

	MouseUpdate(); // 마우스 버튼을 눌렀는지 안눌렀는지 여부와 UI,Actor, Map 등 충돌을 검사함

	CameraUpdate(_DeltaTime); // 모든것이 끝나고 MainPlayer 기준으로 카메라를 업데이트함
}

void PlayerController::KeyStateUpdate(float _DeltaTime)
{
	Key_LB_ = GameEngineInput::GetInst().Down("Key_LB");
	Key_RB_ = GameEngineInput::GetInst().Down("Key_RB");

	Key_A_ = GameEngineInput::GetInst().Down("Key_A");
	Key_S_ = GameEngineInput::GetInst().Down("Key_S");
	Key_H_ = GameEngineInput::GetInst().Down("Key_H");

	Key_X_ = GameEngineInput::GetInst().Down("Key_X");
	Key_F_ = GameEngineInput::GetInst().Down("Key_F");
	Key_C_ = GameEngineInput::GetInst().Down("Key_C");
	Key_V_ = GameEngineInput::GetInst().Down("Key_V");
	Key_B_ = GameEngineInput::GetInst().Down("Key_B");
	Key_M_ = GameEngineInput::GetInst().Down("Key_M");
	Key_P_ = GameEngineInput::GetInst().Down("Key_P");
	Key_Y_ = GameEngineInput::GetInst().Down("Key_Y");

	Key_1_ = GameEngineInput::GetInst().Down("Key_1");
	Key_2_ = GameEngineInput::GetInst().Down("Key_2");
	Key_3_ = GameEngineInput::GetInst().Down("Key_3");
	Key_4_ = GameEngineInput::GetInst().Down("Key_4");
	Key_5_ = GameEngineInput::GetInst().Down("Key_5");
	Key_6_ = GameEngineInput::GetInst().Down("Key_6");
	Key_7_ = GameEngineInput::GetInst().Down("Key_7");
	Key_8_ = GameEngineInput::GetInst().Down("Key_8");
	Key_9_ = GameEngineInput::GetInst().Down("Key_9");
	Key_0_ = GameEngineInput::GetInst().Down("Key_0");

	Key_Q_ = GameEngineInput::GetInst().Down("Key_Q");
	Key_W_ = GameEngineInput::GetInst().Down("Key_W");
	Key_E_ = GameEngineInput::GetInst().Down("Key_E");
	Key_R_ = GameEngineInput::GetInst().Down("Key_R");
	Key_D_ = GameEngineInput::GetInst().Down("Key_D");

	Key_Tab_ = GameEngineInput::GetInst().Down("Key_Tab");
	Key_Esc_ = GameEngineInput::GetInst().Down("Key_ESC");
}

void PlayerController::MouseUpdate()
{
	// 마우스 UI를 위해 마우스 좌표를 상시 업데이트함
	UIMousePos3D_ = GameMouse_->Mouse_GetMousePos();
	UIMousePos2D_ = GameEngineInput::GetInst().GetMousePos();

	Mouse_UICol_ = GameMouse_->Mouse_GetCollision(CollisionGroup::UI);

	if (GameEngineInput::GetInst().Down("Key_LB") == true)
	{
		// 좌클릭 했을때 그것이 유닛인지 UI인지 등등 판별 (나중에 UI작업과 함께함 지금은 어택땅만 구분함)
		if (Mouse_UICol_ != true)
		{
			//LB를 눌렀고 UI위에 없는 경우

			if (Key_A_ == true);
			{
				Controller_Order_ = Order::Attack_Pos;
				Controller_Target_Pos_ = UIMousePos3D_;
			}
		}

		return;
	}

	else if (GameEngineInput::GetInst().Down("Key_RB") == true)
	{
		// 1. 우클릭 했는데 적일때
		// 2. 우클릭 했는데 지형일때

		if (Mouse_UICol_ == true)
		{
			// 우클릭 했는데 마우스가 UI에 있으면 리턴함
			return;
		}

		else
		{
			Controller_Target_Unit_ = dynamic_cast<Unit*>(GameMouse_->Mouse_GetCollisionActor(CollisionGroup::Unit));

			//unsigned int collisionTargetID =0;

			//Controller_Target_ID_ = collisionTargetID;

			//if (Controller_Target_ID_ != 0)
			if (Controller_Target_Unit_ != nullptr)
			{
				Controller_Order_ = Order::Attack_Target;
			}

			else if (Mouse_NaviCol_ == true)
			{
				Controller_Order_ = Order::Move;
				Controller_Target_Pos_ = UIMousePos3D_;
			}
			else
			{
				// 있을 수 없는 상태
				return;
			}

			return; // 마우스 LB, RB를 동시에 눌렀을 경우, RB를 우선시함 특별한 이유는 없음
		}
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
