#include "PreCompile.h"
#include "Yuki.h"
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


Yuki::Yuki() // default constructer 디폴트 생성자
	: b_isQ_(false), timer_Q(0.0f)
{

}

Yuki::~Yuki() // default destructer 디폴트 소멸자
{

}

Yuki::Yuki(Yuki&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void Yuki::LoadResource()
{
	GameEngineDirectory dir;

	dir.MoveParent("FoxTeam");
	dir / "Resources" / "FBX" / "Character" / "Yuki";

	GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Yuki_run.fbx"));
	mesh->CreateRenderingBuffer();

	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Yuki_run.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Yuki_wait.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Yuki_death.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Yuki_atk0.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Yuki_atk1.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Yuki_skillq.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Yuki_skille_attack.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Yuki_skille_move.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Yuki_skillr_end.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Yuki_skillr_loop.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Yuki_skillw_upper_wait.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Yuki_weaponskill_end.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Yuki_weaponskill_start.fbx"));

	std::vector<GameEngineFile> allFile = dir.GetAllFile("UserAnimation");
	for (GameEngineFile& file : allFile)
	{
		GameEngineFBXAnimationManager::GetInst().LoadUser(file.GetFullPath());
	}

	{
		GameEngineDirectory dir;
		dir.MoveParent("FoxTeam");
		dir / "Resources" / "Sound" / "Char" / "Yuki";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("wav");
		for (GameEngineFile& file : allFile)
		{
			GameEngineSoundManager::GetInstance()->CreateSound(file.FileName(), file.GetFullPath());
		}
	}

	{
		GameEngineDirectory dir;

		dir.MoveParent("FoxTeam");
		dir / "Resources" / "Texture" / "Yuki";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("png");
		for (GameEngineFile& file : allFile)
		{
			GameEngineTextureManager::GetInst().Load(file.FileName(), file.GetFullPath());
		}
	}

	GameEngineTexture* hitBase = GameEngineTextureManager::GetInst().Find("Fx_SQ_Cut01.png");
	hitBase->Cut(1, 6);

	GameEngineTexture* hitBase1 = GameEngineTextureManager::GetInst().Find("FX_BI_Yuki_01SE.png");
	hitBase1->Cut(6, 6);

	GameEngineTexture* hitBase2 = GameEngineTextureManager::GetInst().Find("FX_BI_Yuki_01.png");
	hitBase2->Cut(3, 12);
}

void Yuki::ReleaseResource()
{
	{
		GameEngineDirectory dir;
		dir.MoveParent("FoxTeam");
		dir / "Resources" / "Sound" / "Char" / "Yuki";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("wav");
		for (GameEngineFile& file : allFile)
		{
			GameEngineSoundManager::GetInstance()->ReleaseSound(file.FileName());
		}
	}

	GameEngineFBXMeshManager::GetInst().Delete("Yuki_run.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Yuki_run.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Yuki_wait.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Yuki_death.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Yuki_atk0.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Yuki_atk1.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Yuki_skillq.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Yuki_skille_attack.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Yuki_skille_move.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Yuki_skillr_end.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Yuki_skillr_loop.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Yuki_skillw_upper_wait.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Yuki_weaponskill_end.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Yuki_weaponskill_start.fbx");

	{
		GameEngineDirectory dir;
		dir.MoveParent("FoxTeam");
		dir / "Resources" / "FBX" / "Character" / "Yuki";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("UserAnimation");
		for (GameEngineFile& file : allFile)
		{
			GameEngineFBXAnimationManager::GetInst().Delete(file.GetFileName());
		}
	}
}

void Yuki::Start()
{
	Character::Start();
	initYukiCollision();
	initYukiCustomState();
	initEffectRenderer();

	stat_.HPMax = 835.0f; 
	stat_.HP = 835.0f;
	stat_.SPMax = 410.0f;
	stat_.SP = 410.0f;
	stat_.Defence = 27.0f;
	stat_.AttackPower = 38.0f;
	stat_.HPRegeneration = 1.575f;
	stat_.SPRegeneration = 2.2f;

	stat_.AttackSpeed = 0.89f;
	stat_.AttackStartTime = 0.15f;
	stat_.AttackEndTime = 0.4f;
	stat_.MovementSpeed = 355.0f;
	stat_.AttackRange = 280.f;
}

void Yuki::Update(float _deltaTime)
{
	Character::Update(_deltaTime);

	if (true == b_isQ_)
	{
		timer_Q -= _deltaTime;

		if (0.0f >= timer_Q)
		{
			timer_Q = 2.0f;
			b_isQ_ = false;
			// 이펙트 해제
		}
	}
}

void Yuki::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh("Yuki_run.fbx", "TextureDeferredLightAni");

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	renderer_->CreateFBXAnimation("Run", "Yuki_run.UserAnimation", 0);
	renderer_->CreateFBXAnimation("Wait", "Yuki_wait.UserAnimation", 0);
	renderer_->CreateFBXAnimation("Death", "Yuki_death.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("Atk0", "Yuki_atk0.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillD_start", "Yuki_weaponskill_start.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillD_end", "Yuki_weaponskill_end.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("Atk1", "Yuki_atk1.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillQ", "Yuki_skillq.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillE_attack", "Yuki_skille_attack.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillE_move", "Yuki_skille_move.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillR_end", "Yuki_skillr_end.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillR_loop", "Yuki_skillr_loop.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillW", "Yuki_skillw_upper_wait.UserAnimation", 0, false);

	renderer_->ChangeFBXAnimation("Wait");
}

void Yuki::initYukiCollision()
{
	collision_E = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_E->GetTransform()->SetLocalPosition({ 0.f,0.f,0.f });
	collision_E->GetTransform()->SetLocalScaling({ 150.f });
	collision_E->SetCollisionGroup(eCollisionGroup::PlayerAttack);
	collision_E->SetCollisionType(CollisionType::OBBBox3D);
	collision_E->Off();
}

void Yuki::initYukiCustomState()
{
}

void Yuki::initEffectRenderer()
{
	basicAttackEffectRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	basicAttackEffectRenderer_->SetImage("Fx_SQ_Cut01.png");
	basicAttackEffectRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 100.0f,  stat_.AttackRange - 180.f });
	basicAttackEffectRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	basicAttackEffectRenderer_->GetTransform()->SetLocalScaling(basicAttackEffectRenderer_->GetCurrentTexture()->GetTextureSize() / 3);
	basicAttackEffectRenderer_->CreateAnimation("Fx_SQ_Cut01.png", "Fx_SQ_Cut01", 0, 5, 0.03f, false);
	basicAttackEffectRenderer_->Off();

	groundEffectRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	groundEffectRenderer_->SetImage("FX_BI_Yuki_01SE.png");
	groundEffectRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 10.0f,  0.0f });
	groundEffectRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	groundEffectRenderer_->GetTransform()->SetLocalScaling(groundEffectRenderer_->GetCurrentTexture()->GetTextureSize());
	groundEffectRenderer_->CreateAnimation("FX_BI_Yuki_01SE.png", "FX_BI_Yuki_01SE", 0, 35, 0.02f, false);
	groundEffectRenderer_->Off();

	rearEffectRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	rearEffectRenderer_->SetImage("FX_BI_Yuki_01.png", "PointSmp");
	rearEffectRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 100.0f, -180.0f });
	rearEffectRenderer_->GetTransform()->SetLocalRotationDegree({ 0.f,90.f,0.f });
	rearEffectRenderer_->GetTransform()->SetLocalScaling(rearEffectRenderer_->GetCurrentTexture()->GetTextureSize() / 3);
	rearEffectRenderer_->CreateAnimation("FX_BI_Yuki_01.png", "FX_BI_Yuki_01", 0, 35, 0.01f, false);
	rearEffectRenderer_->Off();
}

void Yuki::changeAnimationRun()
{
	curAnimationName_ = "Run";
	renderer_->ChangeFBXAnimation("Run");
}

void Yuki::changeAnimationWait()
{
	curAnimationName_ = "Wait";
	renderer_->ChangeFBXAnimation("Wait");
}

void Yuki::changeAnimationBasicAttack()
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

	GameEngineSoundManager::GetInstance()->PlaySoundByName("attackGlove_Normal01.wav");
	PacketSoundPlay packet;
	packet.SetSound("attackGlove_Normal01.wav", transform_.GetWorldPosition());
	FT::SendPacket(packet);
}

void Yuki::changeDeathAnimation()
{
	ChangeAnimation("Death", true);
}

void Yuki::onStartBasicAttacking(IUnit* _target)
{
	if (b_isQ_)
	{
		target_->Damage(stat_.AttackPower * 1.6f + 20.0f, this);

		GameEngineSoundManager::GetInstance()->PlaySoundByName("Yuki_Skill01_Attack.wav");
		PacketSoundPlay packet0;
		packet0.SetSound("Yuki_Skill01_Attack.wav", transform_.GetWorldPosition());
		FT::SendPacket(packet0);

		GameEngineSoundManager::GetInstance()->PlaySoundByName("Yuki_Passive_Hit_r2.wav");
		PacketSoundPlay packet;
		packet.SetSound("Yuki_Passive_Hit_r2.wav", transform_.GetWorldPosition());
		FT::SendPacket(packet);

		b_isQ_ = false;

		return;
	}


	target_->Damage(stat_.AttackPower, this);

	GameEngineSoundManager::GetInstance()->PlaySoundByName("attackGlove_Normal_Hit_P.wav");
	PacketSoundPlay packet;
	packet.SetSound("attackGlove_Normal_Hit_P.wav", transform_.GetWorldPosition());
	FT::SendPacket(packet);


	float4 wp = target_->GetTransform()->GetWorldPosition();

	basicAttackEffectRenderer_->On();
	basicAttackEffectRenderer_->SetChangeAnimation("Fx_SQ_Cut01", true);
	//basicAttackEffectRenderer_->GetTransform()->SetWorldPosition({ wp.x,wp.y + 40.0f, wp.z });
	basicAttackEffectRenderer_->AnimationPlay();


	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("BasicAttack");
	FT::SendPacket(pack);
	// 여기 이펙트 패킷 하나
}

void Yuki::onUpdateBasicAttacking(IUnit* _target, float _deltaTime)
{
	if (true == basicAttackEffectRenderer_->IsCurAnimationEnd())
	{
		basicAttackEffectRenderer_->Off();
	}
}

void Yuki::onStartQSkill()
{

	// 검에 이팩트 재생

	b_isQ_ = true;
	timer_Q = 2.0f;
	
	GameEngineSoundManager::GetInstance()->PlaySoundByName("Yuki_Skill01_Active.wav");
	PacketSoundPlay packet;
	packet.SetSound("Yuki_Skill01_Active.wav", transform_.GetWorldPosition());
	FT::SendPacket(packet);
}

void Yuki::onUpdateQSkill(float _deltaTime)
{
	changeAnimationWait();
	mainState_.ChangeState("NormalState", true);
	normalState_.ChangeState("Watch", true);
}

void Yuki::onStartWSkill()
{
	// 바닥에 벚꽃 이펙트 재생

	if ("Wait" == curAnimationName_)
	{
		curAnimationName_ = "SkillW";
		renderer_->ChangeFBXAnimation("SkillW", true);
	}

	GameEngineSoundManager::GetInstance()->PlaySoundByName("Yuki_Skill02_Active.wav");
	PacketSoundPlay packet;
	packet.SetSound("Yuki_Skill02_Active.wav", transform_.GetWorldPosition());
	FT::SendPacket(packet);

	groundEffectRenderer_->On();
	groundEffectRenderer_->SetChangeAnimation("FX_BI_Yuki_01SE", true);
	groundEffectRenderer_->AnimationPlay();
}

void Yuki::onUpdateWSkill(float _deltaTime)
{
	changeAnimationWait();
	mainState_.ChangeState("NormalState", true);
	normalState_.ChangeState("Watch", true);
}

void Yuki::onStartESkill()
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

	curAnimationName_ = "SkillE_move";
	renderer_->ChangeFBXAnimation("SkillE_move", true);

	GameEngineSoundManager::GetInstance()->PlaySoundByName("Yuki_Skill03_Move.wav");
	PacketSoundPlay packet;
	packet.SetSound("Yuki_Skill03_Move.wav", transform_.GetWorldPosition());
	FT::SendPacket(packet);

	rearEffectRenderer_->On();
	rearEffectRenderer_->SetChangeAnimation("FX_BI_Yuki_01", true);
	rearEffectRenderer_->AnimationPlay();

	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("SkillE");
	FT::SendPacket(pack);
}

void Yuki::onUpdateESkill(float _deltaTime)
{
	if (true == collision_E->IsUpdate())
	{
		GetLevel()->PushDebugRender(collision_E->GetTransform(), CollisionType::OBBBox3D, float4::RED);
	}

	if (true == renderer_->IsCurrentAnimationEnd())
	{
		//changeAnimationWait();
		//mainState_.ChangeState("NormalState", true);
		//normalState_.ChangeState("Watch", true);
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
						character->Damage(stat_.AttackPower * 0.65f + 60.0f, this);

						GameEngineSoundManager::GetInstance()->PlaySoundByName("Yuki_Skill01_Attack.wav");
						PacketSoundPlay packet0;
						packet0.SetSound("Yuki_Skill01_Attack.wav", transform_.GetWorldPosition());
						FT::SendPacket(packet0);

						GameEngineSoundManager::GetInstance()->PlaySoundByName("Yuki_Passive_Hit_r2.wav");
						PacketSoundPlay packet;
						packet.SetSound("Yuki_Passive_Hit_r2.wav", transform_.GetWorldPosition());
						FT::SendPacket(packet);

						b_Ehit_ = true;
					}
				}
			}
			curAnimationName_ = "SkillE_attack";
			renderer_->ChangeFBXAnimation("SkillE_attack", true);

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

	if (true == rearEffectRenderer_->IsCurAnimationEnd())
	{
		rearEffectRenderer_->Off();
	}
}

void Yuki::onStartRSkill()
{

}

void Yuki::onUpdateRSkill(float _deltaTime)
{

}

void Yuki::onStartDSkill()
{
}

void Yuki::onUpdateDSkill(float _deltaTime)
{
}

void Yuki::onStartDeath()
{
}

void Yuki::onUpdateDeath(float _deltaTime)
{
}

void Yuki::onUpdateCustomState(float _deltaTime)
{
}

void Yuki::onPlayEffect(const std::string& _effectName)
{
}

