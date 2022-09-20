#include "Precompile.h"
#include "Lobby_Map.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"

Lobby_Map::Lobby_Map()
	: SelectedLocation(Location::NONE)
{
}

Lobby_Map::~Lobby_Map()
{
}

void Lobby_Map::Start()
{
	{
		fullMapRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
		fullMapRenderer_->SetImage("Map_Full.png", "PointSmp");
		fullMapRenderer_->GetTransform()->SetLocalPosition({ 420.0f, 90.0f, -101.0f });
		fullMapRenderer_->GetTransform()->SetLocalScaling(fullMapRenderer_->GetCurrentTexture()->GetTextureSize());
	}

	{
		selectAreaRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
		selectAreaRenderer_->SetImage("Map_Alley_Pin.png", "PointSmp");
		selectAreaRenderer_->GetTransform()->SetLocalPosition({ 420.0f, 90.0f, -102.0f });
		selectAreaRenderer_->GetTransform()->SetLocalScaling(selectAreaRenderer_->GetCurrentTexture()->GetTextureSize());
	}


	{
		areaChoiceMapRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
		areaChoiceMapRenderer_->SetImage("Map_Resize_Color.png", "PointSmp");
		//항상 뒤에 가려서 안보이게함
		areaChoiceMapRenderer_->GetTransform()->SetLocalPosition({ 420.0f, 90.0f, -103.0f });
		areaChoiceMapRenderer_->GetTransform()->SetLocalScaling(areaChoiceMapRenderer_->GetCurrentTexture()->GetTextureSize());
		//areaChoiceMapRenderer_->Off();
	}

	{
		textureCollision_ = CreateTransformComponent<GameEngineCollision>();
		textureCollision_->GetTransform()->SetLocalPosition({ 420.0f, 90.0f, -101.0f });
		textureCollision_->GetTransform()->SetLocalScaling(fullMapRenderer_->GetCurrentTexture()->GetTextureSize());
		textureCollision_->SetCollisionInfo(static_cast<int>(CollisionGroup::UI), CollisionType::Rect);
	}
}

void Lobby_Map::Update(float _DeltaTime)
{
	if (textureCollision_->Collision(CollisionGroup::MousePointer))
	{
		float4 Pos = GameEngineInput::GetInst().GetMouse3DPos();
		GetSelectLocation(GameEngineInput::GetInst().GetMouse3DPos());
		//마우스콜리전과 맵콜리전이 충돌중일때만 겟컬러를한다
		
	}
}


Location Lobby_Map::GetSelectLocation(float4 _Position)
{
	//컬러맵으로 현재 내 마우스의 위치를 받아와서 그 좌표로 컬러값을 받아온다음 무슨 지역인지를 판단하는 함수

	//Gimp를 이용시 RGB가 아니라 BGR로 들어가 있음 왜?
	//->애초에 겟픽셀하면 BGRA로 들어가있다. xyz = bgr

	float4 ImagePos = fullMapRenderer_->GetTransform()->GetWorldPosition();
	float4 ImagehalfSize = {-155.0f, 178.0f, 0.0f, 0.0f};

	//이미지 좌측상단의 좌표를 구한다
	float4 ImageLeftTop = ImagePos + ImagehalfSize;

	//리소스와 피봇을 맞춤
	float4 returnPos = { (_Position.x - ImageLeftTop.x), (ImageLeftTop.y - _Position.y)};
	float4 Color = areaChoiceMapRenderer_->GetCurrentTexture()->GetPixel(returnPos.ix(), (returnPos.iy()));

	//Red값 = z
	//현재문제->이미지 크기조정중 생긴 흰 테두리가 방해가됨
	if (Color.z > 0.9f)
	{
		int a = 0;
	}

	

	return SelectedLocation;
}