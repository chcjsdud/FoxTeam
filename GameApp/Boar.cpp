#include "PreCompile.h"
#include "Boar.h"

#include <GameEngine/GameEngineCollision.h>

#include "GameServer.h"
#include "GameClient.h"
#include "CharCrowdControlPacket.h"

#include "LumiaLevel.h"
#include "Character.h"

bool Boar::ResourceLoadFlag = false;

void Boar::InitalizeStateInfo()
{
	// �⺻����
	Type_ = MonsterType::BOAR;											// ����Ÿ��

	//====================================== ������ �ִ�ġ(����)
	StateInfo_.LevelMin_ = 2;											// ���� ����(������ �ش� �����ͷ� �����ʱ�ȭ)
	StateInfo_.LevelMax_ = 12;											// �ִ� ����
	StateInfo_.HomingInstinctValueMax_ = 100.0f;						// �ִ� �ͼҺ��� ��ġ
	StateInfo_.RegenTimeMax_ = 195.0f;									// ����Ÿ��
	StateInfo_.SkillCoolDownMax_ = 7.0f;								// �ִ� ��ų��Ÿ��

	//====================================== ������ ������(����)
	StateInfo_.OffencePowerIncrement_ = 23;								// ������ ���ݷ� ������
	StateInfo_.DefenseIncrement_ = 7;									// ������ ���� ������
	StateInfo_.MoveSpeedIncrement_ = 0.05f;								// ������ �̵��ӵ� ������
	StateInfo_.HPIncrement_ = 167.0f;									// ������ ������ ü�� ������

	//====================================== �⺻����(����)
	StateInfo_.NestPosition_ = float4::ZERO;							// ������ġ(= ������ġ)
	StateInfo_.AttackSpeed_ = 0.5f;										// ���ݼӵ�
	StateInfo_.AttackRange_ = 100.0f;									// ���� �þ߰Ÿ�
	StateInfo_.DetectRange_ = 300.0f;									// ���� �þ߰Ÿ�

	//====================================== �⺻����(����) - �ʱ�: 2�������� ����
	StateInfo_.Level_ = 2;												// ����(�ʰ� ������ ����(�߻�����)�ϼ��� ������ ���� ����) - ������ ���ÿ� ����(�������� ������ ������ ��� �⺻ 1��������)
	StateInfo_.OffencePower_ = 51;										// ���ݷ�
	StateInfo_.Defense_ = 16;											// ����
	StateInfo_.HPMax_ = 942.0f;											// �ִ� ü��(������ ��ȭ)
	StateInfo_.HP_ = 942.0f;											// ����ü��
	StateInfo_.MoveSpeed_ = 2.85f;										// �̵��ӵ�
	StateInfo_.HomingInstinctValue_ = 100.0f;							// �ͼҺ��� ��ġ(0.0f���� �۰ų��������� ���ʹ� ������ġ�� �̵��ϸ� ü���� ȸ�� -> Ÿ���� DetectRange_�� ��� �ð��� ���� ����)
	StateInfo_.RegenTime_ = 195.0f;										// ����Ÿ��(����) -> 0.0f�����Ͻ� RegenTime_���� �ʱ�ȭ

	//====================================== ��ų����
	StateInfo_.IsSkill_ = 1;											// ��ų����(0: ��ų����)
	StateInfo_.SkillCoolDown_ = 7.0f;									// ��ų����ð�

	//====================================== �����۰���
	StateInfo_.DropItems_[ItemName::MEAT] = 40.0f;
	StateInfo_.DropItems_[ItemName::LEATHER] = 60.0f;

	if (true == GameServer::GetInstance()->IsOpened())					// �����̸� ���������� ����
	{
		// �ϴ� ����
		StateInfo_.CurDropItems_.push_back(ItemName::MEAT);
		StateInfo_.CurDropItems_.push_back(ItemName::LEATHER);
	}
}

void Boar::InitalizeResourceLoad()
{
	// ���ҽ� �ε�� �ѹ��� ó��
	if (false == ResourceLoadFlag)
	{
		// BaseMesh Resource Load & (Create Vertex & Index Buffer)
		GameEngineDirectory MeshDir;
		MeshDir.MoveParent("FoxTeam");
		MeshDir / "Resources" / "FBX" / "Monster" / "Boar";

		GameEngineFBXMesh* Basemesh = GameEngineFBXMeshManager::GetInst().Load(MeshDir.PathToPlusFileName("Boar_BaseMesh.fbx"));
		Basemesh->CreateRenderingBuffer();

		std::vector<GameEngineFile> allFile = MeshDir.GetAllFile("UserAnimation");
		for (GameEngineFile& file : allFile)
		{
			GameEngineFBXAnimationManager::GetInst().LoadUser(file.GetFullPath());
		}

		// Monster Resource Load Flag On
		ResourceLoadFlag = true;
	}
}

void Boar::InitalizeRenderAndAnimation()
{
	// ������ ����
	MainRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	MainRenderer_->SetFBXMesh("Boar_BaseMesh.fbx", "TextureDeferredLightAni");

	MainRenderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	MainRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	// �ִϸ��̼� ����
	MainRenderer_->CreateFBXAnimation("APPEAR", "Boar_appear.UserAnimation", 0, false);			// ù��������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("REGEN", "Boar_appear.UserAnimation", 0, false);			// ��������(���� ��� �� ����Ÿ�ӿ� ���� ������ ����)�� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("IDLE", "Boar_wait.UserAnimation", 0);						// �������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("CHASE", "Boar_run.UserAnimation", 0);						// ���������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("HOMINGINSTINCT", "Boar_run.UserAnimation", 0);				// ��ȯ������ �ִϸ��̼�

	MainRenderer_->CreateFBXAnimation("HIT", "Boar_wait.UserAnimation", 0);						// �ǰݻ����� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("DEATH", "Boar_death.UserAnimation", 0, false);				// ����߻����� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("DEAD", "Boar_death.UserAnimation", 0, false);				// ���(�������)������ �ִϸ��̼�

	MainRenderer_->CreateFBXAnimation("ATK01", "Boar_atk01.UserAnimation", 0, false);				// �Ϲݰ���01������ �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("ATK02", "Boar_atk02.UserAnimation", 0, false);				// �Ϲݰ���02������ �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("SKILLATTACK", "Boar_skill_ready.UserAnimation", 0, false);		// ��ų���ݻ����� �ִϸ��̼�
	MainRenderer_->ChangeFBXAnimation("IDLE");

	// �⺻���� ����
	ChangeAnimationAndState(MonsterStateType::APPEAR);
}

void Boar::InitalizeCollider()
{
	// �⺻: ��ü �浹ü ����
	BodyCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	BodyCollider_->SetCollisionGroup(eCollisionGroup::Monster);
	BodyCollider_->SetCollisionType(CollisionType::OBBBox3D);
	BodyCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	BodyCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());

	// �߰�: ���� �浹ü ����(�ɼ�)
	AtkCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	AtkCollider_->SetCollisionGroup(eCollisionGroup::MonsterAttack);
	AtkCollider_->SetCollisionType(CollisionType::OBBBox3D);
	AtkCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	AtkCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());
	AtkCollider_->Off();
}

void Boar::SkillAttackProcessing(float _DeltaTime)
{
	// ���� : ���������� �����ϸ� ���� �о�� ���ظ� ����
	//		  ��, ���� ���� �ε����ٸ� ����
	// ���ط� - �⺻(150) + ���ݷ��� 300%
	// �����Ÿ� - 5m
	// �����ð� - 1.5��
	// ��ٿ� - 7��
	// 1m == 100.0f�� ���
	
	//if (false == SkillAtk_)
	//{
	//	// ���� Ÿ���� ��ų�����Ÿ��� ���������� ��ų�����ð��� ��� �Ҹ��ߴٸ� ��ų���� ����
	//	SkillAtk_CastTime_ -= _DeltaTime;
	//	if (0.0f >= SkillAtk_CastTime_)
	//	{
	//		float4 MyPosition = GetTransform()->GetWorldPosition();
	//		float4 TargetPosition = CurTarget_->GetTransform()->GetWorldPosition();
	//		if ((TargetPosition - MyPosition).Len3D() <= SkillAtk_Range_)
	//		{
	//			// ��ų���� �ִϸ��̼� ����
	//			MainRenderer_->ChangeFBXAnimation("SKILLATTACK");

	//			// ��ų����ó���� ���� �غ�
	//			SkillAtk_ = true;

	//			// �����浹ü���� �÷��̾��� �ǰ��浹ü�� �浹üũ�� ���� On
	//			IsAttack_ = true;
	//			AtkCollider_->On();

	//			// Ÿ�� �˹� ���� ���
	//			float4 KnockbackDir = TargetPosition - MyPosition;

	//			// Ÿ�� �˹� ��Ŷ����
	//			CharCrowdControlPacket ccPacket;
	//			ccPacket.SetTargetIndex(CurTarget_->GetIndex());
	//			ccPacket.SetWallSlam(0.2f, KnockbackDir * 3000.f, 1.0f);
	//			FT::SendPacket(ccPacket);

	//			// ����?
	//			//GameEngineSoundManager::GetInstance()->PlaySoundByName("hyunwoo_Skill03_Hit.wav");
	//			//PacketSoundPlay packet;
	//			//packet.SetSound("hyunwoo_Skill03_Hit.wav", transform_.GetWorldPosition());
	//			//FT::SendPacket(packet);


	//			CurTarget_->Knockback();

	//			// Ÿ�� ������
	//			float CurDmage = SkillAtk_FixedDamage_ + (static_cast<float>(StateInfo_.OffencePower_) * 3.0f);
	//			CurTarget_->Damage(CurDmage, this);
	//		}

	//		// ��ų�����ð� �ʱ�ȭ
	//		SkillAtk_CastTime_ = SkillAtk_CastTimeMax_;
	//	}
	//}

	// �������� 
	if ("SKILLATTACK" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->CheckCurrentAnimationEnd())
	{
		// ��ų���ݿϷ�
		SkillAtk_ = false;

		// �������� ������ ��ȯ
		ChangeAnimationAndState(MonsterStateType::IDLE);
	}
}

Boar::Boar()
	: SkillAtk_(false)
	, SkillAtk_Range_(500.0f)
	, SkillAtk_CastTimeMax_(1.5f)
	, SkillAtk_CastTime_(1.5f)
	, SkillAtk_FixedDamage_(150.0f)
{
}

Boar::~Boar()
{
}
