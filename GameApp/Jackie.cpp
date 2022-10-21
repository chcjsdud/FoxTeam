#include "PreCompile.h"
#include "Jackie.h"
#include "PlayerInfoManager.h"
#include <GameEngine/GameEngineCollision.h>
#include "CharStatPacket.h"
#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineEffectRenderer.h>
#include "GameServer.h"
#include "GameClient.h"
#include <GameEngine/GameEngineLevelControlWindow.h>
#include "MousePointer.h"
#include "LumiaMap.h"
#include "Character.h"
#include "CharCrowdControlPacket.h"
#include "PacketSoundPlay.h"
#include "CharEffectPacket.h"

Jackie::Jackie() // default constructer 디폴트 생성자
	: atkFlag_(false), timer_collision_Q(0.0f), timer_end_Q(0.0f), b_Qhit_(false), collision_Q(nullptr),
	timer_collision_E(0.0f), timer_end_E(0.0f), b_Ehit_(false), collision_E(nullptr),
	basicAttackEffectRenderer_(nullptr)
{

}

Jackie::~Jackie() // default destructer 디폴트 소멸자
{

}

Jackie::Jackie(Jackie&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void Jackie::LoadResource()
{
	GameEngineDirectory dir;

	dir.MoveParent("FoxTeam");
	dir / "Resources" / "FBX" / "PJW";

	GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Jackie_run.fbx"));
	mesh->CreateRenderingBuffer();

	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_run.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_wait.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_death.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_atk1.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_atk2.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_skillQ.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_skillW.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_skillE.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_weaponSkill.fbx"));

	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_R_atk1.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_R_atk2.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_R_run.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_R_skillE.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_R_skillQ.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_R_skillW.fbx"));

	{
		//GameEngineDirectory dir;
		//dir.MoveParent("FoxTeam");
		//dir / "Resources" / "Sound" / "Char" / "Hyunwoo";
		//
		//std::vector<GameEngineFile> allFile = dir.GetAllFile("wav");
		//for (GameEngineFile& file : allFile)
		//{
		//	GameEngineSoundManager::GetInstance()->CreateSound(file.FileName(), file.GetFullPath());
		//}
	}
}

void Jackie::ReleaseResource()
{
	{
		//GameEngineDirectory dir;
		//dir.MoveParent("FoxTeam");
		//dir / "Resources" / "Sound" / "Char" / "Hyunwoo";
		//
		//std::vector<GameEngineFile> allFile = dir.GetAllFile("wav");
		//for (GameEngineFile& file : allFile)
		//{
		//	GameEngineSoundManager::GetInstance()->ReleaseSound(file.FileName());
		//}
	}

	GameEngineFBXMeshManager::GetInst().Delete("Jackie_run.fbx");

	GameEngineFBXAnimationManager::GetInst().Delete("Jackie_run.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("Jackie_wait.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("Jackie_death.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("Jackie_atk1.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("Jackie_atk2.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("Jackie_skillQ.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("Jackie_skillW.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("Jackie_skillE.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("Jackie_weaponSkill.fbx");

	GameEngineFBXAnimationManager::GetInst().Delete("Jackie_R_atk1.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("Jackie_R_atk2.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("Jackie_R_run.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("Jackie_R_skillE.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("Jackie_R_skillQ.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("Jackie_R_skillW.fbx");
}

void Jackie::Start()
{
	Character::Start();
	initJackieCollision();
	initJackieCustomState();
	initEffectRenderer();

	stat_.HPMax = 880.0f;
	stat_.HP = 880.0f;
	stat_.SPMax = 430.0f;
	stat_.SP = 430.0f;
	stat_.Defence = 31.0f;
	stat_.AttackPower = 43.0f;
	stat_.HPRegeneration = 1.275f;
	stat_.SPRegeneration = 2.1f;

	stat_.AttackSpeed = 0.92f;
	stat_.AttackStartTime = 0.15f;
	stat_.AttackEndTime = 0.4f;
	stat_.MovementSpeed = 350.0f;
	stat_.AttackRange = 280.f;

	//GameEngineTexture* hitBase = GameEngineTextureManager::GetInst().Find("FX_BI_Hit_05.png");
	//hitBase->Cut(3, 3);
}

void Jackie::Update(float _deltaTime)
{
	Character::Update(_deltaTime);
}

void Jackie::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh("Jackie_run.fbx", "TextureDeferredLightAni");

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	renderer_->CreateFBXAnimation("Run", "Jackie_run.fbx", 0);
	renderer_->CreateFBXAnimation("Wait", "Jackie_wait.fbx", 0);
	renderer_->CreateFBXAnimation("Death", "Jackie_death.fbx", 0, false);
	renderer_->CreateFBXAnimation("Atk0", "Jackie_atk1.fbx", 0, false);
	renderer_->CreateFBXAnimation("SkillD", "Jackie_weaponSkill.fbx", 0, false);
	renderer_->CreateFBXAnimation("Atk1", "Jackie_atk2.fbx", 0, false);
	renderer_->CreateFBXAnimation("SkillQ", "Jackie_skillQ.fbx", 0, false);
	renderer_->CreateFBXAnimation("SkillE", "Jackie_skillE.fbx", 0, false);

	renderer_->CreateFBXAnimation("SkillW", "Jackie_skillW.fbx", 0);

	//renderer_->CreateFBXAnimation("SkillR_wait", "Jackie_R_wait.fbx", 0);
	//renderer_->CreateFBXAnimation("SkillR_run", "Jackie_R_run.fbx", 0);
	//renderer_->CreateFBXAnimation("R_Atk0", "Jackie_R_atk1.fbx", 0, false);
	//renderer_->CreateFBXAnimation("R_Atk1", "Jackie_R_atk2.fbx", 0, false);
	//renderer_->CreateFBXAnimation("R_SkillQ", "Jackie_R_skillQ.fbx", 0, false);
	//renderer_->CreateFBXAnimation("R_SkillW", "Jackie_R_skillW.fbx", 0);
	//renderer_->CreateFBXAnimation("R_SkillE", "Jackie_R_skillE.fbx", 0, false);

	renderer_->ChangeFBXAnimation("Wait");
}

void Jackie::initJackieCollision()
{
	collision_Q = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_Q->GetTransform()->SetLocalPosition({ 0.f,0.f,300.f });
	collision_Q->GetTransform()->SetLocalScaling({ 450.0f, 10.0f, 300.0f });
	collision_Q->SetCollisionGroup(eCollisionGroup::PlayerAttack);
	collision_Q->SetCollisionType(CollisionType::OBBBox3D);
	collision_Q->Off();
	
	collision_E = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_E->GetTransform()->SetLocalScaling({ 450.0f, 10.0f, 450.0f });
	collision_E->SetCollisionGroup(eCollisionGroup::PlayerAttack);
	collision_E->SetCollisionType(CollisionType::OBBBox3D);
	collision_E->Off();
}

void Jackie::initJackieCustomState()
{
}

void Jackie::initEffectRenderer()
{
	basicAttackEffectRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());

	basicAttackEffectRenderer_->SetImage("FX_BI_Hit_061.png");
	basicAttackEffectRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 100.0f, stat_.AttackRange - 180.f });
	basicAttackEffectRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	basicAttackEffectRenderer_->GetTransform()->SetLocalScaling(basicAttackEffectRenderer_->GetCurrentTexture()->GetTextureSize() / 3);
	basicAttackEffectRenderer_->CreateAnimation("FX_BI_Hit_061.png", "FX_BI_Hit_061", 0, 3, 0.04f, false);
	basicAttackEffectRenderer_->Off();
}

void Jackie::changeAnimationRun()
{
	curAnimationName_ = "Run";
	renderer_->ChangeFBXAnimation("Run");
}

void Jackie::changeAnimationWait()
{
	curAnimationName_ = "Wait";
	renderer_->ChangeFBXAnimation("Wait");
}

void Jackie::changeAnimationBasicAttack()
{
	if (false == atkFlag_)
	{
		curAnimationName_ = "Atk0";
		renderer_->ChangeFBXAnimation("Atk0", true);
		atkFlag_ = true;
	}
	else
	{
		curAnimationName_ = "Atk1";
		renderer_->ChangeFBXAnimation("Atk1", true);
		atkFlag_ = false;
	}

	//GameEngineSoundManager::GetInstance()->PlaySoundByName("attackGlove_Normal01.wav");
	//PacketSoundPlay packet;
	//packet.SetSound("attackGlove_Normal01.wav", transform_.GetWorldPosition());
	//FT::SendPacket(packet);
}

void Jackie::changeDeathAnimation()
{
	ChangeAnimation("Death", true);
}

void Jackie::onStartBasicAttacking(IUnit* _target)
{
	target_->Damage(stat_.AttackPower, this);

	GameEngineSoundManager::GetInstance()->PlaySoundByName("attackGlove_Normal_Hit_P.wav");
	PacketSoundPlay packet;
	packet.SetSound("attackGlove_Normal_Hit_P.wav", transform_.GetWorldPosition());
	FT::SendPacket(packet);



	basicAttackEffectRenderer_->On();
	basicAttackEffectRenderer_->SetChangeAnimation("FX_BI_Hit_061", true);
	basicAttackEffectRenderer_->AnimationPlay();



	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("BasicAttack");
	FT::SendPacket(pack);

}

void Jackie::onUpdateBasicAttacking(IUnit* _target, float _deltaTime)
{
	if (true == basicAttackEffectRenderer_->IsCurAnimationEnd())
	{
		basicAttackEffectRenderer_->Off();
	}
}

void Jackie::onStartQSkill()
{
	timer_collision_Q = 0.0f;
	timer_end_Q = 0.0f;
	b_Qhit_ = false;

	curAnimationName_ = "SkillQ";
	renderer_->ChangeFBXAnimation("SkillQ", true);
}

void Jackie::onUpdateQSkill(float _deltaTime)
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

	if (0.1f <= timer_collision_Q && false == b_Qhit_)
	{
		collision_Q->On();

		//GameEngineSoundManager::GetInstance()->PlaySoundByName("hyunwoo_Skill01_Hit.wav");
		//PacketSoundPlay packet;
		//packet.SetSound("hyunwoo_Skill01_Hit.wav", transform_.GetWorldPosition());

		//FT::SendPacket(packet);

		//float4 wp = GetTransform()->GetWorldPosition();
		//qEffect_->GetTransform()->SetLocalPosition(wp);
		//qEffect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());
		//qEffect_->PlayAwake();

		//CharEffectPacket pack;
		//pack.SetTargetIndex(myIndex_);
		//pack.SetAnimationName("SkillQ");
		//FT::SendPacket(pack);

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
					character->Damage(300.0f, this);
				}
			}
		}

		b_Qhit_ = true;
	}
}

void Jackie::onStartWSkill()
{

}

void Jackie::onUpdateWSkill(float _deltaTime)
{
	changeAnimationWait();
	mainState_.ChangeState("NormalState", true);
	normalState_.ChangeState("Watch", true);
}

void Jackie::onStartESkill()
{
}

void Jackie::onUpdateESkill(float _deltaTime)
{
}

void Jackie::onStartRSkill()
{
}

void Jackie::onUpdateRSkill(float _deltaTime)
{
}

void Jackie::onStartDSkill()
{
}

void Jackie::onUpdateDSkill(float _deltaTime)
{
}

void Jackie::onStartDeath()
{
	changeDeathAnimation();
}

void Jackie::onUpdateDeath(float _deltaTime)
{
}

void Jackie::onUpdateCustomState(float _deltaTime)
{
}

void Jackie::onPlayEffect(const std::string& _effectName)
{
}

