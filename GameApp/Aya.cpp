#include "PreCompile.h"
#include "Aya.h"
#include "AyaBullet.h"

#include "LumiaMap.h"
#include "PacketCreateProjectile.h"
#include "RioArrow.h"

Aya::Aya()
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
	stat_.AttackSpeed = 1.1f;
}

void Aya::Update(float _deltaTime)
{
	Character::Update(_deltaTime);
}

JobType Aya::GetJobType()
{
    return JobType::AYA;
}

void Aya::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh("Aya_Idle.fbx", "TextureDeferredLightAni", true);

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	std::string ext = "UserAnimation";

	renderer_->CreateFBXAnimation("Idle", "Aya_Idle." +  ext, 0, true);
	renderer_->CreateFBXAnimation("Run", "Aya_Run." + ext, 0, true);
	renderer_->CreateFBXAnimation("Attack", "Aya_Attack." +  ext, 0, false);
	renderer_->CreateFBXAnimation("Reload", "Aya_Reload." +  ext, 0, false);
	renderer_->CreateFBXAnimation("SkillD", "Aya_SkillD." +  ext, 0, false);
	renderer_->CreateFBXAnimation("SkillE", "Aya_SkillE." +  ext, 0, false);
	renderer_->CreateFBXAnimation("SkillQ", "Aya_SkillQ." +  ext, 0, false, 0.03f);
	renderer_->CreateFBXAnimation("SkillR_End", "Aya_SkillR_End." +  ext, 0, false);
	renderer_->CreateFBXAnimation("SkillR_Start", "Aya_SkillR_Start." +  ext, 0, false);
	renderer_->CreateFBXAnimation("SkillW_Back", "Aya_SkillW_Back." +  ext, 0, false);
	renderer_->CreateFBXAnimation("SkillW_Forward", "Aya_SkillW_Forward." +  ext, 0, false);
	renderer_->CreateFBXAnimation("SkillW_Left", "Aya_SkillW_Left." +  ext, 0, false);
	renderer_->CreateFBXAnimation("SkillW_Right", "Aya_SkillW_Right." +  ext, 0, false);
	renderer_->CreateFBXAnimation("SkillW_Shot", "Aya_SkillW_Shot." +  ext, 0, false);
	renderer_->CreateFBXAnimation("SkillW_Wait", "Aya_SkillW_Wait." + ext, 0, false);
	renderer_->CreateFBXAnimation("Death", "Aya_Death." + ext, 0, false);

	renderer_->ChangeFBXAnimation("Idle");

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
	target_ =  getMousePickedCharacter();

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
		AyaBullet* arrow = level_->CreateActor<AyaBullet>();
		arrow->MakeTarget(*this, stat_.AttackPower / 1.5f, startPosition, arrowSpeed, *target_);

		arrow = level_->CreateActor<AyaBullet>();
		arrow->MakeTarget(*this, stat_.AttackPower / 1.0f, startPosition, arrowSpeed, *target_);
		arrow->SetWaitTime(doubleStrikeDelay);
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
	
}

void Aya::onUpdateWSkill(float _deltaTime)
{
	
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
}

void Aya::onUpdateRSkill(float _d1eltaTime)
{
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

void Aya::onPlayEffect(const std::string& _effectName)
{
}
