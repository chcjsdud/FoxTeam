#include "PreCompile.h"
#include "Hyunwoo.h"
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
#include "HyunwooQEffect.h"
#include "HyunwooEffect.h"
#include "HyunwooREffect.h"

Hyunwoo::Hyunwoo()
	: timer_collision_Q(0.0f), timer_end_Q(0.0f), collision_Q(nullptr), b_Qhit_(false), timer_Dash_E(0.0f), b_Ehit_(false), collision_E(nullptr), atkFlag_(false),
	  b_Rhit_(false), collision_R(nullptr), collisionRRate_(0.0f), b_Dhit_(false), basicAttackEffectRenderer_(nullptr), QGroundCrackEffectRenderer_(nullptr), qEffect_(nullptr),
	rEffect_(nullptr)
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
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_weaponskill.fbx"));

	{
		GameEngineDirectory dir;
		dir.MoveParent("FoxTeam");
		dir / "Resources" / "Sound" / "Char" / "Hyunwoo";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("wav");
		for (GameEngineFile& file : allFile)
		{
			GameEngineSoundManager::GetInstance()->CreateSound(file.FileName(), file.GetFullPath());
		}
	}


	{
		//GameEngineDirectory dir;
		//dir.MoveParent("FoxTeam");
		//dir / "Resources" / "Texture" / "PJW";
		//
		//std::vector<GameEngineFile> allFile = dir.GetAllFile("png");
		//for (GameEngineFile& file : allFile)
		//{
		//	GameEngineTextureManager::GetInst().Load(file.GetFileName(), file.GetFullPath());
		//}

		GameEngineTexture* hitBase = GameEngineTextureManager::GetInst().Find("FX_BI_Hit_061.png");
		hitBase->Cut(2, 2);
		GameEngineTexture* rearBase = GameEngineTextureManager::GetInst().Find("FX_BI_WindDust_01SE1.png");
		rearBase->Cut(3, 8);


	}
}

void Hyunwoo::ReleaseResource()
{
	{
		GameEngineDirectory dir;
		dir.MoveParent("FoxTeam");
		dir / "Resources" / "Sound" / "Char" / "Hyunwoo";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("wav");
		for (GameEngineFile& file : allFile)
		{
			GameEngineSoundManager::GetInstance()->ReleaseSound(file.FileName());
		}
	}

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
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_weaponskill.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_weaponskill.fbx");
}

void Hyunwoo::Start()
{
	Character::Start();
	initHyunwooCollision();
	initHyunwooCustomState();
	initEffectRenderer();

	stat_.HPMax = 820.0f;
	stat_.HP = 820.0f;
	stat_.SPMax = 350.0f;
	stat_.SP = 350.0f;
	stat_.Defence = 28.0f;
	stat_.AttackPower = 40.0f;
	stat_.HPRegeneration = 1.8f;
	stat_.SPRegeneration = 1.8f;

	stat_.AttackSpeed = 0.86f;
	stat_.AttackStartTime = 0.15f;
	stat_.AttackEndTime = 0.4f;
	stat_.MovementSpeed = 355.0f;
	stat_.AttackRange = 280.f;

	GameEngineTexture* hitBase = GameEngineTextureManager::GetInst().Find("FX_BI_Hit_05.png");
	hitBase->Cut(3, 3);
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
	renderer_->CreateFBXAnimation("SkillD", "hyunwoo_weaponskill.fbx", 0, false);
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

	collision_R = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_R->GetTransform()->SetLocalPosition({ 0.f,0.f,150.f });
	collision_R->GetTransform()->SetLocalScaling({200.0f, 10.0f, 100.0f});
	collision_R->SetCollisionGroup(eCollisionGroup::PlayerAttack);
	collision_R->SetCollisionType(CollisionType::OBBBox3D);
	collision_R->Off();

}

void Hyunwoo::initHyunwooCustomState()
{
	customState_.CreateState(MakeStateWithEnd(Hyunwoo, CustomRSkill));

	customState_ << "CustomRSkill";
}

void Hyunwoo::initEffectRenderer()
{
	basicAttackEffectRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());

	basicAttackEffectRenderer_->SetImage("FX_BI_Hit_061.png");
	basicAttackEffectRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 100.0f, stat_.AttackRange - 180.f });
	basicAttackEffectRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	basicAttackEffectRenderer_->GetTransform()->SetLocalScaling(basicAttackEffectRenderer_->GetCurrentTexture()->GetTextureSize() / 3);
	basicAttackEffectRenderer_->CreateAnimation("FX_BI_Hit_061.png", "FX_BI_Hit_061", 0, 3, 0.04f, false);
	basicAttackEffectRenderer_->Off();


	QGroundCrackEffectRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	QGroundCrackEffectRenderer_->SetImage("FX_BI_GroundBomb_01.png");
	QGroundCrackEffectRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 10.0f, 100.0f });
	QGroundCrackEffectRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	QGroundCrackEffectRenderer_->GetTransform()->SetLocalScaling(QGroundCrackEffectRenderer_->GetCurrentTexture()->GetTextureSize() / 3);
	QGroundCrackEffectRenderer_->Off();

	rearEffectRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());

	rearEffectRenderer_->SetImage("FX_BI_WindDust_01SE1.png", "PointSmp");
	rearEffectRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 100.0f, -180.0f });
	rearEffectRenderer_->GetTransform()->SetLocalRotationDegree({ 0.f,90.f,0.f });
	rearEffectRenderer_->GetTransform()->SetLocalScaling(rearEffectRenderer_->GetCurrentTexture()->GetTextureSize() / 3);
	rearEffectRenderer_->CreateAnimation("FX_BI_WindDust_01SE1.png", "FX_BI_WindDust_01SE", 0, 23, 0.02f, false);
	rearEffectRenderer_->Off();


	qEffect_ = GetLevel()->CreateActor<HyunwooQEffect>();
	qEffect_->SetParent(this);

	rEffect_ = GetLevel()->CreateActor<HyunwooREffect>();
	rEffect_->SetParent(this);
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

void Hyunwoo::onStartQSkill()
{
	timer_collision_Q = 0.0f;
	timer_end_Q = 0.0f;
	b_Qhit_ = false;

	curAnimationName_ = "SkillQ";
	renderer_->ChangeFBXAnimation("SkillQ", true);

	collision_Q->On();

	rearEffectRenderer_->On();
	rearEffectRenderer_->SetChangeAnimation("FX_BI_WindDust_01SE", true);
	rearEffectRenderer_->AnimationPlay();
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
		GameEngineSoundManager::GetInstance()->PlaySoundByName("hyunwoo_Skill01_Hit.wav");
		PacketSoundPlay packet;
		packet.SetSound("hyunwoo_Skill01_Hit.wav", transform_.GetWorldPosition());

		FT::SendPacket(packet);

		float4 wp = GetTransform()->GetWorldPosition();
		qEffect_->GetTransform()->SetLocalPosition(wp);
		qEffect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());
		qEffect_->PlayAwake();

		CharEffectPacket pack;
		pack.SetTargetIndex(myIndex_);
		pack.SetAnimationName("SkillQ");
		FT::SendPacket(pack);

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
					character->Damage(300.0f);
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
	GameEngineSoundManager::GetInstance()->PlaySoundByName("hyunwoo_Skill02_Activation.wav");
	PacketSoundPlay packet;
	packet.SetSound("hyunwoo_Skill02_Activation.wav", transform_.GetWorldPosition());

	FT::SendPacket(packet);
}

void Hyunwoo::onUpdateWSkill(float _deltaTime)
{
	// W ��ų�� �Ͻ����� ���� ������
	// ��� ���� ����� �鿪�� �Ͻ������� �ο��ϴ� ������Ʈ�Դϴ�.



	changeAnimationWait();
	mainState_.ChangeState("NormalState", true);
	normalState_.ChangeState("Watch", true);
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

	GameEngineSoundManager::GetInstance()->PlaySoundByName("hyunwoo_Skill03_Slide.wav");
	PacketSoundPlay packet;
	packet.SetSound("hyunwoo_Skill03_Slide.wav", transform_.GetWorldPosition());

	FT::SendPacket(packet);

	rearEffectRenderer_->On();
	rearEffectRenderer_->SetChangeAnimation("FX_BI_WindDust_01SE", true);
	rearEffectRenderer_->AnimationPlay();

	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("SkillE");
	FT::SendPacket(pack);
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
						CharCrowdControlPacket ccPacket;
						ccPacket.SetTargetIndex(character->GetIndex());
						ccPacket.SetWallSlam(0.2f, direction_ * 3000.f, 1.0f);

						GameEngineSoundManager::GetInstance()->PlaySoundByName("hyunwoo_Skill03_Hit.wav");
						PacketSoundPlay packet;
						packet.SetSound("hyunwoo_Skill03_Hit.wav", transform_.GetWorldPosition());

						FT::SendPacket(packet);

						if (true == GameServer::GetInstance()->IsOpened())
						{
							GameServer::GetInstance()->Send(&ccPacket);
							//GameServer::GetInstance()->Send(&packet);
						}
						else if (true == GameClient::GetInstance()->IsConnected())
						{
							GameClient::GetInstance()->Send(&ccPacket);
							//GameClient::GetInstance()->Send(&packet);
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

	// ������ ����
	// ������ ���� ���

	// ���ݷ� ���� �̼� ���ݼӵ�
	if (true == rearEffectRenderer_->IsCurAnimationEnd())
	{
		rearEffectRenderer_->Off();
	}
}

void Hyunwoo::onStartRSkill()
{
	mainState_.ChangeState("CustomState", true);
	customState_.ChangeState("CustomRSkill", true);

}

void Hyunwoo::onUpdateRSkill(float _deltaTime)
{
}

void Hyunwoo::onStartDSkill()
{
	// ���⼭ ���콺�� ����� Ÿ���� ���θ� ã�� ��.

	if (mouse_ == nullptr)
	{
		mainState_.ChangeState("NormalState", true);
		normalState_.ChangeState("Watch", true);
		return;
	}
}

void Hyunwoo::onUpdateDSkill(float _deltaTime)
{
	bool result = false;
	if (false == b_Dhit_)
	{
		GameEngineCollision* rayCol = mouse_->GetRayCollision();


		float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);

		IUnit* otherCharacter = getMousePickedCharacter();
		target_ = otherCharacter;

		if (nullptr != otherCharacter && otherCharacter != this && false == b_Dhit_)
		{
			// ���� ó��
			float4 targetPosition = target_->GetTransform()->GetWorldPosition();
			float4 playerPosition = transform_.GetWorldPosition();
			float distance = float4::Calc_Len3D(playerPosition, targetPosition);

			if (distance > stat_.AttackRange + 90.f)
			{	// ��Ÿ� �ʹ� �� ��
				mainState_.ChangeState("NormalState", true);
				normalState_.ChangeState("Chase", true);
				return;
			}

			curAnimationName_ = "SkillD";
			renderer_->ChangeFBXAnimation("SkillD", true);
			otherCharacter->Damage(150.0f);
			// 

			b_Dhit_ = true;
			normalState_ << "Chase";
			return;
		}
		else
		{
			target_ = nullptr;
			result = currentMap_->GetNavMesh()->GetIntersectionPointFromMouseRay(destination_);

			changeAnimationWait();
			mainState_.ChangeState("NormalState", true);
			normalState_.ChangeState("Watch", true);

			return;
		}

	}

	if ("SkillD" == curAnimationName_ && true == renderer_->IsCurrentAnimationEnd())
	{
		b_Dhit_ = false;

		changeAnimationWait();
		mainState_.ChangeState("NormalState", true);
		normalState_.ChangeState("Watch", true);
	}


}

void Hyunwoo::onStartDeath()
{
	curAnimationName_ = "Death";
	renderer_->ChangeFBXAnimation("Death", true);
}

void Hyunwoo::onUpdateDeath(float _deltaTime)
{

}

void Hyunwoo::onUpdateCustomState(float _deltaTime)
{
	customState_.Update(_deltaTime);
}



void Hyunwoo::onPlayEffect(const std::string& _effectName)
{
	// ���濡�Լ� ���� �� ����Ʈ ��Ŷ�� execute �Ǹ�
	// ���������� �� ����Ʈ���� �ش� Ÿ�� �ε��� ĳ������ ���ڷ� ���� �� �Լ��� ��� �ɴϴ�.
	
	if ("BasicAttack" == _effectName)
	{
		basicAttackEffectRenderer_->On();
		basicAttackEffectRenderer_->SetChangeAnimation("FX_BI_Hit_061", true);
		basicAttackEffectRenderer_->AnimationPlay();
		return;
	}

	if ("SkillQ" == _effectName)
	{
		float4 wp = GetTransform()->GetWorldPosition();
		qEffect_->GetTransform()->SetLocalPosition(wp);
		qEffect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());
		qEffect_->PlayAwake();
		return;
	}

	if ("SkillE" == _effectName)
	{
		rearEffectRenderer_->On();
		rearEffectRenderer_->SetChangeAnimation("FX_BI_WindDust_01SE", true);
		rearEffectRenderer_->AnimationPlay();
		return;
	}

	if ("SkillR_awaken" == _effectName)
	{
		float4 wp = GetTransform()->GetWorldPosition();
		rEffect_->GetTransform()->SetLocalPosition(wp);
		rEffect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());
		rEffect_->PlayAwake();


		return;
	}

	if ("SkillR_explode" == _effectName)
	{
		float4 wp = GetTransform()->GetWorldPosition();
		rEffect_->GetTransform()->SetLocalPosition(wp);
		rEffect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());
		rEffect_->PlayExplode();
		return;
	}
}

void Hyunwoo::startCustomRSkill()
{
	curAnimationName_ = "SkillR_start";
	renderer_->ChangeFBXAnimation("SkillR_start", true);
	collision_R->On();
	GameEngineSoundManager::GetInstance()->PlaySoundByName("hyunwoo_Skill04_Charging.wav");
	PacketSoundPlay packet;
	packet.SetSound("hyunwoo_Skill04_Charging.wav", transform_.GetWorldPosition());
	FT::SendPacket(packet);

	float4 wp = GetTransform()->GetWorldPosition();
	rEffect_->GetTransform()->SetLocalPosition(wp);
	rEffect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());
	rEffect_->PlayAwake();

	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("SkillR_awaken");
	FT::SendPacket(pack);
}

void Hyunwoo::updateCustomRSkill(float _deltaTime)
{
	if (true == collision_R->IsUpdate())
	{
		GetLevel()->PushDebugRender(collision_R->GetTransform(), CollisionType::OBBBox3D, float4::RED);
	}

	if (curAnimationName_ == "SkillR_start" && renderer_->IsCurrentAnimationEnd())
	{
		curAnimationName_ = "SkillR_loop";
		renderer_->ChangeFBXAnimation("SkillR_loop", true);
	}

	// ���⿡ ��¡ ����� ���� & �浹ü ũ�� ���� �ڵ� �� ����
	if (false == b_Rhit_)
	{
		collisionRRate_ += _deltaTime;
		collision_R->GetTransform()->SetLocalPosition({ 0.f,0.f,150.f + collisionRRate_ * 50.0f });
		collision_R->GetTransform()->SetLocalScaling({ 350.0f, 10.0f, 100.0f + collisionRRate_ * 100.0f });
	}

	if (true == GameEngineInput::GetInst().Down("R"))
	{
		rEffect_->PlayExplode();
		CharEffectPacket pack;
		pack.SetTargetIndex(myIndex_);
		pack.SetAnimationName("SkillR_explode");
		FT::SendPacket(pack);

		GameEngineSoundManager::GetInstance()->StopSound();

		GameEngineSoundManager::GetInstance()->PlaySoundByName("hyunwoo_Skill04_Hit.wav");

		PacketSoundPlay packet;
		packet.SetSound("hyunwoo_Skill04_Hit.wav", transform_.GetWorldPosition());
		FT::SendPacket(packet);

		if (false == b_Rhit_)
		{
			// ���⼭ �ǰ� �浹 ������ ����
			auto collisionList = collision_R->GetCollisionList(eCollisionGroup::Player);

			for (GameEngineCollision* col : collisionList)
			{
				GameEngineActor* actor = col->GetActor();
				Character* character = nullptr;
				if (nullptr != actor && actor != this)
				{
					character = dynamic_cast<Character*>(actor);

					if (nullptr != character)
					{
						

						character->Damage(300.0f);

					}
				}
			}
			b_Rhit_ = true;


		}

		if (curAnimationName_ != "SkillR_end")
		{
			curAnimationName_ = "SkillR_end";
			renderer_->ChangeFBXAnimation("SkillR_end", true);

		}
	}

	if (curAnimationName_ == "SkillR_end" && true == renderer_->IsCurrentAnimationEnd())
	{
		b_Rhit_ = false;
		changeAnimationWait();
		mainState_.ChangeState("NormalState", true);
		normalState_.ChangeState("Watch", true);
	}
	/*���ط�: 80/120/160(+���ݷ��� 75%)(+��ų ������ 85%) ~ 480/720/960(+���ݷ��� 225%)(+��ų ������ 255%)
�����Ÿ�: 3m ~ 4.2m
���׹̳� �Ҹ�: 150
���� �ð�: 1.2 ~ 3��
��ٿ�: 65/55/50��*/
}

void Hyunwoo::endCustomRSkill()
{
	b_Rhit_ = false;
	collisionRRate_ = 0.0f;
	collision_R->Off();
	collision_R->GetTransform()->SetLocalScaling({ 350.0f, 10.0f, 100.0f + collisionRRate_ });
}

void Hyunwoo::onStartBasicAttacking(IUnit* _target)
{
	target_->Damage(stat_.AttackPower);

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
	// ���� ����Ʈ ��Ŷ �ϳ�
}

void Hyunwoo::onUpdateBasicAttacking(IUnit* _target, float _deltaTime)
{
	if (true == basicAttackEffectRenderer_->IsCurAnimationEnd())
	{
		basicAttackEffectRenderer_->Off();
	}

}
