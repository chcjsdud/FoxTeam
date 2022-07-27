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

	//Ÿ���� ������ ��ġ������ �������� LockOnRenderer�� ON
	if (Target != nullptr)
	{
	float4 position = Target->GetTransform()->GetWorldPosition();
	LockOnRenderer->On();
	LockOnRenderer->GetTransform()->SetLocalPosition(GetLockOnPosition(position));
//	//GetLockOnPosition(position);
	//3D��ġ�� ���� 2D��ġ�� �޾ƿ;���
	//LockOnRenderer->GetTransform()->SetLocalPosition();
	}
	else
	{
		//Ÿ���� null�̸� LockOnRenderer�� ����
		LockOnRenderer->Off();
	}


}


