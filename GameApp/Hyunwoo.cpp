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
	: timer_collision_Q(0.0f), timer_end_Q(0.0f), collision_Q(nullptr), b_Qhit_(false), timer_Dash_E(0.0f), b_Ehit_(false), collision_E(nullptr)
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

}

void Hyunwoo::Start()
{
	Character::Start();
	initHyunwooCollision();
	actorStat_.AttackStartTime = 0.15f;
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
	//renderer_->CreateFBXAnimation("Atk1", "hyunwoo_atk1.fbx", 0, false);
	renderer_->CreateFBXAnimation("SkillQ", "hyunwoo_skillQ.fbx", 0, false);
	renderer_->CreateFBXAnimation("SkillE_start", "hyunwoo_skillE_start.fbx", 0, false);
	renderer_->CreateFBXAnimation("SkillE_loop", "hyunwoo_skillE_loop.fbx", 0);
	renderer_->CreateFBXAnimation("SkillE_end", "hyunwoo_skillE_end.fbx", 0, false);
	//renderer_->CreateFBXAnimation("SkillR_start", "hyunwoo_skillR_start.fbx", 0, false);
	//renderer_->CreateFBXAnimation("SkillR_loop", "hyunwoo_skillR_loop.fbx", 0);
	//renderer_->CreateFBXAnimation("SkillR_end", "hyunwoo_skillR_end.fbx", 0, false);

	renderer_->ChangeFBXAnimation("Wait");

	//	tempRenderer_ = CreateTransformComponent<GameEngineRenderer>(GetTransform());
	//	tempRenderer_->SetRenderingPipeLine("DebugBox");
	//	tempRenderer_->GetTransform()->SetLocalPosition({ 0.0f,0.0f,0.0f });
	//	tempRenderer_->GetTransform()->SetLocalScaling({ 10.f,10.f,10.f });
	//
	//	tempRenderer_->ShaderHelper.SettingConstantBufferLink("ResultColor", float4::GREEN);

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
	curAnimationName_ = "Atk0";
	renderer_->ChangeFBXAnimation("Atk0", true);
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
		// ��� ī���� �ʱ�ȭ
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
		// ���⼭ �ǰ� �浹 ������ ����


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
					// ���⿡ ���Դٴ� �� ���𰡿� ������� ��ٴ� ���̴�.
					// ����� ��Ŷ : Playerinfo �� chracterstat ����ü�� �ִ� ü�� ���� �����Ѵ�.
					// �̴� ���� �����Ӹ��� LumiaLevel �� ���μ��� ��Ŷ���� �ǽð����� �����Ѵ�.

					// �浹ü�� ������ ��󿡰� ���� ������� ���ϴ� �ڵ��Դϴ�.
					character->Damage(300.0f);
					//pm->GetPlayerList()[character->GetIndex()].stat_->HP -= 300.0f;



					// ������� ������ Status �� ��Ŷ���� �Ѱ��ݴϴ�.
					CharStatPacket packet;
					packet.SetTargetIndex(character->GetIndex());
					packet.SetStat(*(character->GetStat()));

					if (true == GameServer::GetInstance()->IsOpened())
					{
						GameServer::GetInstance()->Send(&packet);
					}
					else if (true == GameClient::GetInstance()->IsConnected())
					{
						GameClient::GetInstance()->Send(&packet);
					}
				}
			}
		}

		b_Qhit_ = true;
	}



	// ��ŸŸ�ӿ� �ɸ°� 
	/*���ط�:50/100/150/200/250(+���ݷ��� 40%)(+��ų ������ 55%)
	�̵� �ӵ� ����: -40%
	�����Ÿ�: 3m
	���׹̳� �Ҹ�: 60/70/80/90/100
	���� �ð�: ���
	��ȭ ���� �ð�: 2��
	��ٿ�: 8.5/7.5/6.5/5.5/4.5��*/


	// 0.3�� ���� ���������̰� Ȯ�ε�.
	// ��Ȯ�� �����ڸ� ����� ������, Ÿ�� ����Ʈ�� ����� �κ��� 0.3�� ����

}

void Hyunwoo::onStartWSkill()
{
}

void Hyunwoo::onUpdateWSkill(float _deltaTime)
{
}

void Hyunwoo::onStartESkill()
{
	timer_Dash_E = 0.0f;
	b_Ehit_ = false;
	collision_E->On();

	float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.0f);
	direction_ = mousePosition - GetTransform()->GetWorldPosition(); // Ŀ�� ��ġ�� ���� ������ �ȴ�.
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
			// �뽬�� ������.
			destination_ = GetTransform()->GetWorldPosition();
			destinations_.clear();
			// �������� �� ��ġ�� ����� �ְ� Ŭ����

			b_Ehit_ = false;
			changeAnimationWait();
			mainState_.ChangeState("NormalState", true);
			normalState_.ChangeState("Watch", true);
			return;
		}


		// ���⼭ �ǰ� �浹 ������ ����
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
						//character->WallSlam(0.2f, direction_ * 3000.f, 0.5f);
						CharCrowdControlPacket ccPacket;
						ccPacket.SetTargetIndex(character->GetIndex());
						ccPacket.SetWallSlam(0.2f, direction_ * 3000.f, 0.5f);

						//character->dirHyunwooE_ = direction_;
						//character->mainState_.ChangeState("CrowdControlState", true);
						//character->crowdControlState_.ChangeState("HyunwooE", true);

						CharStatPacket packet;
						packet.SetTargetIndex(character->GetIndex());
						packet.SetStat(*(character->GetStat()));

						if (true == GameServer::GetInstance()->IsOpened())
						{
							GameServer::GetInstance()->Send(&ccPacket);
							GameServer::GetInstance()->Send(&packet);
						}
						else if (true == GameClient::GetInstance()->IsConnected())
						{
							GameClient::GetInstance()->Send(&ccPacket);
							GameClient::GetInstance()->Send(&packet);
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

	/*  ó�� ���� �ε��� �� ����: (+���ݷ��� 70%)(+��ų ������ 30%)(+�� ���� ü���� 5/8/11/14/17%)
		���� �ε��� �� �߰� ����:60/95/130/165/200(+���ݷ��� 55%)(+��ų ������ 60%)
		���� ����: 4/6/8/10/12%
		�����Ÿ�: 5m
		���׹̳� �Ҹ�: 70/75/80/85/90
		���� ���� �ð�: 1.5��
		��ٿ�: 18/17/16/15/14��  */




}

void Hyunwoo::onStartRSkill()
{
}

void Hyunwoo::onUpdateRSkill(float _deltaTime)
{
}

void Hyunwoo::onStartDSkill()
{
}

void Hyunwoo::onUpdateDSkill(float _deltaTime)
{
}

void Hyunwoo::onStartDeath()
{
	curAnimationName_ = "Death";
	renderer_->ChangeFBXAnimation("Death", true);
}

void Hyunwoo::onUpdateDeath(float _deltaTime)
{

}

