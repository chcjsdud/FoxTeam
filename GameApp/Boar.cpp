#include "PreCompile.h"
#include "Boar.h"

#include <GameEngine/GameEngineCollision.h>

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
	StateInfo_.SkillCoolDownMax_ = 10.0f;								// �ִ� ��ų��Ÿ��

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
	StateInfo_.SkillCoolDown_ = 0.5f;									// ��ų����ð�

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

		// Animation Resource Load
		//GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Boar_appear.fbx"));
		//GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Boar_wait.fbx"));
		//GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Boar_run.fbx"));
		//GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Boar_death.fbx"));
		//GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Boar_atk01.fbx"));
		//GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Boar_atk02.fbx"));
		//GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Boar_skill_assault.fbx"));		// �ε常�� ������ ���� ������...
		//GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Boar_skill_ready.fbx"));			// �ε常�� ������ ���� ������...

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
	//AtkCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	//AtkCollider_->SetCollisionGroup(eCollisionGroup::MonsterAttack);
	//AtkCollider_->SetCollisionType(CollisionType::AABBBox3D);
	//AtkCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	//AtkCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());
	//AtkCollider_->Off();
}

Boar::Boar()
{
}

Boar::~Boar()
{
}
