#include "PreCompile.h"
#include "Rio.h"
#include "MousePointer.h"
#include "LumiaMap.h"

#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineLevelControlWindow.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameApp/LumiaLevel.h>
#include "RioArrow.h"
#include "PacketCreateProjectile.h"
#include "PacketSoundPlay.h"
#include "eProjectileType.h"
#include "RioDSkill.h"

Rio::Rio()
	: Character()
	, bLongBow_(false)
	, skillECollision_(nullptr)
	, bSkillEPassable_(false)
	, bShortRSkillExtra_(false)
	, shortBow_(nullptr)
	, longBow_(nullptr)
	, skillETime_(0.0f)
	, skillRTime_(0.0f)
{

}

Rio::~Rio()
{

}

void Rio::LoadResource()
{
	{
		GameEngineDirectory dir;

		dir.MoveParent("FoxTeam");
		dir / "Resources" / "FBX" / "Character" / "Rio";

		GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Rio_Short_Run.fbx"));
		mesh->CreateRenderingBuffer();

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Rio_Bow_Short_Idle.fbx"));
		mesh->CreateRenderingBuffer();

		GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Rio_Bow_Short_Idle.fbx"));

		std::vector<GameEngineFile> allFile = dir.GetAllFile("UserAnimation");
		for (GameEngineFile& file : allFile)
		{
			GameEngineFBXAnimationManager::GetInst().LoadUser(file.GetFullPath());
		}
	}

	{
		GameEngineDirectory dir;
		dir.MoveParent("FoxTeam");
		dir / "Resources" / "Sound" / "Char" / "Rio";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("wav");
		for (GameEngineFile& file : allFile)
		{
			GameEngineSoundManager::GetInstance()->CreateSound(file.FileName(), file.GetFullPath());
		}
	}

	{
		GameEngineDirectory dir;

		dir.MoveParent("FoxTeam");
		dir / "Resources" / "Texture" / "Rio";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("png");
		for (GameEngineFile& file : allFile)
		{
			GameEngineTextureManager::GetInst().Load(file.FileName(), file.GetFullPath());
		}
	}
}

void Rio::ReleaseResource()
{
	{
		GameEngineDirectory dir;
		dir.MoveParent("FoxTeam");
		dir / "Resources" / "Sound" / "Char" / "Rio";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("wav");
		for (GameEngineFile& file : allFile)
		{
			GameEngineSoundManager::GetInstance()->ReleaseSound(file.FileName());
		}
	}

	GameEngineDirectory dir;
	dir.MoveParent("FoxTeam");
	dir / "Resources" / "FBX" / "Character" / "Rio";

	std::vector<GameEngineFile> allFile = dir.GetAllFile("UserAnimation");
	for (GameEngineFile& file : allFile)
	{
		GameEngineFBXAnimationManager::GetInst().Delete(file.GetFileName());
	}

	GameEngineFBXMeshManager::GetInst().Delete("Rio_Bow_Short_Idle.fbx");
	GameEngineFBXMeshManager::GetInst().Delete("Rio_Short_Run.fbx");
}

void Rio::Start()
{
	Character::Start();

	skillECollision_ = CreateTransformComponent<GameEngineCollision>();
	skillECollision_->SetCollisionType(CollisionType::OBBBox3D);
	skillECollision_->GetTransform()->SetLocalScaling(500.f);

	customState_.CreateState(MakeState(Rio, SkillEBegin));
	customState_.CreateState(MakeState(Rio, SkillEShot));
	customState_.CreateState(MakeState(Rio, SkillEEnd));

	stat_.AttackPower = 300.0f;
}

void Rio::Update(float _deltaTime)
{
	Character::Update(_deltaTime);
}

void Rio::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh("Rio_Short_Run.fbx", "TextureDeferredLightAni");

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	renderer_->CreateFBXAnimation("Run_Short", "Rio_Short_Run.UserAnimation", 0);
	renderer_->CreateFBXAnimation("Run_Long", "Rio_Long_Run.UserAnimation", 0);

	renderer_->CreateFBXAnimation("Wait_Short", "Rio_Short_Wait.UserAnimation", 0);
	renderer_->CreateFBXAnimation("Wait_Long", "Rio_Long_Wait.UserAnimation", 0);

	renderer_->CreateFBXAnimation("BasicAttack_Short", "Rio_Short_Attack.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("BasicAttack_Long", "Rio_Long_Attack.UserAnimation", 0, false);

	renderer_->CreateFBXAnimation("SkillQ_Short", "Rio_Short_Skill_Q.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillQ_Long", "Rio_Long_Skill_Q.UserAnimation", 0, false);

	renderer_->CreateFBXAnimation("SkillW_Short", "Rio_Short_Skill_W.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillW_Long", "Rio_Long_Skill_W.UserAnimation", 0, false);

	renderer_->CreateFBXAnimation("SkillE_Short", "Rio_Short_Skill_E.UserAnimation", 0, false);

	renderer_->CreateFBXAnimation("SkillR_Short_Start", "Rio_Short_Skill_R_Start.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillR_Short_End", "Rio_Short_Skill_R_End.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillR_Long", "Rio_Long_Skill_R.UserAnimation", 0, false);

	renderer_->CreateFBXAnimation("SkillD_Short", "Rio_Short_Skill_D.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillD_Long", "Rio_Long_Skill_D.UserAnimation", 0, false);

	renderer_->CreateFBXAnimation("Rio_CraftFood", "Rio_CraftFood.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("Rio_CraftMetal", "Rio_CraftMetal.UserAnimation", 0, false);

	renderer_->CreateFBXAnimation("Death", "Rio_Death.UserAnimation", 0, false);

	renderer_->ChangeFBXAnimation("Wait_Short");

	renderer_->GetRenderSet(1).isRender = false;

	shortBow_ = CreateTransformComponent<GameEngineFBXRenderer>();
	shortBow_->SetFBXMesh("Rio_Bow_Short_Idle.fbx", "TextureDeferredLightAni");
	shortBow_->GetTransform()->SetLocalScaling(100.f);
	shortBow_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	//shortBow_->SetParentBoneIndex(renderer_, 52);
	shortBow_->SetParentBoneName(renderer_, "Bip001 L Finger1");
	shortBow_->CreateFBXAnimation("ShortBow_Idle", "Rio_Bow_Short_Idle.fbx");
	shortBow_->ChangeFBXAnimation("ShortBow_Idle");
}

void Rio::changeAnimationWait()
{
	if (bLongBow_)
	{
		ChangeAnimation("Wait_Long");
	}
	else
	{
		ChangeAnimation("Wait_Short");
	}
}

void Rio::changeAnimationRun()
{
	if (bLongBow_)
	{
		ChangeAnimation("Run_Long");
	}
	else
	{
		ChangeAnimation("Run_Short");
	}
}

void Rio::changeAnimationBasicAttack()
{
	if (bLongBow_)
	{
		ChangeAnimation("BasicAttack_Long", true);
	}
	else
	{
		ChangeAnimation("BasicAttack_Short", true);
	}
}

void Rio::changeAnimationCraft()
{
	ChangeAnimation("Rio_CraftMetal");
}

void Rio::changeAnimationCook()
{
	ChangeAnimation("Rio_CraftFood");
}

void Rio::changeDeathAnimation()
{
	ChangeAnimation("Death", true);
}

void Rio::onStartBasicAttacking(IUnit* _target)
{
	if (bLongBow_)
	{
		float4 offset = { 20.f, 140.f, 30.f, 0.f };
		offset = offset * transform_.GetTransformData().WorldWorld_;
		float4 startPosition = transform_.GetWorldPosition();
		startPosition += offset;

		RioArrow* arrow = level_->CreateActor<RioArrow>();
		arrow->MakeTargetArrow(*this, stat_.AttackPower, startPosition, 1000.f, *target_);
	}
	else
	{
		float doubleStrikeDelay = (stat_.AttackEndTime - stat_.AttackStartTime) / stat_.AttackSpeed / 2.0f;

		float4 offset = { 20.f, 120.f, 30.f, 0.f };
		offset = offset * transform_.GetTransformData().WorldWorld_;
		float4 startPosition = transform_.GetWorldPosition();
		startPosition += offset;

		float arrowSpeed = 1500.f;

		{
			PacketCreateProjectile packetArrow;
			packetArrow.MakeTargetProjectile(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *target_);
			packetArrow.SetType(eProjectileType::RioTargetArrow);
			FT::SendPacket(packetArrow);
		}
		{
			PacketCreateProjectile packetArrow;
			packetArrow.MakeTargetProjectile(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *target_);
			packetArrow.SetWaitTime(doubleStrikeDelay);
			packetArrow.SetType(eProjectileType::RioTargetArrow);
			FT::SendPacket(packetArrow);
		}

		if (GameServer::GetInstance()->IsOpened())
		{
			RioArrow* arrow = level_->CreateActor<RioArrow>();
			arrow->MakeTargetArrow(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *target_);

			arrow = level_->CreateActor<RioArrow>();
			arrow->MakeTargetArrow(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *target_);
			arrow->SetWaitTime(doubleStrikeDelay);
		}
	}

}

void Rio::onUpdateBasicAttacking(IUnit* _target, float _deltaTime)
{
}

void Rio::onStartQSkill()
{
	overrideAnimationBoneName_ = "Bip001 Spine2";
	if (bLongBow_)
	{
		overrideAnimationName_ = "SkillQ_Long";
	}
	else
	{
		overrideAnimationName_ = "SkillQ_Short";
	}

	renderer_->OverrideFBXAnimation(overrideAnimationName_, overrideAnimationBoneName_);


	//renderer_->OverrideFBXAnimation("SkillQ", "Bip001 L UpperArm");

	GameEngineSoundManager::GetInstance()->PlaySoundByName("Rio_Bow_Skill01_BowChange.wav");
	PacketSoundPlay packet;
	packet.SetSound("Rio_Bow_Skill01_BowChange.wav", transform_.GetWorldPosition());
	FT::SendPacket(packet);

	SetCooltimeQSkill(8.0f * stat_.CooldownReduction);
	bShortRSkillExtra_ = false;
}

void Rio::onUpdateQSkill(float _deltaTime)
{
	inputProcess(_deltaTime);
	moveProcess(_deltaTime);
	if (renderer_->IsOverrideAnimationEnd())
	{
		bLongBow_ = !bLongBow_;

		overrideAnimationName_ = "";
		overrideAnimationBoneName_ = "";
		renderer_->ClearOverrideAnimation();
		mainState_ << "NormalState";
	}
}

void Rio::onStartWSkill()
{
	if (mouse_ == nullptr)
	{
		changeAnimationWait();
		mainState_ << "NormalState";
		return;
	}

	destination_ = transform_.GetWorldPosition();
	destinations_.clear();
	float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);
	setRotationTo(mousePosition, transform_.GetWorldPosition());
	if (bLongBow_)
	{
		ChangeAnimation("SkillW_Long", true);
	}
	else
	{
		ChangeAnimation("SkillW_Short", true);
	}

	if (bLongBow_)
	{
		GameEngineSoundManager::GetInstance()->PlaySoundByName("Rio_LongBow_Skill02_Shot.wav");
		PacketSoundPlay packet;
		packet.SetSound("Rio_LongBow_Skill02_Shot.wav", transform_.GetWorldPosition());
		FT::SendPacket(packet);

		float4 offset = { 20.f, 120.f, 30.f, 0.f };
		offset = offset * transform_.GetTransformData().WorldWorld_;
		float4 startPosition = transform_.GetWorldPosition();
		startPosition += offset;

		PacketCreateProjectile packetArrow;
		packetArrow.MakeNonTargetProjectile(*this, stat_.AttackPower * 2.0f, startPosition, transform_.GetWorldRotation().y, 1000.f);
		packetArrow.SetLifeTime(0.7f);
		packetArrow.SetWaitTime(0.25f);
		packetArrow.SetScale({ 70.f, 150.0f, 100.f });
		packetArrow.SetType(eProjectileType::RioWSkillLong);
		FT::SendPacket(packetArrow);

		if (GameServer::GetInstance()->IsOpened())
		{
			RioArrow* arrow = level_->CreateActor<RioArrow>();
			arrow->MakeNonTargetArrow(*this, stat_.AttackPower * 2.0f, startPosition, transform_.GetWorldRotation().y, 1000.f);
			arrow->SetLifeTime(0.7f);
			arrow->SetWaitTime(0.25f);
			arrow->SetScale({ 70.f, 150.0f, 100.f });
		}
	}
	else
	{
		GameEngineSoundManager::GetInstance()->PlaySoundByName("Rio_ShortBow_Skill02_Shot.wav");
		PacketSoundPlay packet;
		packet.SetSound("Rio_ShortBow_Skill02_Shot.wav", transform_.GetWorldPosition());
		FT::SendPacket(packet);

		float4 offset = { 20.f, 120.f, 30.f, 0.f };
		offset = offset * transform_.GetTransformData().WorldWorld_;
		float4 startPosition = transform_.GetWorldPosition();
		startPosition += offset;

		for (size_t i = 0; i < 5; i++)
		{

			PacketCreateProjectile packetArrow;
			packetArrow.MakeNonTargetProjectile(*this, stat_.AttackPower / 2.0f, startPosition, transform_.GetWorldRotation().y - 10.f + 5 * i, 1000.f);
			packetArrow.SetWaitTime(0.05f * i);
			packetArrow.SetLifeTime(0.7f);
			packetArrow.SetType(eProjectileType::RioWSkillLong);
			FT::SendPacket(packetArrow);

		}

		if (GameServer::GetInstance()->IsOpened())
		{
			for (size_t i = 0; i < 5; i++)
			{
				RioArrow* arrow = level_->CreateActor<RioArrow>();
				arrow->MakeNonTargetArrow(*this, stat_.AttackPower / 2.0f, startPosition, transform_.GetWorldRotation().y - 10.f + 5 * i, 1000.f);
				arrow->SetWaitTime(0.05f * i);
				arrow->SetLifeTime(0.7f);
			}
		}
	}

	SetCooltimeWSkill(12.0f * stat_.CooldownReduction);
}

void Rio::onUpdateWSkill(float _deltaTime)
{
	if (renderer_->IsCurrentAnimationEnd())
	{
		mainState_ << "NormalState";
		changeAnimationWait();
	}
}

void Rio::onStartESkill()
{
}

void Rio::onUpdateESkill(float _deltaTime)
{
	if (mouse_ == nullptr)
	{
		changeAnimationWait();
		mainState_ << "NormalState";
		return;
	}
	float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);
	setRotationTo(mousePosition, transform_.GetWorldPosition());
	mainState_ << "CustomState";
	customState_ << "SkillEBegin";
}

void Rio::onStartRSkill()
{
	if (mouse_ == nullptr)
	{
		changeAnimationWait();
		mainState_ << "NormalState";
		return;
	}
	float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);
	setRotationTo(mousePosition, transform_.GetWorldPosition());

	if (bLongBow_)
	{
		ChangeAnimation("SkillR_Long");

		{
			GameEngineSoundManager::GetInstance()->PlaySoundByName("Rio_LongBow_SKill04_01.wav");
			PacketSoundPlay packet;
			packet.SetSound("Rio_LongBow_SKill04_01.wav", transform_.GetWorldPosition());
			FT::SendPacket(packet);
		}

		float4 offset = { 20.f, 120.f, 30.f, 0.f };
		offset = offset * transform_.GetTransformData().WorldWorld_;
		float4 startPosition = transform_.GetWorldPosition();
		startPosition += offset;

		PacketCreateProjectile packetArrow;
		packetArrow.MakeNonTargetProjectile(*this, stat_.AttackPower * 6.0f, startPosition, transform_.GetWorldRotation().y, 700.f);
		packetArrow.SetLifeTime(1.0f);
		packetArrow.SetWaitTime(1.4f);
		packetArrow.SetScale({ 100.f, 150.0f, 150.f });
		packetArrow.SetType(eProjectileType::RioRSkillLong);
		FT::SendPacket(packetArrow);

		if (GameServer::GetInstance()->IsOpened())
		{
			RioArrow* arrow = level_->CreateActor<RioArrow>();
			arrow->MakeNonTargetArrow(*this, stat_.AttackPower * 6.0f, startPosition, transform_.GetWorldRotation().y, 700.f);
			arrow->SetLifeTime(1.0f);
			arrow->SetWaitTime(1.4f);
			arrow->SetScale({ 100.f, 150.0f, 150.f });
			arrow->SetKnockback(true);
		}
	}
	else
	{
		if (bShortRSkillExtra_)
		{
			ChangeAnimation("SkillR_Short_End");

			float4 offset = { 20.f, 120.f, 30.f, 0.f };
			offset = offset * transform_.GetTransformData().WorldWorld_;
			float4 startPosition = transform_.GetWorldPosition();
			startPosition += offset;

			PacketCreateProjectile packetArrow;
			packetArrow.MakeNonTargetProjectile(*this, stat_.AttackPower * 3.0f, startPosition, transform_.GetWorldRotation().y, 1000.f);
			packetArrow.SetLifeTime(0.5f);
			packetArrow.SetWaitTime(0.3f);
			packetArrow.SetScale({ 80.f, 150.0f, 100.f });
			packetArrow.SetType(eProjectileType::RioRSkillShortImpact);
			FT::SendPacket(packetArrow);

			if (GameServer::GetInstance()->IsOpened())
			{
				RioArrow* arrow = level_->CreateActor<RioArrow>();
				arrow->MakeNonTargetArrow(*this, stat_.AttackPower * 3.0f, startPosition, transform_.GetWorldRotation().y, 1000.f);
				arrow->SetLifeTime(0.5f);
				arrow->SetWaitTime(0.3f);
				arrow->SetScale({ 80.f, 150.0f, 100.f });
				arrow->SetKnockback(true);
			}
		}
		else
		{

			ChangeAnimation("SkillR_Short_Start");

			float4 offset = { 20.f, 120.f, 30.f, 0.f };
			offset = offset * transform_.GetTransformData().WorldWorld_;
			float4 startPosition = transform_.GetWorldPosition();
			startPosition += offset;

			for (int i = 0; i < 3; i++)
			{
				PacketCreateProjectile packetArrow;
				packetArrow.MakeNonTargetProjectile(*this, stat_.AttackPower, startPosition, transform_.GetWorldRotation().y, 1200.f);
				packetArrow.SetLifeTime(0.5f);
				packetArrow.SetWaitTime(0.4f + 0.1f * i);
				packetArrow.SetScale({ 70.f, 150.0f, 100.f });
				packetArrow.SetType(eProjectileType::RioRSkillShort);
				FT::SendPacket(packetArrow);

				if (GameServer::GetInstance()->IsOpened())
				{
					RioArrow* arrow = level_->CreateActor<RioArrow>();
					arrow->MakeNonTargetArrow(*this, stat_.AttackPower, startPosition, transform_.GetWorldRotation().y, 1200.f);
					arrow->SetLifeTime(0.5f);
					arrow->SetWaitTime(0.4f + 0.1f * i);
					arrow->SetScale({ 70.f, 150.0f, 100.f });
				}
			}
		}
	}

	skillRTime_ = -0.4f;
}

void Rio::onUpdateRSkill(float _deltaTime)
{
	skillRTime_ += _deltaTime;
	if (renderer_->IsCurrentAnimationEnd())
	{
		changeAnimationWait();
		mainState_ << "NormalState";
	}

	if (attackState_.GetCurrentState()->Time_ < 0.0f)
	{
		return;
	}

	if (bLongBow_)
	{
		if (attackState_.GetCurrentState()->Time_ > 0.5f)
		{
			GameEngineSoundManager::GetInstance()->PlaySoundByName("Rio_LongBow_Skill04_02.wav");
			PacketSoundPlay packet;
			packet.SetSound("Rio_LongBow_Skill04_02.wav", transform_.GetWorldPosition());
			FT::SendPacket(packet);

			attackState_.GetCurrentState()->Time_ = -5.f;
		}

	}
	else
	{
		if (bShortRSkillExtra_)
		{
			if (attackState_.GetCurrentState()->Time_ > 0.3f)
			{
				GameEngineSoundManager::GetInstance()->PlaySoundByName("Rio_ShortBow_Skill04_02.wav");
				PacketSoundPlay packet;
				packet.SetSound("Rio_ShortBow_Skill04_02.wav", transform_.GetWorldPosition());
				FT::SendPacket(packet);
				attackState_.GetCurrentState()->Time_ = -5.f;
				bShortRSkillExtra_ = false;
			}
		}
		else
		{
			if (skillRTime_ > 0.12f)
			{
				GameEngineSoundManager::GetInstance()->PlaySoundByName("Rio_ShortBow_Skill04_02.wav");
				PacketSoundPlay packet;
				packet.SetSound("Rio_ShortBow_Skill04_02.wav", transform_.GetWorldPosition());
				FT::SendPacket(packet);
				skillRTime_ = 0.0f;
			}

			if (attackState_.GetCurrentState()->Time_ > 0.8f)
			{
				attackState_.GetCurrentState()->Time_ = -5.f;
				bShortRSkillExtra_ = true;
			}
		}
	}


}

void Rio::onStartDSkill()
{
	if (mouse_ == nullptr)
	{
		changeAnimationWait();
		mainState_ << "NormalState";
		return;
	}

	destination_ = transform_.GetWorldPosition();
	destinations_.clear();
	float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);
	setRotationTo(mousePosition, transform_.GetWorldPosition());

	if (bLongBow_)
	{
		ChangeAnimation("SkillD_Long", true);
	}
	else
	{
		ChangeAnimation("SkillD_Short", true);
	}

	FT::PlaySoundAndSendPacket("skillReadyBow_in.wav", transform_.GetWorldPosition());

	PacketCreateProjectile packetArrow;
	packetArrow.MakeNonTargetProjectile(*this, stat_.AttackPower * 5, mousePosition, transform_.GetWorldRotation().y, 1200.f);
	packetArrow.SetType(eProjectileType::RioDSkill);
	FT::SendPacket(packetArrow);

	if (GameServer::GetInstance()->IsOpened())
	{
		RioDSkill* skill = level_->CreateActor<RioDSkill>();
		skill->SetOwner(this);
		skill->GetTransform()->SetWorldPosition(mousePosition);
		skill->SetDamage(stat_.AttackPower * 5);
		skill->SetWaitTime(0.0f);
	}
}

void Rio::onUpdateDSkill(float _deltaTime)
{
	if (attackState_.GetCurrentState()->Time_ > 0.2f)
	{
		FT::PlaySoundAndSendPacket("attackBow_in_r1.wav", transform_.GetWorldPosition());



		attackState_.GetCurrentState()->Time_ = -10.f;
	}
	
	if (renderer_->IsCurrentAnimationEnd())
	{
		changeAnimationWait();
		mainState_ << "NormalState";
	}
}

void Rio::onStartDeath()
{
	changeDeathAnimation();
}

void Rio::onUpdateDeath(float _deltaTime)
{
}

void Rio::onUpdateCustomState(float _deltaTime)
{
	customState_.Update(_deltaTime);
}

void Rio::onPlayEffect(const std::string& _effectName)
{
}

void Rio::startSkillEBegin()
{
	float height = 0.0f;
	startPosition_ = transform_.GetWorldPosition();
	landingPosition_ = transform_.GetWorldPosition();
	landingPosition_ += transform_.GetWorldForwardVector() * 500.f;

	float4 landingPosition = landingPosition_;
	landingPosition.y += FT::Map::MAX_HEIGHT;

	bSkillEPassable_ = currentMap_->GetNavMesh()->CheckIntersects(landingPosition, float4::DOWN, height);

	ChangeAnimation("SkillE_Short");
	skillETime_ = 0.0f;
}

void Rio::updateSkillEBegin(float _deltaTime)
{
	skillETime_ += _deltaTime;
	if (customState_.GetCurrentState()->Time_ > 0.2f)
	{
		customState_ << "SkillEShot";
	}

	if (bSkillEPassable_)
	{
		transform_.SetWorldPosition(float4::Lerp(startPosition_, landingPosition_, skillETime_ / 0.5f));
	}
}

void Rio::startSkillEShot()
{
	GameEngineSoundManager::GetInstance()->PlaySoundByName("Rio_Bow_Skill03_Start.wav");
	PacketSoundPlay packet;
	packet.SetSound("Rio_Bow_Skill03_Start.wav", transform_.GetWorldPosition());
	FT::SendPacket(packet);

	auto list = skillECollision_->GetCollisionList(eCollisionGroup::Player);



	float doubleStrikeDelay = (stat_.AttackEndTime - stat_.AttackStartTime) / stat_.AttackSpeed / 2.0f;

	//float4 offset = { 20.f, 120.f, 30.f, 0.f };
	//offset = offset * transform_.GetTransformData().WorldWorld_;
	float4 startPosition = transform_.GetWorldPosition();
	startPosition.y += 300.f;
	//startPosition += offset;

	float arrowSpeed = 1500.f;

	for (GameEngineCollision* col : list)
	{
		Character* c = dynamic_cast<Character*>(col->GetActor());
		if (nullptr != c)
		{
			if (c == this)
			{
				continue;
			}
			{
				PacketCreateProjectile packetArrow;
				packetArrow.MakeTargetProjectile(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *c);
				packetArrow.SetType(eProjectileType::RioTargetArrow);
				FT::SendPacket(packetArrow);
			}
			{
				PacketCreateProjectile packetArrow;
				packetArrow.MakeTargetProjectile(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *c);
				packetArrow.SetWaitTime(doubleStrikeDelay);
				packetArrow.SetType(eProjectileType::RioTargetArrow);
				FT::SendPacket(packetArrow);
			}

			if (GameServer::GetInstance()->IsOpened())
			{
				RioArrow* arrow = level_->CreateActor<RioArrow>();
				arrow->MakeTargetArrow(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *c);

				arrow = level_->CreateActor<RioArrow>();
				arrow->MakeTargetArrow(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *c);
				arrow->SetWaitTime(doubleStrikeDelay);
			}
		}

	}
}

void Rio::updateSkillEShot(float _deltaTime)
{
	if (customState_.GetCurrentState()->Time_ > 0.2f)
	{
		customState_ << "SkillEEnd";
	}

	skillETime_ += _deltaTime;
	if (bSkillEPassable_)
	{

		if (bSkillEPassable_)
		{
			transform_.SetWorldPosition(float4::Lerp(startPosition_, landingPosition_, skillETime_ / 0.5f));
		}
	}
}

void Rio::startSkillEEnd()
{
	GameEngineSoundManager::GetInstance()->PlaySoundByName("Rio_Bow_Skill03_End.wav");
	PacketSoundPlay packet;
	packet.SetSound("Rio_Bow_Skill03_End.wav", transform_.GetWorldPosition());
	FT::SendPacket(packet);
}

void Rio::updateSkillEEnd(float _deltaTime)
{
	if (renderer_->IsCurrentAnimationEnd() || skillETime_ > 0.6f)
	{
		destination_ = transform_.GetWorldPosition();
		destinations_.clear();
		changeAnimationWait();
		mainState_ << "NormalState";
		return;
	}


	skillETime_ += _deltaTime;
	if (bSkillEPassable_)
	{
		if (bSkillEPassable_)
		{
			transform_.SetWorldPosition(float4::Lerp(startPosition_, landingPosition_, skillETime_ / 0.5f));
		}
	}
}

