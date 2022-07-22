#include "PreCompile.h"
#include "LockOnUI.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"

#include "Player.h"

LockOnUI::LockOnUI()
	: Time(1.0f), UIOn(false)
{
}

LockOnUI::~LockOnUI()
{

}

void LockOnUI::Start()
{
	{
		LockOnRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		LockOnRenderer->SetImage("TestRockOn.png", "PointSmp");
		LockOnRenderer->GetTransform()->SetLocalScaling(LockOnRenderer->GetCurrentTexture()->GetTextureSize());
		LockOnRenderer->GetTransform()->SetLocalPosition({0.0f,0.0f,0.0f,0.0f});
		LockOnRenderer->Off();
	}
}

void LockOnUI::Update(float _Time)
{
	GameEngineActor* Target = Player_->PlayerGetTarget();

	//타겟이 있으면 위치정보를 가져오고 LockOnRenderer가 ON
	if (Target != nullptr)
	{
	float4 position = Target->GetTransform()->GetWorldPosition();
	LockOnRenderer->On();
	LockOnRenderer->GetTransform()->SetLocalPosition(GetLockOnPosition(position));
//	//GetLockOnPosition(position);
	//3D위치를 토대로 2D위치를 받아와야함
	//LockOnRenderer->GetTransform()->SetLocalPosition();
	}
	else
	{
		//타겟이 null이면 LockOnRenderer를 끈다
		LockOnRenderer->Off();
	}


}


