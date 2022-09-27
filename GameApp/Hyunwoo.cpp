#include "PreCompile.h"
#include "Hyunwoo.h"
#include "PlayerInfoManager.h"
#include <GameEngine/GameEngineCollision.h>
#include "CharStatPacket.h"

#include "GameServer.h"
#include "GameClient.h"

Hyunwoo::Hyunwoo()
	: timer_collision_Q(0.0f), timer_end_Q(0.0f), collision_Q(nullptr), b_Qhit_(false)
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
	PlayerInfoManager::GetInstance()->GetMyPlayer().stat_ = &actorStat_;
}

void Hyunwoo::Update(float _deltaTime)
{
	Character::Update(_deltaTime);

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
	collision_Q->GetTransform()->SetLocalPosition({ 0.0f,0.0f,200.0f });
	collision_Q->GetTransform()->SetLocalScaling({300.0f, 1.0f, 250.0f});
	collision_Q->SetCollisionGroup(eCollisionGroup::PlayerAttack);
	collision_Q->SetCollisionType(CollisionType::AABBBox3D);
	collision_Q->Off();

}

void Hyunwoo::changeAnimationRun()
{
	curAnimation_ = "Run";
	renderer_->ChangeFBXAnimation("Run");
}

void Hyunwoo::changeAnimationWait()
{
	curAnimation_ = "Wait";
	renderer_->ChangeFBXAnimation("Wait");
}

void Hyunwoo::changeAnimationBasicAttack()
{
	curAnimation_ = "Atk0";
	renderer_->ChangeFBXAnimation("Atk0");
}




void Hyunwoo::onStartQSkill()
{
	curAnimation_ = "SkillQ";
	renderer_->ChangeFBXAnimation("SkillQ", true);
}

void Hyunwoo::onUpdateQSkill(float _deltaTime)
{
	GetLevel()->PushDebugRender(collision_Q->GetTransform(), CollisionType::AABBBox3D, float4::BLUE);
	timer_collision_Q += _deltaTime;
	timer_end_Q += _deltaTime;

	if (true == b_Qhit_)
	{
		collision_Q->Off();
	}

	if (0.3f <= timer_collision_Q && false == b_Qhit_)
	{
		// ���⼭ �ǰ� �浹 ������ ����
		collision_Q->On();

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

					character->Damage(100.0f);

					CharStatPacket packet;
					packet.SetTargetIndex(character->GetIndex());
					packet.SetStat(character->GetStat());
					
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


	if (1.05f <= timer_end_Q)
	{
		// ��� ī���� �ʱ�ȭ
		timer_collision_Q = 0.0f;
		timer_end_Q = 0.0f;
		b_Qhit_ = false;
		changeAnimationWait();
		mainState_ << "NormalState";
		normalState_ << "Watch";
		return;
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
}

void Hyunwoo::onUpdateESkill(float _deltaTime)
{
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
	curAnimation_ = "Death";
	renderer_->ChangeFBXAnimation("Death");
}

void Hyunwoo::onUpdateDeath(float _deltaTime)
{
}

void Hyunwoo::InitHyunwooCollision()
{
	collision_Q = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_Q->GetTransform()->SetLocalScaling(120.0f);
	collision_Q->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 50.0f });
	collision_Q->SetCollisionGroup(eCollisionGroup::PlayerAttack);
	collision_Q->SetCollisionType(CollisionType::AABBBox3D);
	collision_Q->Off();
}