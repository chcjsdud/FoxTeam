#include "PreCompile.h"
#include "PJW_Hyunwoo.h"
#include "PJW_Enum.h"

#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

PJW_Hyunwoo::PJW_Hyunwoo() // default constructer 디폴트 생성자
	:FBXRenderer_(nullptr)
{

}

PJW_Hyunwoo::~PJW_Hyunwoo() // default destructer 디폴트 소멸자
{

}

PJW_Hyunwoo::PJW_Hyunwoo(PJW_Hyunwoo&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void PJW_Hyunwoo::Start()
{
	status_MoveSpeed_ = 50.0f;
	status_ATK_ = 20.0f;
	status_HP_ = 100.0f;
	curHP_ = 100.0f;
	//Init_FBX();
	Init_FSM();
	Init_Collision();
}

void PJW_Hyunwoo::Init_FBX()
{
	// 렌더링용 버텍스, 인덱스 버퍼 생성
	FBXRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	FBXRenderer_->SetFBXMesh("Hyunwoo_01_LOD1.FBX", "TextureDeferredLightAni");
	FBXRenderer_->GetTransform()->SetLocalPosition({ 0.0f, -50.0f, 0.0f });
	FBXRenderer_->GetTransform()->SetLocalScaling({ 30.0f, 30.0f, 30.0f });
	FBXRenderer_->GetTransform()->SetLocalRotationDegree({-90.0f, 0.0f, 0.0f});

	curDir_ = { 0.0f,0.0f,1.0f,1.0f };
	for (UINT i = 0; i < FBXRenderer_->GetRenderSetCount(); i++)
	{
		FBXRenderer_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Hyunwoo_01_LOD1.png");
	}



//	FBXRenderer_->CreateFBXAnimation("Move", "Hyunwoo_01_LOD1_Run.FBX", 0);
//	FBXRenderer_->CreateFBXAnimation("Idle", "Hyunwoo_01_LOD1.FBX", 0); // 루프 애니메이션
	//FBXRenderer_->CreateFBXAnimation("Skill_R_Start", "Hyunwoo_01_LOD1.FBX", 1, false); // 루프안함
	//FBXRenderer_->CreateFBXAnimation("Skill_R_Charge", "Hyunwoo_01_LOD1.FBX", 2); // 루프안함
	//FBXRenderer_->CreateFBXAnimation("Skill_R_End", "Hyunwoo_01_LOD1.FBX", 3, false); // 루프안함
	//FBXRenderer_->CreateFBXAnimation("Dash", "Hyunwoo_01_LOD1.FBX", 5, false); // 루프안함
	//FBXRenderer_->CreateFBXAnimation("Skill_Q", "Hyunwoo_01_LOD1.FBX", 8, false); // 루프안함
	//FBXRenderer_->CreateFBXAnimation("Attack0", "Hyunwoo_01_LOD1.FBX", 10, false); // 루프안함
	//FBXRenderer_->CreateFBXAnimation("Attack1", "Hyunwoo_01_LOD1.FBX", 11, false); // 루프안함
	//FBXRenderer_->CreateFBXAnimation("Skill_Weapon", "Hyunwoo_01_LOD1.FBX", 12, false); // 루프안함
	//FBXRenderer_->ChangeFBXAnimation("Idle");
}

void PJW_Hyunwoo::Init_FSM()
{
	hyunwooState_.CreateState<PJW_Hyunwoo>("Idle", this, &PJW_Hyunwoo::Idle_Start, &PJW_Hyunwoo::Idle_Update, &PJW_Hyunwoo::Idle_End);
	hyunwooState_.CreateState<PJW_Hyunwoo>("Move", this, &PJW_Hyunwoo::Move_Start, &PJW_Hyunwoo::Move_Update, &PJW_Hyunwoo::Move_End);
	hyunwooState_.CreateState<PJW_Hyunwoo>("Attack", this, &PJW_Hyunwoo::Attack_Start, &PJW_Hyunwoo::Attack_Update, &PJW_Hyunwoo::Attack_End);
	hyunwooState_.CreateState<PJW_Hyunwoo>("Skill_Q", this, &PJW_Hyunwoo::Skill_Q_Start, &PJW_Hyunwoo::Skill_Q_Update, &PJW_Hyunwoo::Skill_Q_End);
	hyunwooState_.CreateState<PJW_Hyunwoo>("Death", this, &PJW_Hyunwoo::Death_Start, &PJW_Hyunwoo::Death_Update, &PJW_Hyunwoo::Death_End);
	hyunwooState_.ChangeState("Idle");
}

void PJW_Hyunwoo::Init_Collision()
{
	GetTransform()->SetLocalPosition(float4::ZERO);

	////타격 히트 박스
	collision_Body_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_Body_->GetTransform()->SetLocalScaling(float4{ 50.f,50.f, 50.f ,1.f });

	collision_Body_->SetCollisionType(CollisionType::AABBBox3D);

	////피격 히트박스 겸사겸사 맵 컬리전도 가능할듯
	collision_Attack_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_Attack_->GetTransform()->SetLocalScaling(float4{ 100.f,0.f,100.f ,1.f });
	collision_Attack_->GetTransform()->SetLocalPosition({ 0.f,-50.f,0.f });

	collision_Attack_->SetCollisionType(CollisionType::CirCle);
}


void PJW_Hyunwoo::Update(float _DeltaTime)
{
	Check_HP();
	Check_DebugRect(_DeltaTime);
	Check_Collision(_DeltaTime);
	hyunwooState_.Update(_DeltaTime);
}	


void PJW_Hyunwoo::Check_HP()
{
	if (0.0f >= curHP_)
	{
		isMoving_ = false;
		status_MoveSpeed_ = 0.0f;
		status_ATK_ = 0.0f;
		status_HP_ = 0.0f;
		curHP_ = 0.0f;
		hyunwooState_.ChangeState("Death");
	}
}

void PJW_Hyunwoo::Check_Collision(float _DeltaTime)
{
		
}

void PJW_Hyunwoo::Idle_Start()
{
	FBXRenderer_->ChangeFBXAnimation("Idle");
	isMoving_ = false;
	isAttacking_ = false;
	return;
}

void PJW_Hyunwoo::Idle_Update(float _DeltaTime)
{
	{
		if (true == isMoving_)
		{
			if (nullptr == target_)
			{
				return;
			}

			hyunwooState_.ChangeState("Move");
			return;
		}

		if (true == isAttacking_)
		{
			hyunwooState_.ChangeState("Attack");
			return;
		}
	}
	return;
}

void PJW_Hyunwoo::Idle_End()
{
	return;
}

void PJW_Hyunwoo::Move_Start()
{
	isAttacking_ = false;
	FBXRenderer_->ChangeFBXAnimation("Move");
}

void PJW_Hyunwoo::Move_Update(float _DeltaTime)
{ 

	aimDir_ = target_->GetTransform()->GetWorldPosition() - GetTransform()->GetWorldPosition();
	// 상대 위치 - 내 위치 = 떨어진 거리
	aimDir_.Normalize3D(); // 정규화로 방향 벡터 도출

	if (nullptr != collision_Attack_->CollisionPtr(InGameCollisionType::Player2_Body))
	{
		hyunwooState_.ChangeState("Attack");
		return;
	}

	GetTransform()->SetWorldMove(aimDir_ * status_MoveSpeed_ * _DeltaTime);

	if (curDir_ == aimDir_) // 현재 방향 == 지향해야 하는 방향 이면 리턴
	{
		return;
	}

	float4 dir = float4::Cross3D(curDir_, aimDir_);

	float goaldegree = GameEngineMath::UnitVectorToDegree(aimDir_.x, aimDir_.z);

	if (dir.y >= 0.f)
	{
		GetTransform()->AddLocalRotationDegreeY(100.0f * _DeltaTime);
		// 실제 돌아가는 곳

		curDir_.RotateYDegree(100.0f * _DeltaTime);

		dir = float4::Cross3D(curDir_, aimDir_);
	}

	else if (dir.y < 0.f)
	{
		GetTransform()->AddLocalRotationDegreeY(-100.0f * _DeltaTime);
		curDir_.RotateYDegree(-100.0f * _DeltaTime);

		dir = float4::Cross3D(curDir_, aimDir_);
	}

}

void PJW_Hyunwoo::Move_End()
{
	isMoving_ = false;
}

void PJW_Hyunwoo::Attack_Start()
{
	isMoving_ = false;
	FBXRenderer_->ChangeFBXAnimation("Attack0");
}

void PJW_Hyunwoo::Attack_Update(float _DeltaTime)
{


		deltaTime_ += GameEngineTime::GetInst().GetDeltaTime();

		if (0.5f <= deltaTime_)
		{
			target_->GetDamage(status_ATK_);
			//hyunwooState_.ChangeState("Idle");
			deltaTime_ = 0.0f;
			return;
		}
}

void PJW_Hyunwoo::Attack_End()
{
	isAttacking_ = false;
}


void PJW_Hyunwoo::Skill_Q_Start()
{
	//FBXRenderer_->ChangeFBXAnimation("Skill_Q");
	deltaTime_ = 0.0f;
	return;
}

void PJW_Hyunwoo::Skill_Q_Update(float _DeltaTime)
{
	deltaTime_ += GameEngineTime::GetInst().GetDeltaTime();

	if (0.6f <= deltaTime_)
	{
		hyunwooState_.ChangeState("Idle");
		return;
	}

	return;
}

void PJW_Hyunwoo::Skill_Q_End()
{
	return;
}

void PJW_Hyunwoo::Death_Start()
{
	//FBXRenderer_->ChangeFBXAnimation("Death");
	deltaTime_ = 0.0f;
}
void PJW_Hyunwoo::Death_Update(float _DeltaTime)
{
	deltaTime_ += GameEngineTime::GetInst().GetDeltaTime();

	if (1.8f <= deltaTime_)
	{
		Death();
		return;
	}

	return;
}

void PJW_Hyunwoo::Death_End()
{

}







void PJW_Hyunwoo::Check_DebugRect(float _DeltaTime)
{
	GetLevel()->PushDebugRender(collision_Body_->GetTransform(), CollisionType::AABBBox3D);
	GetLevel()->PushDebugRender(collision_Attack_->GetTransform(), CollisionType::CirCle);
}

