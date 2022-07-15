#include "PreCompile.h"
#include <GameEngine/GameEngineCollision.h>
#include "Map.h"

Map::Map() // default constructer 디폴트 생성자
{

}

Map::~Map() // default destructer 디폴트 소멸자
{

}

void Map::Start()
{
	GroundCollision_ = CreateTransformComponent<GameEngineCollision>();
	GroundCollision_->GetTransform()->SetLocalScaling({ 2000.f,1.f,2000.f });
}

void Map::Update(float _DeltaTime)
{
	GetLevel()->PushDebugRender(GroundCollision_->GetTransform(), CollisionType::AABBBox3D,float4::RED);
}


