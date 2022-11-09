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
#include "YukiREffect.h"
#include "YukiQEffect.h"
#include "YukiQSlashEffect.h"
#include "PlayerUIController.h"
#include "Monsters.h"

Yuki::Yuki() // default constructer 디폴트 생성자
	: b_isQ_(false), timer_Q(0.0f), rEffect_(nullptr), timer_R(0.0f), b_RHit_(false)
	, swordRenderer_(nullptr), rExplodeTimer_(1.0f), passiveToken_(4), passiveDamage_(0.0f)
{

}

Yuki::~Yuki() // default destructer 디폴트 소멸자
{

}

void Yuki::LoadResource()
{
	GameEngineDirectory dir;

	dir.MoveParent("FoxTeam");
	dir / "Resources" / "FBX" / "Character" / "Yuki";

	GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Yuki_run.fbx"));
	mesh->CreateRenderingBuffer();

	mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Weapon_TwoHandSword_01.fbx"));
	mesh->CreateRenderingBuffer();

	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Weapon_TwoHandSword_01.fbx"));

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

void Yuki::Show()
{
	Character::Show();
	swordRenderer_->On();
}

void Yuki::Hide()
{
	Character::Hide();
	swordRenderer_->Off();
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

	//stat_.Cooltime_q = 5.0f;
	//stat_.Cooltime_w = 14.0f;
	//stat_.Cooltime_e = 18.0f;
	//stat_.Cooltime_r = 100.0f;

	DefaultCool_Q_ = 5.0f;
	DefaultCool_W_ = 14.0f;
	DefaultCool_E_ = 18.0f;
	DefaultCool_R_ = 100.0f;
	DefaultCool_D_ = 30.0f;

	stat_.passive_Count = 4;

	passiveDamage_ = 15.0f;
	passiveToken_ = 4;

	SetEquipBuildItem("TacticalOPSHelmet", EquipmentType::HEAD);
	SetEquipBuildItem("Changpao", EquipmentType::CHEST);
	SetEquipBuildItem("Radar", EquipmentType::ARM);
	SetEquipBuildItem("Bucephalus", EquipmentType::LEG);
	SetEquipBuildItem("BuccaneerDoubloon", EquipmentType::ACCESSORY);
	SetEquipBuildItem("Monohoshizao", EquipmentType::WEAPON);

	checkBuildItems();
}

void Yuki::Update(float _deltaTime)
{
	Character::Update(_deltaTime);

	// 패시브 체크
	if (passiveToken_ < 4 && false == isInfight_)
	{
		passiveToken_ = 4;
	}

	//패시브를 stat_.passive_Count와 연동
	stat_.passive_Count = passiveToken_;


	// 내 쪽에서 패시브 토큰이 실제로 감산될 때
	// 다른 쪽 컴퓨터의 내 꼭두각시도 그걸 알아야 한다...
	// 
	// Q 온힛 체크
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

	// R 피격자 체크
	if (rUnitList_.size() != 0)
	{
		rExplodeTimer_ -= _deltaTime;

		if (0.0f >= rExplodeTimer_)
		{
			for (int i = 0; i < rUnitList_.size(); i++)
			{
				FT::PlaySoundAndSendPacket("Yuki_Skill04_Debuff.wav", transform_.GetWorldPosition());
				FT::PlaySoundAndSendPacket("Yuki_Skill04_Debuff_Hit.wav", transform_.GetWorldPosition());

				Character* character = dynamic_cast<Character*>(rUnitList_[i]);
				if (nullptr != character)
				{
					rUnitList_[i]->Damage(character->GetStat()->HPMax * (0.125f + (0.025f * stat_.Level_r)), this);
					continue;
				}

				Monsters* monster = dynamic_cast<Monsters*>(rUnitList_[i]);
				if (nullptr != monster)
				{
					rUnitList_[i]->Damage(monster->GetMonsterStateInfo().HPMax_ * (0.125f + (0.025f * stat_.Level_r)), this);
					continue;
				}

				// 여기에 이펙트 배치
				// 잔상 이펙트는 캐릭터 수 만큼 있어야 할 것...

			}

			rExplodeTimer_ = 1.0f;
			rUnitList_.clear();
		}
	}

	GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
	if (nullptr != controlWindow)
	{
		controlWindow->AddText("Yuki Passive Token Count : " + std::to_string(passiveToken_));
		controlWindow->AddText("Yuki Passive Token Count(Online) : " + std::to_string(PlayerInfoManager::GetInstance()->GetMyPlayer().stat_->passive_Count));
		controlWindow->AddText("WorldPos : " + std::to_string(transform_.GetWorldPosition().x) + " " + std::to_string(transform_.GetWorldPosition().z));
		controlWindow->AddText("CurLocation : " + std::to_string(static_cast<int>(curLocation_)));

	//	controlWindow->AddText("MinimapLocation : " + std::to_string(GetUIController()->GetMinimapUI()->GetIconList()[myIndex_]->GetTransform()->GetWorldPosition().x + ',' + GetUIController()->GetMinimapUI()->GetIconList()[myIndex_]->GetTransform()->GetWorldPosition().y));
	}
}

void Yuki::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>(static_cast<int>(ObjectRenderOrder::CHARACTER));
	renderer_->SetFBXMesh("Yuki_run.fbx", "TextureDeferredLightAni", true);

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

	renderer_->CreateFBXAnimation("Craft", "Yuki_Craft.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("Operate", "Yuki_Operate.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("Arrive", "Yuki_Arrive.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("RestStart", "Yuki_Rest_Start.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("RestLoop", "Yuki_Rest_Loop.UserAnimation", 0, true);
	renderer_->CreateFBXAnimation("RestEnd", "Yuki_Rest_End.UserAnimation", 0, false);

	renderer_->ChangeFBXAnimation("Wait");

	renderer_->GetRenderSet(1).isRender = false;

	swordRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>(static_cast<int>(ObjectRenderOrder::WEAPON));
	swordRenderer_->SetFBXMesh("Weapon_TwoHandSword_01.fbx", "TextureDeferredLightAni");

	swordRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	swordRenderer_->GetTransform()->SetLocalScaling(100.f);
	swordRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f, 0.0f });

	swordRenderer_->SetParentBoneName(renderer_, "Bip001 R Finger2");
	swordRenderer_->SetCustomOffset({ -2.5f, 0.f, 0.f });
	swordRenderer_->CreateFBXAnimation("Idle", "Weapon_TwoHandSword_01.fbx");
	swordRenderer_->ChangeFBXAnimation("Idle");

	// 그림자 
	{
		MainShadowRenderer_ = CreateTransformComponent<GameEngineShadowRenderer>();
		MainShadowRenderer_->SetBaseRenderer(renderer_, "RendererShadow");

		WeaponShadowRenderer1_ = CreateTransformComponent<GameEngineShadowRenderer>();
		WeaponShadowRenderer1_->SetBaseRenderer(swordRenderer_, "RendererShadow");
	}

	// 외곽선
	{
		// Main
		MainOutLineRenderer_ = CreateTransformComponent<GameEnginePreprocessingRenderer>();
		MainOutLineRenderer_->SetBaseRenderer(renderer_, "PreprocessingAni", true, false);

		OutLineData MainOutLineData = MainOutLineRenderer_->GetOutLineData();
		float4 MainThickness = float4(1.0f, 1.0f, 1.0f, 0.0f) * MainOutLineData.LineThickness;
		float4 MainCalcScale = renderer_->GetTransform()->GetLocalScaling() + MainThickness;
		MainOutLineRenderer_->GetTransform()->SetLocalScaling(MainCalcScale);
		MainOutLineRenderer_->GetTransform()->SetLocalRotationDegree(renderer_->GetTransform()->GetLocalRotation());
		MainOutLineRenderer_->GetTransform()->SetLocalPosition(renderer_->GetTransform()->GetLocalPosition());
		MainOutLineRenderer_->GetRenderSet(1).isRender = false;
		MainOutLineRenderer_->Off();

		// Weapon1
		WeaponOutLineRenderer1_ = CreateTransformComponent<GameEnginePreprocessingRenderer>();
		WeaponOutLineRenderer1_->SetBaseRenderer(swordRenderer_, "PreprocessingAni", true, false);

		OutLineData Weapon1OutLineData = WeaponOutLineRenderer1_->GetOutLineData();
		float4 Weapon1Thickness = float4(1.0f, 1.0f, 1.0f, 0.0f) * Weapon1OutLineData.LineThickness;
		float4 CalcWeapon1Scale = swordRenderer_->GetTransform()->GetLocalScaling() + Weapon1Thickness;
		WeaponOutLineRenderer1_->GetTransform()->SetLocalScaling(CalcWeapon1Scale);
		WeaponOutLineRenderer1_->GetTransform()->SetLocalRotationDegree(swordRenderer_->GetTransform()->GetLocalRotation());
		WeaponOutLineRenderer1_->GetTransform()->SetLocalPosition(swordRenderer_->GetTransform()->GetLocalPosition());
		WeaponOutLineRenderer1_->Off();
	}

	// 실루엣
	{
		// Main
		MainSilhouetteRenderer_ = CreateTransformComponent<GameEnginePreprocessingRenderer>();
		MainSilhouetteRenderer_->SetBaseRenderer(renderer_, "PreprocessingAni", true);

		OutLineData MainOutLineData = MainSilhouetteRenderer_->GetOutLineData();
		float4 MainThickness = float4(1.0f, 1.0f, 1.0f, 0.0f) * MainOutLineData.LineThickness;
		float4 MainCalcScale = renderer_->GetTransform()->GetLocalScaling() + MainThickness;
		MainSilhouetteRenderer_->GetTransform()->SetLocalScaling(MainCalcScale);
		MainSilhouetteRenderer_->GetTransform()->SetLocalRotationDegree(renderer_->GetTransform()->GetLocalRotation());
		MainSilhouetteRenderer_->GetTransform()->SetLocalPosition(renderer_->GetTransform()->GetLocalPosition());
		MainSilhouetteRenderer_->GetRenderSet(1).isRender = false;

		// Weapon1
		WeaponSilhouetteRenderer1_ = CreateTransformComponent<GameEnginePreprocessingRenderer>();
		WeaponSilhouetteRenderer1_->SetBaseRenderer(swordRenderer_, "PreprocessingAni", true);

		OutLineData Weapon1OutLineData = WeaponSilhouetteRenderer1_->GetOutLineData();
		float4 Weapon1Thickness = float4(1.0f, 1.0f, 1.0f, 0.0f) * Weapon1OutLineData.LineThickness;
		float4 CalcWeapon1Scale = swordRenderer_->GetTransform()->GetLocalScaling() + Weapon1Thickness;
		WeaponSilhouetteRenderer1_->GetTransform()->SetLocalScaling(CalcWeapon1Scale);
		WeaponSilhouetteRenderer1_->GetTransform()->SetLocalRotationDegree(swordRenderer_->GetTransform()->GetLocalRotation());
		WeaponSilhouetteRenderer1_->GetTransform()->SetLocalPosition(swordRenderer_->GetTransform()->GetLocalPosition());
	}
}

void Yuki::initYukiCollision()
{
	collision_E = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_E->GetTransform()->SetLocalPosition({ 0.f,0.f,0.f });
	collision_E->GetTransform()->SetLocalScaling({ 150.f });
	collision_E->SetCollisionGroup(eCollisionGroup::PlayerAttack);
	collision_E->SetCollisionType(CollisionType::OBBBox3D);
	collision_E->Off();

	collision_R = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_R->GetTransform()->SetLocalPosition({ 0.f,10.f,200.f });
	collision_R->GetTransform()->SetLocalScaling({ 768.0f, 10.0f, 512.0f });
	collision_R->SetCollisionGroup(eCollisionGroup::PlayerAttack);
	collision_R->SetCollisionType(CollisionType::OBBBox3D);
	collision_R->Off();
}

void Yuki::initYukiCustomState()
{
	customState_.CreateState(MakeStateWithEnd(Yuki, CustomRStandBy));
	customState_.CreateState(MakeStateWithEnd(Yuki, CustomRSlash));
	customState_.CreateState(MakeStateWithEnd(Yuki, CustomRAfterBurst));
	customState_ << "CustomRStandBy";
}

void Yuki::initEffectRenderer()
{
	float4 bsicAtkOriScale = { 256.0f, 42.7f };
	basicAttackEffect_ = GetLevel()->CreateActor<BasicAttackEffect>();
	basicAttackEffect_->GetAttackRenderer()->SetImage("Fx_SQ_Cut01.png", "PointSmp");
	basicAttackEffect_->GetAttackRenderer()->GetTransform()->SetLocalPosition({ 0.0f,0.0f,0.0f });
	basicAttackEffect_->GetAttackRenderer()->GetTransform()->SetLocalRotationDegree({ 90.0f,0.0f,0.0f });
	basicAttackEffect_->GetAttackRenderer()->GetTransform()->SetLocalScaling(bsicAtkOriScale * 1.5f);
	basicAttackEffect_->GetAttackRenderer()->CreateAnimation("Fx_SQ_Cut01.png", "Fx_SQ_Cut01", 0, 5, 0.03f, false);

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

	rEffect_ = GetLevel()->CreateActor<YukiREffect>();
	rEffect_->SetParent(this);

	qEffect_ = GetLevel()->CreateActor<YukiQEffect>();
	qEffect_->SetParent(this);

	qSlashEffect_ = GetLevel()->CreateActor<YukiQSlashEffect>();
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
	if (b_isQ_)
	{
		curAnimationName_ = "SkillQ";
		renderer_->ChangeFBXAnimation("SkillQ", true);
		return;
	}

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

	int random = randomGenerator.RandomInt(0, 1);

	PacketSoundPlay packet0;

	switch (random)
	{
	case 0:
		GameEngineSoundManager::GetInstance()->PlaySoundByName("attackTwoHandSword_r1.wav");
		packet0.SetSound("attackTwoHandSword_r1.wav", transform_.GetWorldPosition());
		break;
	case 1:
		GameEngineSoundManager::GetInstance()->PlaySoundByName("attackTwoHandSword_r2.wav");
		packet0.SetSound("attackTwoHandSword_r2.wav", transform_.GetWorldPosition());
		break;
	default:
		break;
	}
	FT::SendPacket(packet0);
}

void Yuki::changeDeathAnimation()
{
	ChangeAnimation("Death", true);
}

void Yuki::changeAnimationOperate()
{
	ChangeAnimation("Operate");
}

void Yuki::changeAnimationArrive()
{
	ChangeAnimation("Arrive");
}

void Yuki::changeAnimationRestStart()
{
	ChangeAnimation("RestStart");
}

void Yuki::changeAnimationRestLoop()
{
	ChangeAnimation("RestLoop");
}

void Yuki::changeAnimationRestEnd()
{
	ChangeAnimation("RestEnd");
}

void Yuki::changeAnimationCraft()
{
	ChangeAnimation("Craft");
}

void Yuki::onStartBasicAttacking(IUnit* _target)
{
	if (b_isQ_)
	{
		RandomSoundPlay("Yuki_PlaySkill1011200seq1_1_ko.wav", "Yuki_PlaySkill1011200seq1_2_ko.wav", "Yuki_PlaySkill1011200seq1_3_ko.wav");

		if (passiveToken_ <= 0)
		{
			target_->Damage(stat_.AttackPower * 1.6f + (20.0f * stat_.Level_q), this);

			target_->Slow(1.0f, 0.5f);
			CharCrowdControlPacket ccPacket;
			ccPacket.SetTargetIndex(target_->GetIndex());
			ccPacket.SetSlow(1.0f, 0.5f);
			FT::SendPacket(ccPacket);
		}
		else
		{
			if (GameServer::GetInstance()->IsOpened())
			{
				passiveToken_--;
				stat_.passive_Count = passiveToken_;
			}

			target_->Damage(stat_.AttackPower * 1.6f + (20.0f * stat_.Level_q) + (passiveDamage_ * stat_.Level_passive), this);

			target_->Stun(0.5f);

			CharCrowdControlPacket ccPacket;
			ccPacket.SetTargetIndex(target_->GetIndex());
			ccPacket.SetStun(0.5f);
			FT::SendPacket(ccPacket);
		}


		GameEngineSoundManager::GetInstance()->PlaySoundByName("Yuki_Skill01_Attack.wav");
		PacketSoundPlay packet0;
		packet0.SetSound("Yuki_Skill01_Attack.wav", transform_.GetWorldPosition());
		FT::SendPacket(packet0);

		GameEngineSoundManager::GetInstance()->PlaySoundByName("Yuki_Passive_Hit_r2.wav");
		PacketSoundPlay packet;
		packet.SetSound("Yuki_Passive_Hit_r2.wav", transform_.GetWorldPosition());
		FT::SendPacket(packet);

		qSlashEffect_->PlayBurst(_target->GetTransform()->GetWorldPosition());
		qSlashEffect_->GetTransform()->SetWorldPosition(_target->GetTransform()->GetWorldPosition());

		CharEffectPacket pack;
		pack.SetTargetIndex(myIndex_);
		pack.SetVictimIndex(_target->GetIndex());
		pack.SetAnimationName("SkillQ_Slash");
		FT::SendPacket(pack);

		b_isQ_ = false;

		return;
	}


	target_->Damage(stat_.AttackPower, this);

	GameEngineSoundManager::GetInstance()->PlaySoundByName("Yuki_Passive_Hit_r2.wav");
	PacketSoundPlay packet;
	packet.SetSound("Yuki_Passive_Hit_r2.wav", transform_.GetWorldPosition());
	FT::SendPacket(packet);

	float4 wp = target_->GetTransform()->GetWorldPosition();
	wp.y += 50.0f;
	basicAttackEffect_->GetTransform()->SetWorldPosition(wp);
	basicAttackEffect_->PlayAwake("Fx_SQ_Cut01");

	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("BasicAttack");
	pack.SetVictimIndex(_target->GetIndex());
	FT::SendPacket(pack);
}

void Yuki::onUpdateBasicAttacking(IUnit* _target, float _deltaTime)
{

}

void Yuki::onStartQSkill()
{
	qEffect_->GetTransform()->SetWorldPosition(swordRenderer_->GetTransform()->GetWorldPosition());
	qEffect_->PlayAwake();
	b_isQ_ = true;
	timer_Q = 2.0f;


	GameEngineSoundManager::GetInstance()->PlaySoundByName("Yuki_Skill01_Active.wav");
	PacketSoundPlay packet;
	packet.SetSound("Yuki_Skill01_Active.wav", transform_.GetWorldPosition());
	FT::SendPacket(packet);

	qEffect_->PlayAwake();

	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("SkillQ_Start");
	FT::SendPacket(pack);
}

void Yuki::onUpdateQSkill(float _deltaTime)
{
	changeAnimationWait();
	mainState_.ChangeState("NormalState", true);
	normalState_.ChangeState("Watch", true);
}

void Yuki::onStartWSkill()
{
	coolTimer_E_ -= 3.0f;

	// 바닥에 벚꽃 이펙트 재생
	RandomSoundPlay("Yuki_PlaySkill1011300seq0_1_ko.wav", "Yuki_PlaySkill1011300seq0_2_ko.wav", "Yuki_PlaySkill1011300seq0_3_ko.wav");
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

	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("SkillW");
	FT::SendPacket(pack);
}

void Yuki::onUpdateWSkill(float _deltaTime)
{
	//changeAnimationWait();
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

	RandomSoundPlay("Yuki_PlaySkill1011400seq0_1_ko.wav", "Yuki_PlaySkill1011400seq0_2_ko.wav", "");

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


		{
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
		}

		{
			// 여기서 피격 충돌 판정이 나옴
			auto collisionList = collision_E->GetCollisionList(eCollisionGroup::Monster);

			if (false == b_Ehit_)
			{
				for (GameEngineCollision* col : collisionList)
				{
					GameEngineActor* actor = col->GetActor();
					IUnit* character = nullptr;

					if (nullptr != actor)
					{
						character = dynamic_cast<IUnit*>(actor);

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
	mainState_.ChangeState("CustomState", true);
	customState_.ChangeState("CustomRStandBy", true);
	setRotationToMouse();
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
	changeDeathAnimation();
}

void Yuki::onUpdateDeath(float _deltaTime)
{
}

void Yuki::onUpdateCustomState(float _deltaTime)
{
	customState_.Update(_deltaTime);
	if (true == collision_R->IsUpdate())
	{
		GetLevel()->PushDebugRender(collision_R->GetTransform(), CollisionType::OBBBox3D, float4::RED);
	}
}

void Yuki::onPlayEffect(const std::string& _effectName, IUnit* _victim)
{

	if ("BasicAttack" == _effectName)
	{
		if (_victim != nullptr)
		{
			float4 wp = _victim->GetTransform()->GetWorldPosition();
			wp.y += 50.0f;
			basicAttackEffect_->GetTransform()->SetWorldPosition(wp);
		}

		basicAttackEffect_->PlayAwake("Fx_SQ_Cut01");
		return;
	}

	if ("SkillQ_Start" == _effectName)
	{
		qEffect_->PlayAwake();
		return;
	}

	if ("SkillQ_Slash" == _effectName)
	{
		qSlashEffect_->PlayBurst(_victim->GetTransform()->GetWorldPosition());
		passiveToken_--;
		stat_.passive_Count = passiveToken_;
		return;
	}

	if ("SkillW" == _effectName)
	{
		groundEffectRenderer_->On();
		groundEffectRenderer_->SetChangeAnimation("FX_BI_Yuki_01SE", true);
		groundEffectRenderer_->AnimationPlay();
		return;
	}

	if ("SkillE" == _effectName)
	{
		rearEffectRenderer_->On();
		rearEffectRenderer_->SetChangeAnimation("FX_BI_Yuki_01", true);
		rearEffectRenderer_->AnimationPlay();
		return;
	}

	if ("SkillR_awake" == _effectName)
	{
		rEffect_->PlayAwake();
		return;
	}

	if ("SkillR_slash" == _effectName)
	{
		rEffect_->PlaySlash();
		return;
	}
}

void Yuki::onEffectTransformCheck(float _deltaTime)
{
	float4 wp = GetTransform()->GetWorldPosition();
	rEffect_->GetTransform()->SetLocalPosition(wp);
	rEffect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());

	qEffect_->GetTransform()->SetLocalPosition(wp);
	qEffect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());

}

void Yuki::onLevelUp()
{
	switch (stat_.Level_q)
	{
	case 1:
		DefaultCool_W_ = 14.0f;
		DefaultCool_E_ = 18.0f;
		DefaultCool_R_ = 100.0f;
		break;
	case 2:
		DefaultCool_W_ = 13.0f;
		DefaultCool_E_ = 17.0f;
		DefaultCool_R_ = 90.0f;
		break;
	case 3:
		DefaultCool_W_ = 12.0f;
		DefaultCool_E_ = 16.0f;
		DefaultCool_R_ = 80.0f;
		break;
	case 4:
		DefaultCool_W_ = 11.0f;
		DefaultCool_E_ = 15.0f;
		DefaultCool_R_ = 80.0f;
		break;
	case 5:
		DefaultCool_W_ = 10.0f;
		DefaultCool_E_ = 14.0f;
		DefaultCool_R_ = 80.0f;
		break;
	default:
		break;
	}
}

void Yuki::startCustomRStandBy()
{
	curAnimationName_ = "SkillR_loop";
	renderer_->ChangeFBXAnimation("SkillR_loop", true);

	collision_R->On();

	RandomSoundPlay("Yuki_PlaySkill1011500seq0_1_ko.wav", "Yuki_PlaySkill1011500seq0_2_ko.wav", "Yuki_PlaySkill1011500seq0_1_ko.wav");

	GameEngineSoundManager::GetInstance()->PlaySoundByName("Yuki_Skill04_Active.wav");
	PacketSoundPlay packet;
	packet.SetSound("Yuki_Skill04_Active.wav", transform_.GetWorldPosition());
	FT::SendPacket(packet);

	float4 wp = GetTransform()->GetWorldPosition();
	rEffect_->GetTransform()->SetLocalPosition(wp);
	rEffect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());
	rEffect_->PlayAwake();

	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("SkillR_awake");
	FT::SendPacket(pack);
}

void Yuki::updateCustomRStandBy(float _deltaTime)
{
	if (0.8f <= timer_R)
	{
		timer_R = 0.0f;
		customState_.ChangeState("CustomRSlash", true);
		return;
	}

	timer_R += _deltaTime;
}

void Yuki::endCustomRStandBy()
{

}

void Yuki::startCustomRSlash()
{
	curAnimationName_ = "SkillR_end";
	renderer_->ChangeFBXAnimation("SkillR_end", true);

	rEffect_->PlaySlash();

	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("SkillR_slash");
	FT::SendPacket(pack);
}

void Yuki::updateCustomRSlash(float _deltaTime)
{
	if (true == b_RHit_)
	{
		collision_R->Off();
	}

	if (0.5f <= timer_R)
	{
		timer_R = 0.0f;
		b_RHit_ = false;
		changeAnimationWait();
		mainState_.ChangeState("NormalState", true);
		normalState_.ChangeState("Watch", true);
		return;
	}

	if (false == b_RHit_)
	{
		collision_R->On();
		GameEngineSoundManager::GetInstance()->PlaySoundByName("Yuki_Skill04_Attack.wav");
		PacketSoundPlay packet;
		packet.SetSound("Yuki_Skill04_Attack.wav", transform_.GetWorldPosition());

		FT::SendPacket(packet);

		auto collisionList = collision_R->GetCollisionList(eCollisionGroup::Player);

		for (GameEngineCollision* col : collisionList)
		{
			GameEngineActor* actor = col->GetActor();
			IUnit* character = nullptr;
			if (nullptr != actor && actor != this)
			{
				character = dynamic_cast<IUnit*>(actor);

				if (nullptr != character)
				{
					character->Damage((stat_.AttackPower * 2.0f) + 250.0f, this);

					rUnitList_.push_back(character);
					character->Slow(1.0f, 0.7f);

					CharCrowdControlPacket ccPacket;
					ccPacket.SetTargetIndex(character->GetIndex());
					ccPacket.SetSlow(1.0f, 0.7f);


				}
			}
		}

		{
			auto collisionList = collision_R->GetCollisionList(eCollisionGroup::Monster);

			for (GameEngineCollision* col : collisionList)
			{
				GameEngineActor* actor = col->GetActor();
				IUnit* character = nullptr;
				if (nullptr != actor)
				{
					character = dynamic_cast<IUnit*>(actor);

					if (nullptr != character)
					{
						character->Damage((stat_.AttackPower * 2.0f) + 250.0f, this);

						rUnitList_.push_back(character);

						//CharCrowdControlPacket ccPacket;
						//ccPacket.SetTargetIndex(character->GetIndex());
						//ccPacket.SetSlow(1.0f, 0.7f);
					}
				}
			}
		}

		b_RHit_ = true;

	}


	timer_R += _deltaTime;
}

void Yuki::endCustomRSlash()
{




}

void Yuki::startCustomRAfterBurst()
{

}

void Yuki::updateCustomRAfterBurst(float _deltaTime)
{

}

void Yuki::endCustomRAfterBurst()
{

}