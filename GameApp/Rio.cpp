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

Rio::Rio()
	: Character()
	, bLongBow_(false)
	, skillECollision_(nullptr)
	, bSkillEPassable_(false)
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

	GameEngineFBXMeshManager::GetInst().Delete("Rio_Short_Run.fbx");
}

void Rio::Start()
{
	Character::Start();

	skillECollision_ = CreateTransformComponent<GameEngineCollision>();
	skillECollision_->SetCollisionType(CollisionType::Sphere3D);
	skillECollision_->GetTransform()->SetLocalScaling(500.f);

	
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

	//renderer_->CreateFBXAnimation("SkillW_Short", "Rio_Short_Skill_W.UserAnimation", 0, false);
	//renderer_->CreateFBXAnimation("SkillW_Long", "Rio_Short_Skill_W.UserAnimation", 0, false);

	renderer_->ChangeFBXAnimation("Wait_Short");
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
		//GameEngineSoundManager::GetInstance()->PlaySoundByName("Rio_LongBow_NormalAttack_01.wav");
		ChangeAnimation("BasicAttack_Long", true);
	}
	else
	{
		ChangeAnimation("BasicAttack_Short", true);
	}
}

void Rio::onStartBasicAttacking(Character* _target)
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
			packetArrow.MakeTargetArrow(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *target_);
			FT::SendPacket(packetArrow);
		}
		{
			PacketCreateProjectile packetArrow;
			packetArrow.MakeTargetArrow(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *target_);
			packetArrow.SetWaitTime(doubleStrikeDelay);
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

void Rio::onUpdateBasicAttacking(Character* _target, float _deltaTime)
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
		packetArrow.MakeNonTargetArrow(*this, stat_.AttackPower * 2.0f, startPosition, transform_.GetWorldRotation().y, 1000.f);
		packetArrow.SetLifeTime(0.7f);
		packetArrow.SetWaitTime(0.25f);
		packetArrow.SetScale({ 70.f, 150.0f, 100.f });
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
			packetArrow.MakeNonTargetArrow(*this, stat_.AttackPower / 2.0f, startPosition, transform_.GetWorldRotation().y - 10.f + 5 * i, 1000.f);
			packetArrow.SetWaitTime(0.05f * i);
			packetArrow.SetLifeTime(0.7f);
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
	mainState_ << "NormalState";
}

void Rio::onStartRSkill()
{
}

void Rio::onUpdateRSkill(float _deltaTime)
{
	mainState_ << "NormalState";
}

void Rio::onStartDSkill()
{
}

void Rio::onUpdateDSkill(float _deltaTime)
{
}

void Rio::onStartDeath()
{
}

void Rio::onUpdateDeath(float _deltaTime)
{
}

void Rio::onUpdateCustomState(float _deltaTime)
{
	customState_.Update(_deltaTime);
}

void Rio::startSkillEBegin()
{
}

void Rio::updateSkillEBegin(float _deltaTime)
{
}

void Rio::startSkillEShot()
{
}

void Rio::updateSkillEShot(float _deltaTime)
{
}

void Rio::startSkillEEnd()
{
}

void Rio::updateSkillEEnd(float _deltaTime)
{
}

