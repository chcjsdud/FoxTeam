#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>
#include "Player.h"

#include <GameEngine/GameEngineFBXMeshManager.h>
#include <GameEngine/GameEngineFBXWindow.h>

//Idle
void Player::Idle_Start()
{
	Speed_ = 0.f;
}
void Player::Idle_Update(float _DeltaTime)
{
	if (GameEngineInput::GetInst().IsKey("W")||
		GameEngineInput::GetInst().IsKey("A")||
		GameEngineInput::GetInst().IsKey("S")||
		GameEngineInput::GetInst().IsKey("D"))
	{
		PlayerState_.ChangeState("Walk");
	}


}
void Player::Idle_End()
{
}

//Walk
void Player::Walk_Start()
{
	Speed_ = 300.f;
}
void Player::Walk_Update(float _DeltaTime)
{
	if (!GameEngineInput::GetInst().IsKey("W") &&
		!GameEngineInput::GetInst().IsKey("A") &&
		!GameEngineInput::GetInst().IsKey("S") &&
		!GameEngineInput::GetInst().IsKey("D"))
	{
		PlayerState_.ChangeState("Idle");
	}

	if (GameEngineInput::GetInst().IsKey("Space"))
	{
		PlayerState_.ChangeState("Run");
	}

	MoveUpdate(_DeltaTime);
	RotateFunc(_DeltaTime);
}
void Player::Walk_End()
{
}

//Run
void Player::Run_Start()
{
	Speed_ = 600.f;
}
void Player::Run_Update(float _DeltaTime)
{
	if (!GameEngineInput::GetInst().IsKey("Space"))
	{
		if (GameEngineInput::GetInst().IsKey("W") ||
			GameEngineInput::GetInst().IsKey("A") ||
			GameEngineInput::GetInst().IsKey("S") ||
			GameEngineInput::GetInst().IsKey("D"))
		{
			PlayerState_.ChangeState("Walk");

			return;
		}
	}

	if (!GameEngineInput::GetInst().IsKey("W") &&
		!GameEngineInput::GetInst().IsKey("A") &&
		!GameEngineInput::GetInst().IsKey("S") &&
		!GameEngineInput::GetInst().IsKey("D"))
	{
		PlayerState_.ChangeState("Idle");
	}

	MoveUpdate(_DeltaTime);
	RotateFunc(_DeltaTime);
}
void Player::Run_End()
{
}

//Attack
void Player::Attack_Start()
{
}
void Player::Attack_Update(float _DeltaTime)
{
}
void Player::Attack_End()
{
}
