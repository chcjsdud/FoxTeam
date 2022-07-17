#include "PreCompile.h"
#include "GameEngineCollision.h"
#include "GameEngineLevel.h"
#include "GameEngineTransform.h"


std::function<bool(GameEngineTransform*, GameEngineTransform*)>
GameEngineCollision::CollisionCheckFunction[static_cast<int>(CollisionType::MAX)][static_cast<int>(CollisionType::MAX)];


void GameEngineCollision::Init() 
{
	CollisionCheckFunction[static_cast<int>(CollisionType::CirCle)][static_cast<int>(CollisionType::CirCle)]
		= std::bind(&GameEngineCollision::CirCleToCirCle, std::placeholders::_1, std::placeholders::_2);

	CollisionCheckFunction[static_cast<int>(CollisionType::Sphere3D)][static_cast<int>(CollisionType::Sphere3D)]
		= std::bind(&GameEngineCollision::Sphere3DToSphere3D, std::placeholders::_1, std::placeholders::_2);


	CollisionCheckFunction[static_cast<int>(CollisionType::Rect)][static_cast<int>(CollisionType::Rect)]
		= std::bind(&GameEngineCollision::RectToRect, std::placeholders::_1, std::placeholders::_2);

	CollisionCheckFunction[static_cast<int>(CollisionType::AABBBox3D)][static_cast<int>(CollisionType::AABBBox3D)]
		= std::bind(&GameEngineCollision::AABBToAABB, std::placeholders::_1, std::placeholders::_2);


	CollisionCheckFunction[static_cast<int>(CollisionType::OBBBox3D)][static_cast<int>(CollisionType::OBBBox3D)]
		= std::bind(&GameEngineCollision::OBBToOBB, std::placeholders::_1, std::placeholders::_2);

	CollisionCheckFunction[static_cast<int>(CollisionType::CirCle)][static_cast<int>(CollisionType::AABBBox3D)]
		= std::bind(&GameEngineCollision::CirCleToAABB, std::placeholders::_1, std::placeholders::_2);

	CollisionCheckFunction[static_cast<int>(CollisionType::AABBBox3D)][static_cast<int>(CollisionType::CirCle)]
		= std::bind(&GameEngineCollision::AABBToCirCle, std::placeholders::_1, std::placeholders::_2);
}


bool GameEngineCollision::OBBToOBB(GameEngineTransform* _Left, GameEngineTransform* _Right) {
	return _Left->GetOBB().Intersects(_Right->GetOBB());
}

bool GameEngineCollision::RectToRect(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	DirectX::BoundingBox Left = _Left->GetAABB();
	DirectX::BoundingBox Right = _Right->GetAABB();
	Left.Center.z = 0.0f;
	Right.Center.z = 0.0f;
	return Left.Intersects(Right);
}

bool GameEngineCollision::AABBToAABB(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	// DirectX::TriangleTests::Intersects()

	return _Left->GetAABB().Intersects(_Right->GetAABB());
}


bool GameEngineCollision::CirCleToCirCle(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	DirectX::BoundingSphere Left = _Left->GetSphere();
	DirectX::BoundingSphere Right = _Right->GetSphere();
	Left.Center.z = 0.0f;
	Right.Center.z = 0.0f;
	return Left.Intersects(Right);
}

bool GameEngineCollision::Sphere3DToSphere3D(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	return _Left->GetSphere().Intersects(_Right->GetSphere());
}

bool GameEngineCollision::CirCleToAABB(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	DirectX::BoundingSphere Left = _Left->GetSphere();
	DirectX::BoundingBox Right = _Right->GetAABB();
	Left.Center.z = 0.0f;
	Right.Center.z = 0.0f;
	return Left.Intersects(Right);
}

bool GameEngineCollision::AABBToCirCle(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	return CirCleToAABB(_Right, _Left);
}

GameEngineCollision::GameEngineCollision()
	: ColType_(CollisionType::Rect)
{
}

GameEngineCollision::~GameEngineCollision() 
{
}

void GameEngineCollision::Start()
{
	GetLevel()->PushCollision(this, GetOrder());
}

void GameEngineCollision::Update(float _DeltaTime)
{
	
}

void GameEngineCollision::SetCollisionGroup(int _Type)
{
	GetLevel()->ChangeCollisionGroup(_Type, this);
}

bool GameEngineCollision::Collision(int _OtherGroup)
{
	std::list<GameEngineCollision*>& Group = GetLevel()->GetCollisionGroup(_OtherGroup);

	for (GameEngineCollision* OtherCollision : Group)
	{
		if (false == OtherCollision->IsUpdate())
		{
			continue;
		}

		auto& CheckFunction = CollisionCheckFunction[static_cast<int>(ColType_)][static_cast<int>(OtherCollision->ColType_)];

		if (nullptr == CheckFunction)
		{
			GameEngineDebug::MsgBoxError("아직 구현하지 않는 타입간에 충돌을 하려고 했습니다.");
		}

		return CheckFunction(GetTransform(), OtherCollision->GetTransform());
	}

	return false;
}

GameEngineCollision* GameEngineCollision::CollisionPtr(int _OtherGroup)
{
	std::list<GameEngineCollision*>& Group = GetLevel()->GetCollisionGroup(_OtherGroup);

	for (GameEngineCollision* OtherCollision : Group)
	{
		if (false == OtherCollision->IsUpdate())
		{
			continue;
		}

		auto& CheckFunction = CollisionCheckFunction[static_cast<int>(ColType_)][static_cast<int>(OtherCollision->ColType_)];

		if (nullptr == CheckFunction)
		{
			GameEngineDebug::MsgBoxError("아직 구현하지 않는 타입간에 충돌을 하려고 했습니다.");
		}

		if (false ==CheckFunction(GetTransform(), OtherCollision->GetTransform()))
		{
			continue;
		}

		return OtherCollision;
	}

	return nullptr;
}

void GameEngineCollision::Collision(CollisionType _ThisType, CollisionType _OtherType, int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack)
{
	std::list<GameEngineCollision*>& Group = GetLevel()->GetCollisionGroup(_OtherGroup);

	for (GameEngineCollision* OtherCollision : Group)
	{
		if (false == OtherCollision->IsUpdate())
		{
			continue;
		}

		auto& CheckFunction = CollisionCheckFunction[static_cast<int>(_ThisType)][static_cast<int>(_OtherType)];

		if (nullptr == CheckFunction)
		{
			GameEngineDebug::MsgBoxError("아직 구현하지 않는 타입간에 충돌을 하려고 했습니다.");
		}

		if (
			false == 
			CheckFunction(GetTransform(), OtherCollision->GetTransform())
			)
		{
			continue;
		}

		_CallBack(OtherCollision);
	}
}

void GameEngineCollision::SphereToSphereCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack) 
{
	Collision(CollisionType::Sphere3D, CollisionType::Sphere3D, _OtherGroup, _CallBack);
}