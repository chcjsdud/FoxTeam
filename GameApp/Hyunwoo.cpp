#include "PreCompile.h"
#include "Hyunwoo.h"
#include "PlayerInfoManager.h"
#include <GameEngine/GameEngineCollision.h>
#include "CharStatPacket.h"
#include <GameEngine/GameEngineFBXRenderer.h>
#include "GameServer.h"
#include "GameClient.h"
#include <GameEngine/GameEngineLevelControlWindow.h>
#include "MousePointer.h"
#include "LumiaMap.h"
#include "Character.h"
#include "CharCrowdControlPacket.h"
Hyunwoo::Hyunwoo()
	: timer_collision_Q(0.0f), timer_end_Q(0.0f), collision_Q(nullptr), b_Qhit_(false), timer_Dash_E(0.0f), b_Ehit_(false), collision_E(nullptr), atkFlag_(false),
	  b_Rhit_(false), collision_R(nullptr), collisionRRate_(0.0f), b_Dhit_(false)
{

}

Hyunwoo::~Hyunwoo()
{

}

void Hyunwoo::LoadResource()
{
	GameEngineDirectory dir;

	dir.MoveParent("FoxTeam");
	dir / "Resources" / "FBX" / "PJW";

	GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_run.fbx"));
	mesh->CreateRenderingBuffer();

	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_run.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_wait.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_death.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_atk0.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_atk1.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_skillQ.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_skillE_start.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_skillE_loop.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_skillE_end.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_skillR_start.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_skillR_loop.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_skillR_end.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_weaponskill.fbx"));
}

void Hyunwoo::ReleaseResource()
{
	GameEngineFBXMeshManager::GetInst().Delete("hyunwoo_run.fbx");

	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_run.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_wait.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_death.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_atk0.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_atk1.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_skillQ.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_skillE_start.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_skillE_loop.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_skillE_end.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_skillR_start.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_skillR_loop.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_skillR_end.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_weaponskill.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_weaponskill.fbx");
}

void Hyunwoo::Start()
{
	Character::Start();
	initHyunwooCollision();
	initHyunwooCustomState();
	stat_.AttackStartTime = 0.15f;
}

void Hyunwoo::Update(float _deltaTime)
{
	Character::Update(_deltaTime);

	GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
	if (nullptr != controlWindow)
	{
		GetLevel()->PushDebugRender(collision_Q->GetTransform(), CollisionType::AABBBox3D, float4::BLUE);
		controlWindow->AddText("P1Dir" + std::to_string(direction_.x) + ", " + std::to_string(direction_.z));
		controlWindow->AddText("P1Pos " + std::to_string(collision_->GetTransform()->GetWorldPosition().x) + " , " + std::to_string(collision_->GetTransform()->GetWorldPosition().z));
		controlWindow->AddText("ColQpos " + std::to_string(collision_Q->GetTransform()->GetWorldPosition().x) + ", " + std::to_string(collision_Q->GetTransform()->GetWorldPosition().z));

		float4 position = MousePointer::InGameMouse->GetIntersectionYAxisPlane(0, 50000.f);
		controlWindow->AddText("MPos : " + std::to_string(position.x) + ", " + std::to_string(position.z));
	}
}

void Hyunwoo::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh("hyunwoo_run.fbx", "TextureDeferredLightAni");

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	renderer_->CreateFBXAnimation("Run", "hyunwoo_run.fbx", 0);
	renderer_->CreateFBXAnimation("Wait", "hyunwoo_wait.fbx", 0);
	renderer_->CreateFBXAnimation("Death", "hyunwoo_death.fbx", 0, false);
	renderer_->CreateFBXAnimation("Atk0", "hyunwoo_atk0.fbx", 0, false);
	renderer_->CreateFBXAnimation("SkillD", "hyunwoo_weaponskill.fbx", 0, false);
	renderer_->CreateFBXAnimation("Atk1", "hyunwoo_atk1.fbx", 0, false);
	renderer_->CreateFBXAnimation("SkillQ", "hyunwoo_skillQ.fbx", 0, false);
	renderer_->CreateFBXAnimation("SkillE_start", "hyunwoo_skillE_start.fbx", 0, false);
	renderer_->CreateFBXAnimation("SkillE_loop", "hyunwoo_skillE_loop.fbx", 0);
	renderer_->CreateFBXAnimation("SkillE_end", "hyunwoo_skillE_end.fbx", 0, false);
	renderer_->CreateFBXAnimation("SkillR_start", "hyunwoo_skillR_start.fbx", 0, false);
	renderer_->CreateFBXAnimation("SkillR_loop", "hyunwoo_skillR_loop.fbx", 0);
	renderer_->CreateFBXAnimation("SkillR_end", "hyunwoo_skillR_end.fbx", 0, false);

	renderer_->ChangeFBXAnimation("Wait");
}


void Hyunwoo::initHyunwooCollision()
{
	collision_Q = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_Q->GetTransform()->SetLocalPosition({ 0.f,0.f,300.f });
	collision_Q->GetTransform()->SetLocalScaling({ 10.0f, 10.0f, 10.0f });
	collision_Q->SetCollisionGroup(eCollisionGroup::PlayerAttack);
	collision_Q->SetCollisionType(CollisionType::OBBBox3D);
	collision_Q->Off();

	collision_E = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_E->GetTransform()->SetLocalPosition({ 0.f,0.f,0.f });
	collision_E->GetTransform()->SetLocalScaling({ 150.f });
	collision_E->SetCollisionGroup(eCollisionGroup::PlayerAttack);
	collision_E->SetCollisionType(CollisionType::OBBBox3D);
	collision_E->Off();

	collision_R = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_R->GetTransform()->SetLocalPosition({ 0.f,0.f,150.f });
	collision_R->GetTransform()->SetLocalScaling({200.0f, 10.0f, 100.0f});
	collision_R->SetCollisionGroup(eCollisionGroup::PlayerAttack);
	collision_R->SetCollisionType(CollisionType::OBBBox3D);
	collision_R->Off();

}

void Hyunwoo::initHyunwooCustomState()
{
	customState_.CreateState(MakeStateWithEnd(Hyunwoo, CustomRSkill));

	customState_ << "CustomRSkill";
}

void Hyunwoo::changeAnimationRun()
{
	curAnimationName_ = "Run";
	renderer_->ChangeFBXAnimation("Run");
}

void Hyunwoo::changeAnimationWait()
{
	curAnimationName_ = "Wait";
	renderer_->ChangeFBXAnimation("Wait");
}

void Hyunwoo::changeAnimationBasicAttack()
{
	if (false == atkFlag_)
	{
		curAnimationName_ = "Atk0";
		renderer_->ChangeFBXAnimation("Atk0", true);
		atkFlag_ = true;
		return;
	}
	else
	{
		curAnimationName_ = "Atk1";
		renderer_->ChangeFBXAnimation("Atk1", true);
		atkFlag_ = false;
		return;
	}
}

void Hyunwoo::onStartQSkill()
{
	timer_collision_Q = 0.0f;
	timer_end_Q = 0.0f;
	b_Qhit_ = false;

	curAnimationName_ = "SkillQ";
	renderer_->ChangeFBXAnimation("SkillQ", true);

	collision_Q->On();
}

void Hyunwoo::onUpdateQSkill(float _deltaTime)
{
	PlayerInfoManager* pm = PlayerInfoManager().GetInstance();

	if (true == collision_Q->IsUpdate())
	{
		GetLevel()->PushDebugRender(collision_Q->GetTransform(), CollisionType::OBBBox3D, float4::BLUE);
	}

	timer_collision_Q += _deltaTime;
	timer_end_Q += _deltaTime;

	if (true == b_Qhit_)
	{
		collision_Q->Off();
	}

	if (1.05f <= timer_end_Q)
	{
		// 모든 카운터 초기화
		timer_collision_Q = 0.0f;
		timer_end_Q = 0.0f;
		b_Qhit_ = false;
		changeAnimationWait();
		mainState_.ChangeState("NormalState", true);
		normalState_.ChangeState("Watch", true);
		return;
	}

	if (0.3f <= timer_collision_Q && false == b_Qhit_)
	{
		// 여기서 피격 충돌 판정이 나옴


		auto collisionList = collision_Q->GetCollisionList(eCollisionGroup::Player);

		for (GameEngineCollision* col : collisionList)
		{
			GameEngineActor* actor = col->GetActor();
			Character* character = nullptr;
			if (nullptr != actor && actor != this)
			{
				character = dynamic_cast<Character*>(actor);

				if (nullptr != character)
				{
					character->Damage(300.0f);
				}
			}
		}

		b_Qhit_ = true;
	}

	// 델타타임에 걸맞게 
	/*피해량:50/100/150/200/250(+공격력의 40%)(+스킬 증폭의 55%)
	이동 속도 감소: -40%
	사정거리: 3m
	스테미너 소모: 60/70/80/90/100
	시전 시간: 즉시
	둔화 지속 시간: 2초
	쿨다운: 8.5/7.5/6.5/5.5/4.5초*/


	// 0.3초 쯤의 시전딜레이가 확인됨.
	// 정확히 말하자면 즉발은 맞지만, 타격 이펙트가 생기는 부분은 0.3초 이후

}

void Hyunwoo::onStartWSkill()
{
}

void Hyunwoo::onUpdateWSkill(float _deltaTime)
{
	// W 스킬은 일시적인 방어력 증강과
	// 모든 군중 제어기 면역을 일시적으로 부여하는 스테이트입니다.
}

void Hyunwoo::onStartESkill()
{
	timer_Dash_E = 0.0f;
	b_Ehit_ = false;
	collision_E->On();

	float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.0f);
	direction_ = mousePosition - GetTransform()->GetWorldPosition(); // 커서 위치가 돌진 방향이 된다.
	direction_.Normalize3D();

	float4 cross = float4::Cross3D(direction_, { 0.0f, 0.0f, 1.0f });
	cross.Normalize3D();

	float angle = float4::DegreeDot3DToACosAngle(direction_, { 0.0f, 0.0f, 1.0f });

	transform_.SetLocalRotationDegree({ 0.0f, angle * -cross.y, 0.0f });

	curAnimationName_ = "SkillE_start";
	renderer_->ChangeFBXAnimation("SkillE_start", true);
}

void Hyunwoo::onUpdateESkill(float _deltaTime)
{
	if (true == collision_E->IsUpdate())
	{
		GetLevel()->PushDebugRender(collision_E->GetTransform(), CollisionType::OBBBox3D, float4::WHITE);
	}

	if (true == renderer_->IsCurrentAnimationEnd())
	{
		curAnimationName_ = "SkillE_loop";
		renderer_->ChangeFBXAnimation("SkillE_loop", true);
	}

	float4 dashSpeed = direction_ * 1000.f * _deltaTime;
	float4 nextMovePosition = GetTransform()->GetWorldPosition() + dashSpeed;
	timer_Dash_E += _deltaTime;

	float temp;
	if (true == currentMap_->GetNavMesh()->CheckIntersects(nextMovePosition + float4{ 0.0f, FT::Map::MAX_HEIGHT, 0.0f }, float4::DOWN, temp))
	{
		if (0.5f <= timer_Dash_E)
		{
			timer_Dash_E = 0.0f;
			collision_E->Off();
			// 대쉬가 끝났다.
			destination_ = GetTransform()->GetWorldPosition();
			destinations_.clear();
			// 목적지를 현 위치로 만들어 주고 클리어

			b_Ehit_ = false;
			changeAnimationWait();
			mainState_.ChangeState("NormalState", true);
			normalState_.ChangeState("Watch", true);
			return;
		}


		// 여기서 피격 충돌 판정이 나옴
		auto collisionList = collision_E->GetCollisionList(eCollisionGroup::Player);

		if (false == b_Ehit_)
		{
			for (GameEngineCollision* col : collisionList)
			{
				GameEngineActor* actor = col->GetActor();
				Character* character = nullptr;

				if (nullptr != actor && actor != this)
				{
					character = dynamic_cast<Character*>(actor);

					if (nullptr != character)
					{
						character->Damage(150.0f);
						CharCrowdControlPacket ccPacket;
						ccPacket.SetTargetIndex(character->GetIndex());
						ccPacket.SetWallSlam(0.2f, direction_ * 3000.f, 1.0f);

						if (true == GameServer::GetInstance()->IsOpened())
						{
							GameServer::GetInstance()->Send(&ccPacket);
							//GameServer::GetInstance()->Send(&packet);
						}
						else if (true == GameClient::GetInstance()->IsConnected())
						{
							GameClient::GetInstance()->Send(&ccPacket);
							//GameClient::GetInstance()->Send(&packet);
						}

						b_Ehit_ = true;
					}
				}
			}
		}

		GetTransform()->SetWorldPosition(nextMovePosition);
	}
	else
	{
		timer_Dash_E = 0.0f;
		collision_E->Off();
		b_Ehit_ = false;

		destination_ = GetTransform()->GetWorldPosition();
		destinations_.clear();

		changeAnimationWait();
		mainState_.ChangeState("NormalState", true);
		normalState_.ChangeState("Watch", true);
	}

	/*  처음 적과 부딪힐 때 피해: (+공격력의 70%)(+스킬 증폭의 30%)(+적 현재 체력의 5/8/11/14/17%)
		벽에 부딪힐 시 추가 피해:60/95/130/165/200(+공격력의 55%)(+스킬 증폭의 60%)
		방어력 감소: 4/6/8/10/12%
		사정거리: 5m
		스테미너 소모: 70/75/80/85/90
		기절 지속 시간: 1.5초
		쿨다운: 18/17/16/15/14초  */

	// 아이템 조합
	// 아이템 스탯 계산

	// 공격력 방어력 이속 공격속도
}

void Hyunwoo::onStartRSkill()
{
	mainState_.ChangeState("CustomState", true);
	customState_.ChangeState("CustomRSkill", true);

}

void Hyunwoo::onUpdateRSkill(float _deltaTime)
{
}

void Hyunwoo::onStartDSkill()
{


	// 여기서 마우스와 상대의 타겟팅 여부를 찾을 것.

	if (mouse_ == nullptr)
	{
		mainState_.ChangeState("NormalState", true);
		normalState_.ChangeState("Watch", true);
		return;
	}


}

void Hyunwoo::onUpdateDSkill(float _deltaTime)
{
	bool result = false;
	if (false == b_Dhit_)
	{
		GameEngineCollision* rayCol = mouse_->GetRayCollision();


		float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);

		Character* otherCharacter = getMousePickedCharacter();
		target_ = otherCharacter;

		if (nullptr != otherCharacter && otherCharacter != this && false == b_Dhit_)
		{
			// 공격 처리
			float4 targetPosition = target_->GetTransform()->GetWorldPosition();
			float4 playerPosition = transform_.GetWorldPosition();
			float distance = float4::Calc_Len3D(playerPosition, targetPosition);

			if (distance > stat_.AttackRange)
			{	// 사거리 너무 멀 때
				mainState_.ChangeState("NormalState", true);
				normalState_.ChangeState("Chase", true);
				return;
			}

			curAnimationName_ = "SkillD";
			renderer_->ChangeFBXAnimation("SkillD", true);
			otherCharacter->Damage(150.0f);
			// 

			b_Dhit_ = true;
			normalState_ << "Chase";
			return;
		}
		else
		{
			target_ = nullptr;
			result = currentMap_->GetNavMesh()->GetIntersectionPointFromMouseRay(destination_);

			changeAnimationWait();
			mainState_.ChangeState("NormalState", true);
			normalState_.ChangeState("Watch", true);

			return;
		}

	}

	if ("SkillD" == curAnimationName_ && true == renderer_->IsCurrentAnimationEnd())
	{
		b_Dhit_ = false;

		changeAnimationWait();
		mainState_.ChangeState("NormalState", true);
		normalState_.ChangeState("Watch", true);
	}
}

void Hyunwoo::onStartDeath()
{
	curAnimationName_ = "Death";
	renderer_->ChangeFBXAnimation("Death", true);
}

void Hyunwoo::onUpdateDeath(float _deltaTime)
{

}

void Hyunwoo::onUpdateCustomState(float _deltaTime)
{
	customState_.Update(_deltaTime);
}

void Hyunwoo::startCustomRSkill()
{
	curAnimationName_ = "SkillR_start";
	renderer_->ChangeFBXAnimation("SkillR_start", true);
	collision_R->On();
}

void Hyunwoo::updateCustomRSkill(float _deltaTime)
{
	if (true == collision_R->IsUpdate())
	{
		GetLevel()->PushDebugRender(collision_R->GetTransform(), CollisionType::OBBBox3D, float4::RED);
	}

	if (curAnimationName_ == "SkillR_start" && renderer_->IsCurrentAnimationEnd())
	{
		curAnimationName_ = "SkillR_loop";
		renderer_->ChangeFBXAnimation("SkillR_loop", true);
	}

	// 여기에 차징 대미지 증감 & 충돌체 크기 증감 코드 들어갈 예정
	if (false == b_Rhit_)
	{
		collisionRRate_ += _deltaTime;
		collision_R->GetTransform()->SetLocalPosition({ 0.f,0.f,150.f + collisionRRate_ * 50.0f });
		collision_R->GetTransform()->SetLocalScaling({ 350.0f, 10.0f, 100.0f + collisionRRate_ * 100.0f });
	}

	if (true == GameEngineInput::GetInst().Down("R"))
	{
		if (false == b_Rhit_)
		{
			// 여기서 피격 충돌 판정이 나옴
			auto collisionList = collision_R->GetCollisionList(eCollisionGroup::Player);

			for (GameEngineCollision* col : collisionList)
			{
				GameEngineActor* actor = col->GetActor();
				Character* character = nullptr;
				if (nullptr != actor && actor != this)
				{
					character = dynamic_cast<Character*>(actor);

					if (nullptr != character)
					{
						character->Damage(500.0f);
					}
				}
			}
			b_Rhit_ = true;

		}

		if (curAnimationName_ != "SkillR_end")
		{
			curAnimationName_ = "SkillR_end";
			renderer_->ChangeFBXAnimation("SkillR_end", true);
		}
	}

	if (curAnimationName_ == "SkillR_end" && true == renderer_->IsCurrentAnimationEnd())
	{
		b_Rhit_ = false;

		changeAnimationWait();
		mainState_.ChangeState("NormalState", true);
		normalState_.ChangeState("Watch", true);
	}
	/*피해량: 80/120/160(+공격력의 75%)(+스킬 증폭의 85%) ~ 480/720/960(+공격력의 225%)(+스킬 증폭의 255%)
사정거리: 3m ~ 4.2m
스테미너 소모: 150
시전 시간: 1.2 ~ 3초
쿨다운: 65/55/50초*/
}

void Hyunwoo::endCustomRSkill()
{
	b_Rhit_ = false;
	collisionRRate_ = 0.0f;
	collision_R->Off();
	collision_R->GetTransform()->SetLocalScaling({ 350.0f, 10.0f, 100.0f + collisionRRate_ });
}

void Hyunwoo::onStartBasicAttacking(Character* _target)
{
	target_->Damage(stat_.AttackPower);
}

void Hyunwoo::onUpdateBasicAttacking(Character* _target, float _deltaTime)
{
}
