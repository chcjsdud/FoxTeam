#include "PreCompile.h"


#include <GameEngine/GameEngineCollision.h>

#include "Aya.h"
#include "AyaBullet.h"
#include "LumiaMap.h"
#include "PacketCreateProjectile.h"
#include "RioArrow.h"
#include "MousePointer.h"
#include "Monsters.h"
#include "CharCrowdControlPacket.h"

Aya::Aya()
	: ammo_(6)
	, skillWFireCount_(0)
	, skillWFireDelay_(0.3f)
	, skillRCol_(nullptr)
	, pistolRenderer_(nullptr)
{

}

Aya::~Aya()
{

}

void Aya::LoadResource()
{
	{
		GameEngineDirectory dir;

		dir.MoveParent("FoxTeam");
		dir / "Resources" / "FBX" / "Character" / "Aya";

		GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Aya_Idle.fbx"));
		mesh->CreateRenderingBuffer();

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Weapon_Pistol_01.fbx"));
		mesh->CreateRenderingBuffer();
		GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Weapon_Pistol_01.fbx"));

		std::vector<GameEngineFile> allFile = dir.GetAllFile("UserAnimation");
		for (GameEngineFile& file : allFile)
		{
			GameEngineFBXAnimationManager::GetInst().LoadUser(file.GetFullPath());
		}
	}

	{
		GameEngineDirectory dir;
		dir.MoveParent("FoxTeam");
		dir / "Resources" / "Sound" / "Char" / "Aya";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("wav");
		for (GameEngineFile& file : allFile)
		{
			GameEngineSoundManager::GetInstance()->CreateSound(file.FileName(), file.GetFullPath());
		}
	}
}

void Aya::ReleaseResource()
{
	{
		GameEngineDirectory dir;
		dir.MoveParent("FoxTeam");
		dir / "Resources" / "Sound" / "Char" / "Aya";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("wav");
		for (GameEngineFile& file : allFile)
		{
			GameEngineSoundManager::GetInstance()->ReleaseSound(file.FileName());
		}
	}

	GameEngineDirectory dir;
	dir.MoveParent("FoxTeam");
	dir / "Resources" / "FBX" / "Character" / "Aya";

	std::vector<GameEngineFile> allFile = dir.GetAllFile("UserAnimation");
	for (GameEngineFile& file : allFile)
	{
		GameEngineFBXAnimationManager::GetInst().Delete(file.GetFileName());
	}

	GameEngineFBXMeshManager::GetInst().Delete("Aya_Idle.fbx");
}

void Aya::Start()
{
	Character::Start();

	skillRCol_ = CreateTransformComponent<GameEngineCollision>();
	skillRCol_->SetCollisionType(CollisionType::Sphere3D);
	skillRCol_->SetCollisionGroup(eCollisionGroup::PlayerAttack);
	skillRCol_->GetTransform()->SetLocalScaling(700.f);

	stat_.AttackSpeed = 1.1f;
}

void Aya::Update(float _deltaTime)
{
	Character::Update(_deltaTime);

	level_->PushDebugRender(skillRCol_->GetTransform(), CollisionType::CirCle);

	if (GameEngineInput::Down("Z"))
	{
		coolTimer_Q_ = 0.5f;
		coolTimer_W_ = 0.5f;
		coolTimer_E_ = 0.5f;
		coolTimer_R_ = 0.5f;
		coolTimer_D_ = 0.5f;
	}
}

JobType Aya::GetJobType()
{
	return JobType::AYA;
}

void Aya::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>(static_cast<int>(ObjectRenderOrder::CHARACTER));
	renderer_->SetFBXMesh("Aya_Idle.fbx", "TextureDeferredLightAni", true);

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	std::string ext = "UserAnimation";

	renderer_->CreateFBXAnimation("Idle", "Aya_Idle." + ext, 0, true);
	renderer_->CreateFBXAnimation("Run", "Aya_Run." + ext, 0, true);
	renderer_->CreateFBXAnimation("Attack", "Aya_Attack." + ext, 0, false);
	renderer_->CreateFBXAnimation("Reload", "Aya_Reload." + ext, 0, false);
	renderer_->CreateFBXAnimation("SkillD", "Aya_SkillD." + ext, 0, false);
	renderer_->CreateFBXAnimation("SkillE", "Aya_SkillE." + ext, 0, false);
	renderer_->CreateFBXAnimation("SkillQ", "Aya_SkillQ." + ext, 0, false, 0.03f);
	renderer_->CreateFBXAnimation("SkillR_End", "Aya_SkillR_End." + ext, 0, false);
	renderer_->CreateFBXAnimation("SkillR_Start", "Aya_SkillR_Start." + ext, 0, false);
	renderer_->CreateFBXAnimation("SkillW_Back", "Aya_SkillW_Back." + ext, 0);
	renderer_->CreateFBXAnimation("SkillW_Forward", "Aya_SkillW_Forward." + ext, 0);
	renderer_->CreateFBXAnimation("SkillW_Left", "Aya_SkillW_Left." + ext, 0);
	renderer_->CreateFBXAnimation("SkillW_Right", "Aya_SkillW_Right." + ext, 0);
	renderer_->CreateFBXAnimation("SkillW_Shot", "Aya_SkillW_Shot." + ext, 0, false);
	renderer_->CreateFBXAnimation("SkillW_Wait", "Aya_SkillW_Wait." + ext, 0);
	renderer_->CreateFBXAnimation("Death", "Aya_Death." + ext, 0, false);

	renderer_->ChangeFBXAnimation("Idle");

	renderer_->GetRenderSet(1).isRender = false;

	pistolRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>(static_cast<int>(ObjectRenderOrder::WEAPON));
	pistolRenderer_->SetFBXMesh("Weapon_Pistol_01.fbx", "TextureDeferredLightAni");
	pistolRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	pistolRenderer_->GetTransform()->SetLocalScaling(100.f);
	pistolRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f, 0.0f });
	pistolRenderer_->SetParentBoneName(renderer_, "Bip001 R Finger1");
	pistolRenderer_->SetCustomOffset({ 2.7f, 0.f, -2.5f });
	pistolRenderer_->CreateFBXAnimation("Idle", "Weapon_Pistol_01.fbx");
	pistolRenderer_->ChangeFBXAnimation("Idle");

	MainOutLineRenderer_ = CreateTransformComponent<GameEnginePreprocessingRenderer>();
	MainOutLineRenderer_->SetBaseRenderer(renderer_, "ObjectOutLineAni", true);
	MainOutLineRenderer_->GetRenderSet(1).isRender = false;

	WeaponOutLineRenderer1_ = CreateTransformComponent<GameEnginePreprocessingRenderer>();
	WeaponOutLineRenderer1_->SetBaseRenderer(pistolRenderer_, "ObjectOutLineAni", true);
}

void Aya::changeAnimationRun()
{
	ChangeAnimation("Run");
}

void Aya::changeAnimationWait()
{
	ChangeAnimation("Idle");
}

void Aya::changeAnimationBasicAttack()
{
	ChangeAnimation("Attack", true);
}

void Aya::changeDeathAnimation()
{
	ChangeAnimation("Death");
}

void Aya::onStartBasicAttacking(IUnit* _target)
{
	float4 offset = { 20.f, 120.f, 30.f, 0.f };
	offset = offset * transform_.GetTransformData().WorldWorld_;
	float4 startPosition = transform_.GetWorldPosition();
	startPosition += offset;

	float arrowSpeed = 1500.f;
	{
		PacketCreateProjectile packetBullet;
		packetBullet.MakeTargetProjectile(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *target_);
		packetBullet.SetType(eProjectileType::AyaTargetBullet);
		FT::SendPacket(packetBullet);
	}

	if (GameServer::GetInstance()->IsOpened())
	{
		AyaBullet* arrow = level_->CreateActor<AyaBullet>();
		arrow->MakeTarget(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *target_);

		arrow = level_->CreateActor<AyaBullet>();
		arrow->MakeTarget(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *target_);
	}

	FT::PlaySoundAndSendPacket("Gun_Fire.wav", transform_.GetWorldPosition());
}

void Aya::onUpdateBasicAttacking(IUnit* _target, float _deltaTime)
{
	if (renderer_->IsCurrentAnimationEnd())
	{
		changeAnimationWait();
	}
}

void Aya::onStartQSkill()
{
	target_ = getMousePickedCharacter();

	if (target_ == nullptr)
	{
		changeAnimationWait();
		mainState_ << "NormalState";
		return;
	}

	float4 targetPosition = target_->GetTransform()->GetWorldPosition();
	float4 playerPosition = transform_.GetWorldPosition();

	float dist = float4::Calc_Len3D(targetPosition, playerPosition);
	if (dist > stat_.AttackRange + 100.f)
	{
		changeAnimationWait();
		mainState_ << "NormalState";
		return;
	}

	ChangeAnimation("SkillQ");
	setRotationTo(targetPosition, playerPosition);

	float doubleStrikeDelay = 0.4f;

	float4 offset = { 20.f, 120.f, 30.f, 0.f };
	offset = offset * transform_.GetTransformData().WorldWorld_;
	float4 startPosition = transform_.GetWorldPosition();
	startPosition += offset;

	float arrowSpeed = 1500.f;

	{
		PacketCreateProjectile packetBullet;
		packetBullet.MakeTargetProjectile(*this, stat_.AttackPower / 1.5f, startPosition, arrowSpeed, *target_);
		packetBullet.SetType(eProjectileType::AyaTargetBullet);
		FT::SendPacket(packetBullet);
	}
	{
		PacketCreateProjectile packetArrow;
		packetArrow.MakeTargetProjectile(*this, stat_.AttackPower / 1.0f, startPosition, arrowSpeed, *target_);
		packetArrow.SetWaitTime(doubleStrikeDelay);
		packetArrow.SetType(eProjectileType::AyaTargetBullet);
		FT::SendPacket(packetArrow);
	}

	if (GameServer::GetInstance()->IsOpened())
	{
		AyaBullet* bullet = level_->CreateActor<AyaBullet>();
		bullet->MakeTarget(*this, stat_.AttackPower / 1.5f, startPosition, arrowSpeed, *target_);

		bullet = level_->CreateActor<AyaBullet>();
		bullet->MakeTarget(*this, stat_.AttackPower / 1.0f, startPosition, arrowSpeed, *target_);
		bullet->SetWaitTime(doubleStrikeDelay);
	}

	FT::PlaySoundAndSendPacket("aya_Skill01_Attack_v1.wav", transform_.GetWorldPosition());
}

void Aya::onUpdateQSkill(float _deltaTime)
{
	if (attackState_.GetTime() > 0.3f)
	{
		FT::PlaySoundAndSendPacket("aya_Skill01_Attack_v1.wav", transform_.GetWorldPosition());
		attackState_.GetCurrentState()->Time_ = -10.f;
	}

	if (renderer_->IsCurrentAnimationEnd())
	{
		changeAnimationWait();
		mainState_ << "NormalState";
	}
}

void Aya::onStartWSkill()
{
	setRotationToMouse();

	ChangeAnimation("SkillW_Wait");
	renderer_->OverrideFBXAnimation("SkillW_Shot", "Bip001 Spine2");


	float4 offset = { 20.f, 120.f, 30.f, 0.f };
	offset = offset * transform_.GetTransformData().WorldWorld_;
	float4 startPosition = transform_.GetWorldPosition();
	startPosition += offset;

	float bulletSpeed = 1200.f;


	PacketCreateProjectile packetBullet;
	packetBullet.MakeNonTargetProjectile(*this, stat_.AttackPower / 1.5f, startPosition, transform_.GetWorldRotation().y, bulletSpeed);
	packetBullet.SetScale({ 80.f, 150.f, 100 });
	packetBullet.SetType(eProjectileType::AyaTargetBullet);
	FT::SendPacket(packetBullet);


	if (GameServer::GetInstance()->IsOpened())
	{
		AyaBullet* bullet = level_->CreateActor<AyaBullet>();
		bullet->MakeNonTarget(*this, stat_.AttackPower / 1.5f, startPosition, transform_.GetWorldRotation().y, bulletSpeed);
		bullet->SetScale({ 80.f, 150.f, 100 });
	}

	FT::PlaySoundAndSendPacket("aya_Skill02_Shot.wav", transform_.GetWorldPosition());
	skillWFireCount_ = 1;
}

void Aya::onUpdateWSkill(float _deltaTime)
{
	float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);
	static float cos = 0.0f;
	static float4 cross;
	if (GameEngineInput::Press("LButton") || GameEngineInput::Down("LButton"))
	{
		Move(mousePosition);
		cos = float4::Dot3DToCos(mousePosition - transform_.GetWorldPosition(), transform_.GetWorldForwardVector());
		cross = float4::Cross3D(mousePosition - transform_.GetWorldPosition(), transform_.GetWorldForwardVector());
	}

	if (cos > 0.8f)
	{
		ChangeAnimation("SkillW_Forward");
	}
	else if (cos < -0.8f)
	{
		ChangeAnimation("SkillW_Back");
	}
	else
	{	// 양수 왼쪽, 음수 오른쪽
		if (cross.y > 0.f)
		{
			ChangeAnimation("SkillW_Left");
		}
		else
		{
			ChangeAnimation("SkillW_Right");
		}
	}


	float4 worldPosition = transform_.GetWorldPosition();
	worldPosition.y = destination_.y;
	if ((destination_ - worldPosition).Len3D() > FT::Char::MOVE_FINISH_CHECK_DISTANCE)
	{
		moveTickLockDirection(_deltaTime, worldPosition);
	}
	else
	{
		if (!destinations_.empty())
		{
			destination_ = destinations_.back();
			destinations_.pop_back();

			// 여기서 한번 더 해주지 않으면 움직임이 1 프레임 손실된다.
			if ((destination_ - worldPosition).Len3D() > FT::Char::MOVE_FINISH_CHECK_DISTANCE)
			{
				moveTickLockDirection(_deltaTime, worldPosition);
			}
		}
		else
		{
			ChangeAnimation("SkillW_Wait");
		}
	}

	if (renderer_->IsOverrideAnimationEnd())
	{
		float4 offset = { 20.f, 120.f, 30.f, 0.f };
		offset = offset * transform_.GetTransformData().WorldWorld_;
		float4 startPosition = transform_.GetWorldPosition();
		startPosition += offset;

		float bulletSpeed = 1200.f;


		PacketCreateProjectile packetBullet;
		packetBullet.MakeNonTargetProjectile(*this, stat_.AttackPower / 1.5f, startPosition, transform_.GetWorldRotation().y, bulletSpeed);
		packetBullet.SetScale({ 80.f, 150.f, 100 });
		packetBullet.SetType(eProjectileType::AyaTargetBullet);
		FT::SendPacket(packetBullet);


		if (GameServer::GetInstance()->IsOpened())
		{
			AyaBullet* bullet = level_->CreateActor<AyaBullet>();
			bullet->MakeNonTarget(*this, stat_.AttackPower / 1.5f, startPosition, transform_.GetWorldRotation().y, bulletSpeed);
			bullet->SetScale({ 80.f, 150.f, 100 });
		}

		FT::PlaySoundAndSendPacket("aya_Skill02_Shot.wav", transform_.GetWorldPosition());

		ChangeOverrideAnimation("SkillW_Shot", "Bip001 Spine2", true);
		++skillWFireCount_;
	}

	if (skillWFireCount_ >= 10)
	{
		renderer_->ClearOverrideAnimation();
		mainState_ << "NormalState";

	}

	if (GameEngineInput::Down("Z"))
	{
		renderer_->ClearOverrideAnimation();
		mainState_ << "NormalState";
	}
}

void Aya::onStartESkill()
{
	setRotationToMouse();

	ChangeAnimation("SkillE", true);

	FT::PlaySoundAndSendPacket("aya_Skill03_Activation.wav", transform_.GetWorldPosition());
}

void Aya::onUpdateESkill(float _deltaTime)
{
	if (attackState_.GetCurrentState()->Time_ > 0.5f)
	{
		destinations_.clear();
		destination_ = transform_.GetWorldPosition();
		changeAnimationWait();
		mainState_ << "NormalState";
	}

	float4 moveSpeed = (direction_ * stat_.MovementSpeed * 2.0f) * _deltaTime;
	float4 nextMovePosition = transform_.GetWorldPosition() + moveSpeed;

	float temp;
	if (true == currentMap_->GetNavMesh()->CheckIntersects(nextMovePosition + float4{ 0.0f, FT::Map::MAX_HEIGHT, 0.0f }, float4::DOWN, temp))
	{
		GetTransform()->SetWorldPosition(nextMovePosition);
	}
}

void Aya::onStartRSkill()
{
	ChangeAnimation("SkillR_Start");
	FT::PlaySoundAndSendPacket("aya_Skill04_Ready.wav", transform_.GetWorldPosition());
}

void Aya::onUpdateRSkill(float _d1eltaTime)
{
	if (attackState_.GetTime() < 0.0f)
	{
		if (renderer_->IsCurrentAnimationEnd())
		{
			normalState_ << "Watch";
			mainState_ << "NormalState";
		}
		return;
	}

	if (attackState_.GetTime() > 1.5f)
	{
		std::list<GameEngineCollision*> list = skillRCol_->GetCollisionList(eCollisionGroup::Player);
		for (GameEngineCollision* c : list)
		{
			Character* unit = dynamic_cast<Character*>(c->GetActor());
			if (nullptr != unit)
			{
				if (unit == this)
				{
					continue;
				}
				unit->Damage(300.f + stat_.AttackPower * 2.f, this);
				unit->Stun(2.0f);
			}
		}

		list.clear();
		list = skillRCol_->GetCollisionList(eCollisionGroup::Monster);
		for (GameEngineCollision* c : list)
		{
			Monsters* unit = dynamic_cast<Monsters*>(c->GetActor());
			if (nullptr != unit)
			{
				unit->Damage(300.f + stat_.AttackPower * 2.f, this);
				unit->Stun(2.0f);
			}
		}

		attackState_.GetCurrentState()->Time_ = -1000.f;
		ChangeAnimation("SkillR_End");
		FT::PlaySoundAndSendPacket("aya_Skill04_Activation.wav", transform_.GetWorldPosition());
	}

}

void Aya::onStartDSkill()
{
}

void Aya::onUpdateDSkill(float _deltaTime)
{
}

void Aya::onUpdateCustomState(float _deltaTime)
{
}

void Aya::onStartDeath()
{
}

void Aya::onUpdateDeath(float _deltaTime)
{
}

void Aya::onPlayEffect(const std::string& _effectName, IUnit* _victim)
{
}

void Aya::onEffectTransformCheck(float _deltaTime)
{
}
