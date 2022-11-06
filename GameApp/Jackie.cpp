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
	: atkFlag_(false), timer_collision_Q(0.0f), timer_end_Q(0.0f), b_Qhit_(0), collision_Q(nullptr),
	timer_collision_E(0.0f), timer_end_E(0.0f), b_Ehit_(false), collision_E(nullptr),
	basicAttackEffect_(nullptr), skillQEffectRenderer_(nullptr), sawRenderer_(nullptr), axeRenderer_(nullptr),
	isW_(false), timer_W(0.0f), bSkillEPassable_(false), eStartPosition_(float4::ZERO), eLandingPosition_(float4::ZERO),
	debugX(-90.0f), debugY(0.0f), debugZ(0.0f), timer_R(0.0f)

{

}

Jackie::~Jackie() // default destructer 디폴트 소멸자
{

}

void Jackie::LoadResource()
{
	GameEngineDirectory dir;

	dir.MoveParent("FoxTeam");
	dir / "Resources" / "FBX" / "Character" / "Jackie";


	GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Jackie_run.fbx"));
	mesh->CreateRenderingBuffer();

	mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Weapon_Axe_01.fbx"));
	mesh->CreateRenderingBuffer();
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Weapon_Axe_01.fbx"));

	mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Weapon_Special_Jackie_01.fbx"));
	mesh->CreateRenderingBuffer();
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Weapon_Special_Jackie_01.fbx"));

	std::vector<GameEngineFile> allFile = dir.GetAllFile("UserAnimation");
	for (GameEngineFile& file : allFile)
	{
		GameEngineFBXAnimationManager::GetInst().LoadUser(file.GetFullPath());
	}

	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_run.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_wait.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_death.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_atk1.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_atk2.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_skillQ.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_skillW.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_skillE.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_weaponSkill.fbx"));
	//
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_R_wait.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_R_atk1.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_R_atk2.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_R_run.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_R_skillE.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_R_skillQ.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_R_skillW.fbx"));



	{
		GameEngineDirectory dir;
		dir.MoveParent("FoxTeam");
		dir / "Resources" / "Sound" / "Char" / "Jackie";
		
		std::vector<GameEngineFile> allFile = dir.GetAllFile("wav");
		for (GameEngineFile& file : allFile)
		{
			GameEngineSoundManager::GetInstance()->CreateSound(file.FileName(), file.GetFullPath());
		}
	}

	{
		GameEngineDirectory dir;

		dir.MoveParent("FoxTeam");
		dir / "Resources" / "Texture" / "Jackie";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("png");
		for (GameEngineFile& file : allFile)
		{
			GameEngineTextureManager::GetInst().Load(file.FileName(), file.GetFullPath());
		}
	}
}

void Jackie::ReleaseResource()
{
	{
		GameEngineDirectory dir;
		dir.MoveParent("FoxTeam");
		dir / "Resources" / "Sound" / "Char" / "Jackie";
		
		std::vector<GameEngineFile> allFile = dir.GetAllFile("wav");
		for (GameEngineFile& file : allFile)
		{
			GameEngineSoundManager::GetInstance()->ReleaseSound(file.FileName());
		}
	}

	GameEngineFBXMeshManager::GetInst().Delete("Jackie_run.fbx");
	GameEngineFBXMeshManager::GetInst().Delete("Weapon_Axe_01.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Jackie_run.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Jackie_wait.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Jackie_death.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Jackie_atk1.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Jackie_atk2.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Jackie_skillQ.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Jackie_skillW.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Jackie_skillE.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Jackie_weaponSkill.fbx");


	//GameEngineFBXAnimationManager::GetInst().Delete("Jackie_R_wait.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Jackie_R_atk1.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Jackie_R_atk2.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Jackie_R_run.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Jackie_R_skillE.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Jackie_R_skillQ.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Jackie_R_skillW.fbx");

	GameEngineFBXAnimationManager::GetInst().Delete("Weapon_Special_Jackie_01.fbx");

	{
		GameEngineDirectory dir;
		dir.MoveParent("FoxTeam");
		dir / "Resources" / "FBX" / "PJW";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("UserAnimation");
		for (GameEngineFile& file : allFile)
		{
			GameEngineFBXAnimationManager::GetInst().Delete(file.GetFileName());
		}
	}
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

	stat_.Cooltime_q = 9.0f;
	stat_.Cooltime_w = 19.0f;
	stat_.Cooltime_e = 24.0f;
	stat_.Cooltime_r = 70.0f;

	//GameEngineTexture* hitBase = GameEngineTextureManager::GetInst().Find("FX_BI_Hit_05.png");
	//hitBase->Cut(3, 3);
	if (false == GameEngineInput::GetInst().IsKey("X"))
	{
		GameEngineInput::GetInst().CreateKey("X", 'X');
	}
	if (false == GameEngineInput::GetInst().IsKey("Y"))
	{
		GameEngineInput::GetInst().CreateKey("Y", 'Y');
	}
	if (false == GameEngineInput::GetInst().IsKey("Z"))
	{
		GameEngineInput::GetInst().CreateKey("Z", 'Z');
	}
	if (false == GameEngineInput::GetInst().IsKey("B"))
	{
		GameEngineInput::GetInst().CreateKey("B", 'B');
	}
	if (false == GameEngineInput::GetInst().IsKey("N"))
	{
		GameEngineInput::GetInst().CreateKey("N", 'N');
	}
	if (false == GameEngineInput::GetInst().IsKey("M"))
	{
		GameEngineInput::GetInst().CreateKey("M", 'M');
	}
}

void Jackie::Update(float _deltaTime)
{
	Character::Update(_deltaTime);

	if (true == isPlayerDead_)
	{
		int a = 0;
		return;
	}

	if (true == isW_)
	{
		timer_W -= _deltaTime;

		if (0.0f >= timer_W)
		{
			timer_W = 0.0f;
			isW_ = false;
			stat_.MovementSpeed -= 100.0f;
		}
	}

	if (true == isR_)
	{
		timer_R -= _deltaTime;
		sawRenderer_->On();
		WeaponSilhouetteRenderer2_->On();
		axeRenderer_->Off();
		WeaponSilhouetteRenderer1_->Off();

		sawRenderer_->GetTransform()->GetTransformData().WorldWorld_* sawRenderer_->GetParentAffine();
		WeaponSilhouetteRenderer2_->GetTransform()->GetTransformData().WorldWorld_* sawRenderer_->GetParentAffine();

		float x = debugX;
		float y = debugY;
		float z = debugZ;

		if (0.0f >= timer_R)
		{
			timer_R = 0.0f;
			isR_ = false;
		}
	}

	if (false == isR_)
	{
		sawRenderer_->Off();
		WeaponSilhouetteRenderer1_->Off();
		axeRenderer_->On();
		axeRenderer_->GetTransform()->GetTransformData().WorldWorld_* sawRenderer_->GetParentAffine();
		WeaponSilhouetteRenderer2_->On();
		WeaponSilhouetteRenderer2_->GetTransform()->GetTransformData().WorldWorld_* sawRenderer_->GetParentAffine();
	}

	if ("Run" == curAnimationName_)
	{
		axeRenderer_->GetTransform()->SetLocalPosition({ 5.0f, 40.0f, 80.0f });
		axeRenderer_->GetTransform()->SetLocalRotationDegree({ -140.f, 0.0f, -30.0f });

		WeaponSilhouetteRenderer1_->GetTransform()->SetLocalPosition({ 5.0f, 40.0f, 80.0f });
		WeaponSilhouetteRenderer1_->GetTransform()->SetLocalRotationDegree({ -140.f, 0.0f, -30.0f });
	}
	else if ("Wait" == curAnimationName_)
	{
		axeRenderer_->GetTransform()->SetLocalPosition({ 45.0f, 120.0f, 58.0f });
		axeRenderer_->GetTransform()->SetLocalRotationDegree({ -164.0f, -317.0f, -82.0f });

		WeaponSilhouetteRenderer1_->GetTransform()->SetLocalPosition({ 45.0f, 120.0f, 58.0f });
		WeaponSilhouetteRenderer1_->GetTransform()->SetLocalRotationDegree({ -164.0f, -317.0f, -82.0f });
	}
	else
	{
		axeRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
		axeRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f, 0.0f });

		WeaponSilhouetteRenderer1_->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
		WeaponSilhouetteRenderer1_->GetTransform()->SetLocalRotationDegree({ -90.f, 0.0f });
	}

	GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
	if (nullptr != controlWindow)
	{
		controlWindow->AddText("WorldPos : " + std::to_string(transform_.GetWorldPosition().x) + " " + std::to_string(transform_.GetWorldPosition().z));
		controlWindow->AddText("CurLocation : " + std::to_string(stat_.curLocation));
	}
}

void Jackie::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>(static_cast<int>(ObjectRenderOrder::CHARACTER));
	renderer_->SetFBXMesh("Jackie_run.fbx", "TextureDeferredLightAni", true);

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	renderer_->CreateFBXAnimation("Run", "Jackie_run.UserAnimation", 0);
	renderer_->CreateFBXAnimation("Wait", "Jackie_wait.UserAnimation", 0);
	renderer_->CreateFBXAnimation("Death", "Jackie_death.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("Atk0", "Jackie_atk1.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillD", "Jackie_weaponSkill.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("Atk1", "Jackie_atk2.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillQ", "Jackie_skillQ.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillE", "Jackie_skillE.UserAnimation", 0, false);

	renderer_->CreateFBXAnimation("SkillW", "Jackie_skillW.UserAnimation", 0);

	renderer_->CreateFBXAnimation("SkillR_wait", "Jackie_R_wait.UserAnimation", 0);
	renderer_->CreateFBXAnimation("SkillR_run", "Jackie_R_run.UserAnimation", 0);
	renderer_->CreateFBXAnimation("R_Atk0", "Jackie_R_atk1.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("R_Atk1", "Jackie_R_atk2.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("R_SkillQ", "Jackie_R_skillQ.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("R_SkillW", "Jackie_R_skillW.UserAnimation", 0);
	renderer_->CreateFBXAnimation("R_SkillE", "Jackie_R_skillE.UserAnimation", 0, false);

	renderer_->ChangeFBXAnimation("Wait");

	renderer_->GetRenderSet(1).isRender = false;

	sawRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>(static_cast<int>(ObjectRenderOrder::WEAPON));
	sawRenderer_->SetFBXMesh("Weapon_Special_Jackie_01.fbx", "TextureDeferredLightAni");

	sawRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	sawRenderer_->GetTransform()->SetLocalScaling(100.f);
	sawRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f, 0.0f });
	
	sawRenderer_->SetParentBoneName(renderer_, "Bip001 R Finger2");

	sawRenderer_->CreateFBXAnimation("Idle", "Weapon_Special_Jackie_01.fbx");
	sawRenderer_->ChangeFBXAnimation("Idle");
	sawRenderer_->Off();

	axeRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>(static_cast<int>(ObjectRenderOrder::WEAPON));
	axeRenderer_->SetFBXMesh("Weapon_Axe_01.fbx", "TextureDeferredLightAni");

	axeRenderer_->SetParentBoneName(renderer_, "Bip001 L Finger1");
	axeRenderer_->GetTransform()->SetLocalScaling(100.f);
	axeRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f, 0.0f });

	axeRenderer_->CreateFBXAnimation("Idle", "Weapon_Axe_01.fbx");
	axeRenderer_->ChangeFBXAnimation("Idle");

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
		WeaponOutLineRenderer1_->SetBaseRenderer(axeRenderer_, "PreprocessingAni", true, false);

		OutLineData Weapon1OutLineData = WeaponOutLineRenderer1_->GetOutLineData();
		float4 Weapon1Thickness = float4(1.0f, 1.0f, 1.0f, 0.0f) * Weapon1OutLineData.LineThickness;
		float4 CalcWeapon1Scale = axeRenderer_->GetTransform()->GetLocalScaling() + Weapon1Thickness;
		WeaponOutLineRenderer1_->GetTransform()->SetLocalScaling(CalcWeapon1Scale);
		WeaponOutLineRenderer1_->GetTransform()->SetLocalRotationDegree(axeRenderer_->GetTransform()->GetLocalRotation());
		WeaponOutLineRenderer1_->GetTransform()->SetLocalPosition(axeRenderer_->GetTransform()->GetLocalPosition());
		WeaponOutLineRenderer1_->Off();

		// Weapon2
		WeaponOutLineRenderer2_ = CreateTransformComponent<GameEnginePreprocessingRenderer>();
		WeaponOutLineRenderer2_->SetBaseRenderer(sawRenderer_, "PreprocessingAni", true, false);

		OutLineData Weapon2OutLineData = WeaponOutLineRenderer1_->GetOutLineData();
		float4 Weapon2Thickness = float4(1.0f, 1.0f, 1.0f, 0.0f) * Weapon2OutLineData.LineThickness;
		float4 CalcWeapon2Scale = sawRenderer_->GetTransform()->GetLocalScaling() + Weapon2Thickness;
		WeaponOutLineRenderer2_->GetTransform()->SetLocalScaling(CalcWeapon2Scale);
		WeaponOutLineRenderer2_->GetTransform()->SetLocalRotationDegree(sawRenderer_->GetTransform()->GetLocalRotation());
		WeaponOutLineRenderer2_->GetTransform()->SetLocalPosition(sawRenderer_->GetTransform()->GetLocalPosition());
		WeaponOutLineRenderer2_->Off();
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
		WeaponSilhouetteRenderer1_->SetBaseRenderer(axeRenderer_, "PreprocessingAni", true);

		OutLineData Weapon1OutLineData = WeaponSilhouetteRenderer1_->GetOutLineData();
		float4 Weapon1Thickness = float4(1.0f, 1.0f, 1.0f, 0.0f) * Weapon1OutLineData.LineThickness;
		float4 CalcWeapon1Scale = axeRenderer_->GetTransform()->GetLocalScaling() + Weapon1Thickness;
		WeaponSilhouetteRenderer1_->GetTransform()->SetLocalScaling(CalcWeapon1Scale);
		WeaponSilhouetteRenderer1_->GetTransform()->SetLocalRotationDegree(axeRenderer_->GetTransform()->GetLocalRotation());
		WeaponSilhouetteRenderer1_->GetTransform()->SetLocalPosition(axeRenderer_->GetTransform()->GetLocalPosition());

		// Weapon2
		WeaponSilhouetteRenderer2_ = CreateTransformComponent<GameEnginePreprocessingRenderer>();
		WeaponSilhouetteRenderer2_->SetBaseRenderer(sawRenderer_, "PreprocessingAni", true);

		OutLineData Weapon2OutLineData = WeaponSilhouetteRenderer2_->GetOutLineData();
		float4 Weapon2Thickness = float4(1.0f, 1.0f, 1.0f, 0.0f) * Weapon2OutLineData.LineThickness;
		float4 CalcWeapon2Scale = sawRenderer_->GetTransform()->GetLocalScaling() + Weapon2Thickness;
		WeaponSilhouetteRenderer2_->GetTransform()->SetLocalScaling(CalcWeapon2Scale);
		WeaponSilhouetteRenderer2_->GetTransform()->SetLocalRotationDegree(sawRenderer_->GetTransform()->GetLocalRotation());
		WeaponSilhouetteRenderer2_->GetTransform()->SetLocalPosition(sawRenderer_->GetTransform()->GetLocalPosition());
	}
}

void Jackie::initJackieCollision()
{
	collision_Q = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_Q->GetTransform()->SetLocalPosition({ 0.f,0.f,300.f });
	collision_Q->GetTransform()->SetLocalScaling({ 450.0f, 10.0f, 230.0f });
	collision_Q->SetCollisionGroup(eCollisionGroup::PlayerAttack);
	collision_Q->SetCollisionType(CollisionType::OBBBox3D);
	collision_Q->Off();

	collision_E = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_E->SetCollisionType(CollisionType::Sphere3D);
	collision_E->SetCollisionGroup(eCollisionGroup::PlayerAttack);
	collision_E->GetTransform()->SetLocalScaling(250.0f);
	collision_E->Off();
}

void Jackie::initJackieCustomState()
{
	customState_.CreateState(MakeState(Jackie, SkillEBegin));
	customState_.CreateState(MakeState(Jackie, SkillEShot));
	customState_.CreateState(MakeState(Jackie, SkillEEnd));
}

void Jackie::initEffectRenderer()
{

	basicAttackEffect_ = GetLevel()->CreateActor<BasicAttackEffect>();
	basicAttackEffect_->GetAttackRenderer()->SetImage("FX_BI_SELine_10.png", "PointSmp");
	basicAttackEffect_->GetAttackRenderer()->GetTransform()->SetLocalPosition({ 0.0f,0.0f,0.0f });
	basicAttackEffect_->GetAttackRenderer()->GetTransform()->SetLocalRotationDegree({ 90.0f,0.0f,0.0f });
	basicAttackEffect_->GetAttackRenderer()->GetTransform()->SetLocalScaling(basicAttackEffect_->GetAttackRenderer()->GetCurrentTexture()->GetTextureSize() / 3);
	basicAttackEffect_->GetAttackRenderer()->CreateAnimation("FX_BI_SELine_10.png", "FX_BI_SELine_10", 0, 8, 0.03f, false);

	qEffect_ = GetLevel()->CreateActor<JackieQEffect>();
	qEffect_->SetParent(this);

	eEffect_ = GetLevel()->CreateActor<JackieEEffect>();
	eEffect_->SetParent(this);

}

void Jackie::changeAnimationRun()
{
	if (true == isR_)
	{
		if (true == isW_)
		{
			curAnimationName_ = "R_SkillW";
			renderer_->ChangeFBXAnimation("R_SkillW");
			return;
		}

		curAnimationName_ = "SkillR_run";
		renderer_->ChangeFBXAnimation("SkillR_run");
		return;
	}

	if (true == isW_)
	{
		curAnimationName_ = "SkillW";
		renderer_->ChangeFBXAnimation("SkillW");
		return;
	}

	curAnimationName_ = "Run";


	renderer_->ChangeFBXAnimation("Run");
}

void Jackie::changeAnimationWait()
{
	if (true == isR_)
	{
		curAnimationName_ = "SkillR_wait";
		renderer_->ChangeFBXAnimation("SkillR_wait");
		return;
	}

	curAnimationName_ = "Wait";
	renderer_->ChangeFBXAnimation("Wait");
}

void Jackie::changeAnimationBasicAttack()
{
	if (true == isR_)
	{
		if (false == atkFlag_)
		{
			curAnimationName_ = "R_Atk0";
			renderer_->ChangeFBXAnimation("R_Atk0", true);
			atkFlag_ = true;
			GameEngineSoundManager::GetInstance()->PlaySoundByName("jackie_ChainSaw_Attack_v1.wav");
			PacketSoundPlay packet;
			packet.SetSound("jackie_ChainSaw_Attack_v1.wav", transform_.GetWorldPosition());
			FT::SendPacket(packet);
		}
		else
		{
			curAnimationName_ = "R_Atk1";
			renderer_->ChangeFBXAnimation("R_Atk1", true);
			atkFlag_ = false;
			GameEngineSoundManager::GetInstance()->PlaySoundByName("jackie_ChainSaw_Attack_v2.wav");
			PacketSoundPlay packet;
			packet.SetSound("jackie_ChainSaw_Attack_v2.wav", transform_.GetWorldPosition());
			FT::SendPacket(packet);
		}
	}
	else
	{
		if (false == atkFlag_)
		{
			curAnimationName_ = "Atk0";
			renderer_->ChangeFBXAnimation("Atk0", true);
			atkFlag_ = true;
			GameEngineSoundManager::GetInstance()->PlaySoundByName("attackAxe_r1.wav");
			PacketSoundPlay packet;
			packet.SetSound("attackAxe_r1.wav", transform_.GetWorldPosition());
			FT::SendPacket(packet);
		}
		else
		{
			curAnimationName_ = "Atk1";
			renderer_->ChangeFBXAnimation("Atk1", true);
			atkFlag_ = false;
			GameEngineSoundManager::GetInstance()->PlaySoundByName("attackTwoHandSword_r2.wav");
			PacketSoundPlay packet;
			packet.SetSound("attackTwoHandSword_r2.wav", transform_.GetWorldPosition());
			FT::SendPacket(packet);
		}
	}
}

void Jackie::changeDeathAnimation()
{
	ChangeAnimation("Death", true);
}

void Jackie::onStartBasicAttacking(IUnit* _target)
{
	target_->Damage(stat_.AttackPower, this);

	if (true == isW_ && stat_.HP < stat_.HPMax)
	{
		float tmp = stat_.HPMax - stat_.HP;
		if (tmp < 50.0f)
		{
			stat_.HP += tmp;
		}
		else
		{
			stat_.HP += 50.0f;
		}
	
	}

	if (true == isR_)
	{
		GameEngineSoundManager::GetInstance()->PlaySoundByName("jackie_ChainSaw_Hit_v1.wav");
		PacketSoundPlay packet;
		packet.SetSound("jackie_ChainSaw_Hit_v1.wav", transform_.GetWorldPosition());
		FT::SendPacket(packet);
	}
	else
	{
		GameEngineSoundManager::GetInstance()->PlaySoundByName("hitSkillAxe_r1.wav");
		PacketSoundPlay packet;
		packet.SetSound("hitSkillAxe_r1.wav", transform_.GetWorldPosition());
		FT::SendPacket(packet);
	}

	float4 wp = target_->GetTransform()->GetWorldPosition();
	wp.y += 100.0f;
	basicAttackEffect_->GetTransform()->SetWorldPosition(wp);
	basicAttackEffect_->PlayAwake("FX_BI_SELine_10");

	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("BasicAttack");
	pack.SetVictimIndex(_target->GetIndex());
	FT::SendPacket(pack);
	// 여기 이펙트 패킷 하나

}

void Jackie::onUpdateBasicAttacking(IUnit* _target, float _deltaTime)
{

}

void Jackie::onStartQSkill()
{
	timer_collision_Q = 0.0f;
	timer_end_Q = 0.0f;
	b_Qhit_ = false;

	setRotationToMouse();

	curAnimationName_ = "SkillQ";
	renderer_->ChangeFBXAnimation("SkillQ", true);

	RandomSoundPlay("Jackie_PlaySkill1001200seq0_1_ko.wav", "Jackie_PlaySkill1001200seq0_2_ko.wav", "Jackie_PlaySkill1001200seq0_3_ko.wav");

	float4 wp = GetTransform()->GetWorldPosition();
	float4 wr = GetTransform()->GetLocalRotation();
	qEffect_->GetTransform()->SetLocalPosition(wp);
	qEffect_->GetTransform()->SetLocalRotationDegree(wr);
	qEffect_->PlayAwake();

	//float4 wp = GetTransform()->GetWorldPosition();
	//qEffect_->GetTransform()->SetLocalPosition(wp);
	//qEffect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());
	//qEffect_->PlayAwake();

	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("SkillQ");
	FT::SendPacket(pack);
	//skillQEffectRenderer_->On();
	//skillQEffectRenderer_->GetTransform()->SetLocalPosition({ 0.0f, this->GetTransform()->GetWorldPosition().y + 30.0f, collision_Q->GetTransform()->GetLocalPosition().z});
	//skillQEffectRenderer_->GetTransform()->SetLocalRotationDegree({ 90.0f, 0.0f, GetTransform()->GetWorldRotation().z - 90.0f });
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

	if (0.8f <= timer_end_Q)
	{
		// 모든 카운터 초기화
		timer_collision_Q = 0.0f;
		timer_end_Q = 0.0f;
		b_Qhit_ = 0;
		changeAnimationWait();
		//skillQEffectRenderer_->Off();
		mainState_.ChangeState("NormalState", true);
		normalState_.ChangeState("Watch", true);
		return;
	}

	if (0 == b_Qhit_)
	{
		collision_Q->On();
		GameEngineSoundManager::GetInstance()->PlaySoundByName("wskill_Axe_attack.wav");
		PacketSoundPlay packet;
		packet.SetSound("wskill_Axe_attack.wav", transform_.GetWorldPosition());
		FT::SendPacket(packet);

		//CharEffectPacket pack;
		//pack.SetTargetIndex(myIndex_);
		//pack.SetAnimationName("SkillQ");
		//FT::SendPacket(pack);

		{
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
						if (true == isR_)
						{
							GameEngineSoundManager::GetInstance()->PlaySoundByName("jackie_ChainSaw_Hit_v1.wav");
							PacketSoundPlay packet;
							packet.SetSound("jackie_ChainSaw_Hit_v1.wav", transform_.GetWorldPosition());
							FT::SendPacket(packet);
						}
						else
						{
							GameEngineSoundManager::GetInstance()->PlaySoundByName("hitSkillAxe_r1.wav");
							PacketSoundPlay packet;
							packet.SetSound("hitSkillAxe_r1.wav", transform_.GetWorldPosition());
							FT::SendPacket(packet);
						}


						character->Damage(150.0f, this);
					}
				}
			}
		}

		{
			auto collisionList = collision_Q->GetCollisionList(eCollisionGroup::Monster);

			for (GameEngineCollision* col : collisionList)
			{
				GameEngineActor* actor = col->GetActor();
				IUnit* character = nullptr;
				if (nullptr != actor)
				{
					character = dynamic_cast<IUnit*>(actor);

					if (nullptr != character)
					{
						if (true == isR_)
						{
							GameEngineSoundManager::GetInstance()->PlaySoundByName("jackie_ChainSaw_Hit_v1.wav");
							PacketSoundPlay packet;
							packet.SetSound("jackie_ChainSaw_Hit_v1.wav", transform_.GetWorldPosition());
							FT::SendPacket(packet);
						}
						else
						{
							GameEngineSoundManager::GetInstance()->PlaySoundByName("hitSkillAxe_r1.wav");
							PacketSoundPlay packet;
							packet.SetSound("hitSkillAxe_r1.wav", transform_.GetWorldPosition());
							FT::SendPacket(packet);
						}


						character->Damage(150.0f, this);
					}
				}
			}
		}

		b_Qhit_++;
	}

	if (0.2f <= timer_collision_Q && 1 == b_Qhit_)
	{
		collision_Q->On();

		GameEngineSoundManager::GetInstance()->PlaySoundByName("wskill_Axe_attack.wav");
		PacketSoundPlay packet;
		packet.SetSound("wskill_Axe_attack.wav", transform_.GetWorldPosition());
		FT::SendPacket(packet);


		//float4 wp = GetTransform()->GetWorldPosition();
		//qEffect_->GetTransform()->SetLocalPosition(wp);
		//qEffect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());
		//qEffect_->PlayAwake();

		//CharEffectPacket pack;
		//pack.SetTargetIndex(myIndex_);
		//pack.SetAnimationName("SkillQ");
		//FT::SendPacket(pack);

		{
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
						if (true == isR_)
						{
							GameEngineSoundManager::GetInstance()->PlaySoundByName("jackie_ChainSaw_Hit_v1.wav");
							PacketSoundPlay packet;
							packet.SetSound("jackie_ChainSaw_Hit_v1.wav", transform_.GetWorldPosition());
							FT::SendPacket(packet);
						}
						else
						{
							GameEngineSoundManager::GetInstance()->PlaySoundByName("hitSkillAxe_r1.wav");
							PacketSoundPlay packet;
							packet.SetSound("hitSkillAxe_r1.wav", transform_.GetWorldPosition());
							FT::SendPacket(packet);
						}
						character->Damage(150.0f, this);
					}
				}
			}
		}
		{
			auto collisionList = collision_Q->GetCollisionList(eCollisionGroup::Monster);

			for (GameEngineCollision* col : collisionList)
			{
				GameEngineActor* actor = col->GetActor();
				IUnit* character = nullptr;
				if (nullptr != actor)
				{
					character = dynamic_cast<IUnit*>(actor);

					if (nullptr != character)
					{
						if (true == isR_)
						{
							GameEngineSoundManager::GetInstance()->PlaySoundByName("jackie_ChainSaw_Hit_v1.wav");
							PacketSoundPlay packet;
							packet.SetSound("jackie_ChainSaw_Hit_v1.wav", transform_.GetWorldPosition());
							FT::SendPacket(packet);
						}
						else
						{
							GameEngineSoundManager::GetInstance()->PlaySoundByName("hitSkillAxe_r1.wav");
							PacketSoundPlay packet;
							packet.SetSound("hitSkillAxe_r1.wav", transform_.GetWorldPosition());
							FT::SendPacket(packet);
						}
						character->Damage(150.0f, this);
					}
				}
			}
		}


		b_Qhit_++;
	}
}

void Jackie::onStartWSkill()
{
	isW_ = true;
	timer_W = 3.0f;
	RandomSoundPlay("Jackie_PlaySkill1001300seq0_1_ko.wav", "Jackie_PlaySkill1001300seq0_2_ko.wav", "Jackie_PlaySkill1001300seq0_3_ko.wav");


	GameEngineSoundManager::GetInstance()->PlaySoundByName("jackie_Skill02_Activation.wav");
	PacketSoundPlay packet;
	packet.SetSound("jackie_Skill02_Activation.wav", transform_.GetWorldPosition());
	FT::SendPacket(packet);
}

void Jackie::onUpdateWSkill(float _deltaTime)
{
	changeAnimationWait();
	mainState_.ChangeState("NormalState", true);
	normalState_.ChangeState("Watch", true);
	stat_.MovementSpeed += 100.0f;
}

void Jackie::onStartESkill()
{


}

void Jackie::onUpdateESkill(float _deltaTime)
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

void Jackie::onStartRSkill()
{
	isR_ = true;

	// timer_R = 12.0f;
	timer_R = 12.0f;

	axeRenderer_->Off();
	sawRenderer_->On();

	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("SkillR");
	FT::SendPacket(pack);

	RandomSoundPlay("Jackie_PlaySkill1001500seq0_1_ko.wav", "Jackie_PlaySkill1001500seq0_2_ko.wav", "Jackie_PlaySkill1001500seq0_3_ko.wav");

	GameEngineSoundManager::GetInstance()->PlaySoundByName("jackie_Skill04_Activation_v1.wav");
	PacketSoundPlay packet;
	packet.SetSound("jackie_Skill04_Activation_v1.wav", transform_.GetWorldPosition());
	FT::SendPacket(packet);
}

void Jackie::onUpdateRSkill(float _deltaTime)
{
	changeAnimationWait();
	mainState_.ChangeState("NormalState", true);
	normalState_.ChangeState("Watch", true);
}

void Jackie::onStartDSkill()
{
}

void Jackie::onUpdateDSkill(float _deltaTime)
{
	changeAnimationWait();
	mainState_.ChangeState("NormalState", true);
	normalState_.ChangeState("Watch", true);
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
	customState_.Update(_deltaTime);
}

void Jackie::onPlayEffect(const std::string& _effectName, IUnit* _victim)
{
	if ("BasicAttack" == _effectName)
	{
		if (_victim != nullptr)
		{
			float4 wp = _victim->GetTransform()->GetWorldPosition();
			wp.y += 100.0f;
			basicAttackEffect_->GetTransform()->SetWorldPosition(wp);
		}

		basicAttackEffect_->PlayAwake("FX_BI_SELine_10");
		return;
	}

	if ("SkillQ" == _effectName)
	{
		float4 wp = GetTransform()->GetWorldPosition();
		float4 wr = GetTransform()->GetLocalRotation();
		qEffect_->GetTransform()->SetLocalPosition(wp);
		qEffect_->GetTransform()->SetLocalRotationDegree(wr);
		qEffect_->PlayAwake();
		return;
	}

	if ("SkillE" == _effectName)
	{
		float4 wp = GetTransform()->GetWorldPosition();
		float4 wr = GetTransform()->GetLocalRotation();
		eEffect_->GetTransform()->SetLocalPosition(wp);
		eEffect_->GetTransform()->SetLocalRotationDegree(wr);
		eEffect_->PlayAwake();
		return;
	}

	if ("SkillR" == _effectName)
	{
		timer_R = 12.0f;
		isR_ = true;
		return;
	}

}

void Jackie::onEffectTransformCheck(float _deltaTime)
{
}

void Jackie::startSkillEBegin()
{
	b_Ehit_ = false;
	float height = 0.0f;

	eStartPosition_ = GetTransform()->GetWorldPosition();
	eLandingPosition_ = GetTransform()->GetWorldPosition();
	eLandingPosition_ += GetTransform()->GetWorldForwardVector() * 500.f;

	float4 landingPosition = eLandingPosition_;
	landingPosition.y += FT::Map::MAX_HEIGHT;

	bSkillEPassable_ = currentMap_->GetNavMesh()->CheckIntersects(landingPosition, float4::DOWN, height);


	RandomSoundPlay("Jackie_PlaySkill1001400seq0_1_ko.wav", "Jackie_PlaySkill1001400seq0_2_ko.wav", "Jackie_PlaySkill1001400seq0_3_ko.wav");


	if (true == isR_)
	{
		curAnimationName_ = "R_SkillE";
		ChangeAnimation("R_SkillE", true);
	}
	else
	{
		curAnimationName_ = "SkillE";
		ChangeAnimation("SkillE", true);
	}

	GameEngineSoundManager::GetInstance()->PlaySoundByName("jackie_Skill03_Jumping.wav");
	PacketSoundPlay packet;
	packet.SetSound("jackie_Skill03_Jumping.wav", transform_.GetWorldPosition());
	FT::SendPacket(packet);

	skillETime_ = 0.0f;
}

void Jackie::updateSkillEBegin(float _deltaTime)
{
	skillETime_ += _deltaTime;
	if (customState_.GetCurrentState()->Time_ > 0.2f)
	{
		customState_ << "SkillEShot";
	}

	if (bSkillEPassable_)
	{
		transform_.SetWorldPosition(float4::Lerp(eStartPosition_, eLandingPosition_, skillETime_ / 0.5f));
	}
}

void Jackie::startSkillEShot()
{
}

void Jackie::updateSkillEShot(float _deltaTime)
{
	if (customState_.GetCurrentState()->Time_ > 0.2f)
	{
		eEffect_->GetTransform()->SetLocalPosition(eLandingPosition_);
		eEffect_->On();
		eEffect_->PlayAwake();

		CharEffectPacket pack;
		pack.SetTargetIndex(myIndex_);
		pack.SetAnimationName("SkillE");
		FT::SendPacket(pack);

		customState_ << "SkillEEnd";
	}

	skillETime_ += _deltaTime;
	if (bSkillEPassable_)
	{

		if (bSkillEPassable_)
		{
			transform_.SetWorldPosition(float4::Lerp(eStartPosition_, eLandingPosition_, skillETime_ / 0.5f));
		}
	}
}

void Jackie::startSkillEEnd()
{
	collision_E->On();

	GameEngineSoundManager::GetInstance()->PlaySoundByName("jackie_Skill03_Bump.wav");
	PacketSoundPlay packet;
	packet.SetSound("jackie_Skill03_Bump.wav", transform_.GetWorldPosition());
	FT::SendPacket(packet);

}

void Jackie::updateSkillEEnd(float _deltaTime)
{


	if (false == b_Ehit_)
	{
		auto collisionList = collision_E->GetCollisionList(eCollisionGroup::Player);

		for (GameEngineCollision* col : collisionList)
		{
			GameEngineActor* actor = col->GetActor();
			Character* character = nullptr;
			if (nullptr != actor && actor != this)
			{
				character = dynamic_cast<Character*>(actor);

				if (nullptr != character)
				{
					character->Damage(120.0f, this);
				}
			}
		}

		b_Ehit_ = true;
	}

	if (false == b_Ehit_)
	{
		auto collisionList = collision_E->GetCollisionList(eCollisionGroup::Monster);

		for (GameEngineCollision* col : collisionList)
		{
			GameEngineActor* actor = col->GetActor();
			IUnit* character = nullptr;
			if (nullptr != actor)
			{
				character = dynamic_cast<IUnit*>(actor);

				if (nullptr != character)
				{
					character->Damage(120.0f, this);
				}
			}
		}

		b_Ehit_ = true;
	}


	if (renderer_->IsCurrentAnimationEnd() || skillETime_ > 0.6f)
	{
		destination_ = transform_.GetWorldPosition();
		destinations_.clear();
		changeAnimationWait();
		b_Ehit_ = false;
		mainState_ << "NormalState";
		return;
	}


	skillETime_ += _deltaTime;
	if (bSkillEPassable_)
	{
		if (bSkillEPassable_)
		{
			transform_.SetWorldPosition(float4::Lerp(eStartPosition_, eLandingPosition_, skillETime_ / 0.5f));
		}
	}
}

