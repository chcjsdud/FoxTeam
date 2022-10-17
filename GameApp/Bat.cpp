#include "PreCompile.h"
#include "Bat.h"

#include <GameEngine/GameEngineCollision.h>

bool Bat::ResourceLoadFlag = false;

void Bat::InitalizeStateInfo()
{
	// �⺻����
	Type_ = MonsterType::BAT;											// ����Ÿ��

	// �⺻����
	StateInfo_.HPMax_ = 100.0f;											// �ִ�ü��
	StateInfo_.HP_ = 100.0f;											// ����ü��
	StateInfo_.HPRegenPercent_ = 0.5f;									// ü��ȸ����(% ��ġ)
	StateInfo_.SPMax_ = 100.0f;											// �ִ븶��
	StateInfo_.SP_ = 100.0f;											// ���縶��
	StateInfo_.SPRegenPercent_ = 0.5f;									// ����ȸ����(% ��ġ)
	StateInfo_.Defence_ = 100.0f;										// ����

	// �󼼽���
	StateInfo_.NestPosition_ = float4::ZERO;							// ������ġ(= ������ġ)
	StateInfo_.HomingInstinctValueMax_ = 100.0f;						// �ִ� �ͼҺ��� ��ġ
	StateInfo_.HomingInstinctValue_ = 100.0f;							// �ͼҺ��� ��ġ(0.0f���� �۰ų��������� ���ʹ� ������ġ�� �̵��ϸ� ü���� ȸ�� -> Ÿ���� DetectRange_�� ��� �ð��� ���� ����)
	StateInfo_.AttackRange_ = 100.0f;									// ���� �þ߰Ÿ�
	StateInfo_.DetectRange_ = 300.0f;									// ���� �þ߰Ÿ�
	StateInfo_.AttackSpeed_ = 0.5f;										// ���� �ӵ�
	StateInfo_.MoveSpeed_ = 100.0f;										// �̵� �ӵ�

	// ��ų����
	StateInfo_.SkillCoolDown_ = 0.5f;									// ��ų����ð�

	// ������
	StateInfo_.RegenTimeMax_ = 125.f;									// ����Ÿ��(����)
	StateInfo_.RegenTime_ = 125.f;										// ����Ÿ��(����) -> 0.0f�����Ͻ� RegenTime_���� �ʱ�ȭ
}

void Bat::InitalizeResourceLoad()
{
	// ���ҽ� �ε�� �ѹ��� ó��
	if (false == ResourceLoadFlag)
	{
		// BaseMesh Resource Load & (Create Vertex & Index Buffer)
		GameEngineDirectory MeshDir;
		MeshDir.MoveParent("FoxTeam");
		MeshDir / "Resources" / "FBX" / "Monster" / "Bat";

		GameEngineFBXMesh* Basemesh = GameEngineFBXMeshManager::GetInst().Load(MeshDir.PathToPlusFileName("Bat_BaseMesh.fbx"));
		Basemesh->CreateRenderingBuffer();

		// Animation Resource Load
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Bat_appear.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Bat_wait.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Bat_run.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Bat_death.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Bat_atk01.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Bat_atk02.fbx"));

		// Monster Resource Load Flag On
		ResourceLoadFlag = true;
	}
}

void Bat::InitalizeRenderAndAnimation()
{
	// ������ ����
	MainRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	MainRenderer_->SetFBXMesh("Bat_BaseMesh.fbx", "TextureDeferredLightAni");

	MainRenderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	MainRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	// �ִϸ��̼� ����
	MainRenderer_->CreateFBXAnimation("APPEAR", "Bat_appear.fbx", 0, false);			// ù��������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("REGEN", "Bat_appear.fbx", 0, false);				// ��������(���� ��� �� ����Ÿ�ӿ� ���� ������ ����)�� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("IDLE", "Bat_wait.fbx", 0);						// �������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("RUN", "Bat_run.fbx", 0);							// �̵������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("HOMINGINSTINCT", "Bat_run.fbx", 0);				// ��ȯ������ �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("CHASE", "Bat_run.fbx", 0);						// ���������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("HIT", "Bat_wait.fbx", 0);						// �ǰݻ����� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("DEATH", "Bat_death.fbx", 0, false);				// ����߻����� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("DEAD", "Bat_death.fbx", 0, false);				// ���(�������)������ �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("ATK01", "Bat_atk01.fbx", 0, false);				// �Ϲݰ���01������ �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("ATK02", "Bat_atk02.fbx", 0, false);				// �Ϲݰ���02������ �ִϸ��̼�
	//MainRenderer_->CreateFBXAnimation("SKILLATTACK", "Bat_skill.fbx", 0, false);		// ��ų���ݻ����� �ִϸ��̼�(������)
	MainRenderer_->ChangeFBXAnimation("IDLE");

	// �⺻���� ����
	ChangeAnimationAndState(MonsterStateType::APPEAR);
}

void Bat::InitalizeCollider()
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

Bat::Bat()
{
}

Bat::~Bat()
{
}
