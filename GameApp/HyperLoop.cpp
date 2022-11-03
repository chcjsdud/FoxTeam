#include "PreCompile.h"
#include "HyperLoop.h"
#include <GameEngine/GameEngineCollision.h>
#include "LumiaLevel.h"
#include "LumiaMap.h"
#include "PlayerInfoManager.h"

HyperLoop::HyperLoop()
	: renderer_(nullptr)
	, collision_(nullptr)

{

}

HyperLoop::~HyperLoop()
{

}

void HyperLoop::Start()
{
	static bool bLoad = false;

	if (false == bLoad)
	{
		GameEngineDirectory dir;

		dir.MoveParent("FoxTeam");
		dir / "Resources" / "FBX" / "Map";

		GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Hyperloop.fbx"));
		mesh->CreateRenderingBuffer();

		bLoad = true;
	}

	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh("Hyperloop.fbx", "TextureDeferredLight");
	renderer_->GetTransform()->SetLocalScaling(100.0f);

	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->GetTransform()->SetLocalScaling({ 150.0f , 250.0f, 150.0f });
	collision_->GetTransform()->SetLocalMove({ 0.0f , 125.0f, 0.0f });
	collision_->SetCollisionGroup(eCollisionGroup::HyperLoop);
	collision_->SetCollisionType(CollisionType::OBBBox3D);

	//�̰�ȣ : ��UI CreateActor �� �ʱ�ȭ
	mapUI_ = GetLevel()->CreateActor<UI_HyperMap>();
	//�ʱ�ȭ�� Off�ؼ� ��� ����� ����
	mapUI_->MapOff();
}

void HyperLoop::Update(float _DeltaTime)
{
	Location SelectedArea = Location::NONE;

	if (nullptr != collision_ &&
		collision_->IsUpdate())
	{
		GetLevel()->PushDebugRender(collision_->GetTransform(), CollisionType::OBBBox3D, float4::BLACK);
	}

	if (true == collision_->Collision(eCollisionGroup::MouseRay) &&
		true == collision_->Collision(eCollisionGroup::Player))
	{
		// �����۷��� UI�� ������, Ư�� ������ �����ϸ� �ش� �������� �̵�

		//�̰�ȣ : �� UI�� Ų�� 
		// MapOff(); �Լ��� ����ϸ� Map��Ÿ� ���� �ֽ��ϴ�
		mapUI_->MapOn();

		//�̰�ȣ : ���� �ʿ��� ��Ŭ������ Ŭ���� ������ enum class Location���� ���� �� �ֽ��ϴ�
		SelectedArea = mapUI_->ReturnSelectedLocation();
	}
	else
	{
		mapUI_->MapOff();
	}

	if (Location::NONE != SelectedArea)
	{
		Character* player = PlayerInfoManager::GetInstance()->GetMainCharacter();

		std::vector<float4> spawnPoints = GetLevelConvert<LumiaLevel>()->GetMap()->GetCharacterSpawnPoints(static_cast<int>(SelectedArea));
		GameEngineRandom random;
		int point = random.RandomInt(0, spawnPoints.size() - 1);

		player->GetTransform()->SetWorldPosition(spawnPoints[point]);
		mapUI_->MapOff();
	}
}

