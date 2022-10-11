#include "Precompile.h"
#include "Wolf.h"

#include <GameEngine/GameEngineCollision.h>

bool Wolf::ResourceLoadFlag = false;

void Wolf::InitalizeStateInfo()
{
	// �⺻����
	Type_ = MonsterType::WOLF;											// ����Ÿ��

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
	StateInfo_.FirstAppearTime_ = 125.f;								// ù���� Ÿ��
	StateInfo_.RegenTimeMax_ = 125.f;									// ����Ÿ��(����)
	StateInfo_.RegenTime_ = 125.f;										// ����Ÿ��(����) -> 0.0f�����Ͻ� RegenTime_���� �ʱ�ȭ
}

void Wolf::InitalizeResourceLoad()
{
	// ���ҽ� �ε�� �ѹ��� ó��
	if (false == Wolf::ResourceLoadFlag)
	{
		// BaseMesh Resource Load & (Create Vertex & Index Buffer)
		GameEngineDirectory MeshDir;
		MeshDir.MoveParent("FoxTeam");
		MeshDir / "Resources" / "FBX" / "Monster" / "Wolf";

		GameEngineFBXMesh* Basemesh = GameEngineFBXMeshManager::GetInst().Load(MeshDir.PathToPlusFileName("Wolf_BaseMesh.fbx"));
		Basemesh->CreateRenderingBuffer();
	
		// Animation Resource Load
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Wolf_appear.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Wolf_wait.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Wolf_run.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Wolf_death.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Wolf_atk01.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Wolf_atk02.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Wolf_skill.fbx"));

		// Monster Wolf Resource Load Flag On
		Wolf::ResourceLoadFlag = true;
	}
}

void Wolf::InitalizeRenderAndAnimation()
{
	// ������ ����
	MainRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	MainRenderer_->SetFBXMesh("Wolf_BaseMesh.fbx", "TextureDeferredLightAni");

	MainRenderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	MainRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	// �ִϸ��̼� ����
	MainRenderer_->CreateFBXAnimation("APPEAR", "Wolf_appear.fbx", 0, false);			// ù��������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("REGEN", "Wolf_appear.fbx", 0, false);			// ��������(���� ��� �� ����Ÿ�ӿ� ���� ������ ����)�� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("IDLE", "Wolf_wait.fbx", 0);						// �������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("RUN", "Wolf_run.fbx", 0);						// �̵������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("HOMINGINSTINCT", "Wolf_run.fbx", 0);				// ��ȯ������ �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("CHASE", "Wolf_run.fbx", 0);						// ���������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("HIT", "Wolf_wait.fbx", 0);						// �ǰݻ����� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("DEATH", "Wolf_death.fbx", 0, false);				// ����߻����� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("DEAD", "Wolf_death.fbx", 0, false);				// ���(�������)������ �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("ATK01", "Wolf_atk01.fbx", 0, false);				// �Ϲݰ���01������ �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("ATK02", "Wolf_atk02.fbx", 0, false);				// �Ϲݰ���02������ �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("SKILLATTACK", "Wolf_skill.fbx", 0, false);		// ��ų���ݻ����� �ִϸ��̼�
	MainRenderer_->ChangeFBXAnimation("IDLE");

	// �⺻���� ����
	ChangeAnimationAndState(MonsterStateType::APPEAR);
}

void Wolf::InitalizeCollider()
{
	//// �⺻: ��ü �浹ü ����
	//BodyCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	//BodyCollider_->SetCollisionGroup(eCollisionGroup::Monster);
	//BodyCollider_->SetCollisionType(CollisionType::AABBBox3D);
	//BodyCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	//BodyCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());

	// �߰�: ���� �浹ü ����(�ɼ�)
	//AtkCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	//AtkCollider_->SetCollisionGroup(eCollisionGroup::MonsterAttack);
	//AtkCollider_->SetCollisionType(CollisionType::AABBBox3D);
	//AtkCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	//AtkCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());
	//AtkCollider_->Off();
}

Wolf::Wolf()
{

}

Wolf::~Wolf()
{
}
