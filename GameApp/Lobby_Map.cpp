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
		selectAreaRenderer_->GetTransform()->SetLocalPosition({ 420.0f, 90.0f, -104.0f });
		selectAreaRenderer_->GetTransform()->SetLocalScaling(selectAreaRenderer_->GetCurrentTexture()->GetTextureSize());
		selectAreaRenderer_->Off();
	}


	{
		areaChoiceMapRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
		areaChoiceMapRenderer_->SetImage("Map_Resize_Color.png", "PointSmp");
		//항상 뒤에 가려서 안보이게함
		areaChoiceMapRenderer_->GetTransform()->SetLocalPosition({ 420.0f, 90.0f,0.0f });
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
		//마우스콜리전과 맵콜리전이 충돌중일때만 겟컬러를한다
		GetSelectLocation(GameEngineInput::GetInst().GetMouse3DPos());

		//맵콜리전이 충돌중일때 
		if (GameEngineInput::GetInst().Down("LBUTTON"))
		{
			switch (SelectedLocation)
			{
			case Location::NONE:
				selectAreaRenderer_->Off();
				break;
			case Location::DOCK:
				selectAreaRenderer_->On();
				selectAreaRenderer_->SetImage("Map_Harbor_Pin.png", "PointSmp");
				break;
			case Location::POND:
				selectAreaRenderer_->On();
				selectAreaRenderer_->SetImage("Map_Pond_Pin.png", "PointSmp");
				break;
			case Location::BEACH:
				selectAreaRenderer_->On();
				selectAreaRenderer_->SetImage("Map_SandyBeach_Pin.png", "PointSmp");
				break;
			case Location::UPTOWN:
				selectAreaRenderer_->On();
				selectAreaRenderer_->SetImage("Map_UpTown_Pin.png", "PointSmp");
				break;
			case Location::ALLEY:
				selectAreaRenderer_->On();
				selectAreaRenderer_->SetImage("Map_Alley_Pin.png", "PointSmp");
				break;
			case Location::HOTEL:
				selectAreaRenderer_->On();
				selectAreaRenderer_->SetImage("Map_Hotel_Pin.png", "PointSmp");
				break;
			case Location::AVENUE:
				selectAreaRenderer_->On();
				selectAreaRenderer_->SetImage("Map_DownTown_Pin.png", "PointSmp");
				break;
			case Location::HOSPITAL:
				selectAreaRenderer_->On();
				selectAreaRenderer_->SetImage("Map_Hospital_Pin.png", "PointSmp");
				break;
			case Location::TEMPLE:
				selectAreaRenderer_->On();
				selectAreaRenderer_->SetImage("Map_Temple_Pin.png", "PointSmp");
				break;
			case Location::ARCHERY_RANGE:
				selectAreaRenderer_->On();
				selectAreaRenderer_->SetImage("Map_Archery_Pin.png", "PointSmp");
				break;
			case Location::CEMETERY:
				selectAreaRenderer_->On();
				selectAreaRenderer_->SetImage("Map_Cemetery_Pin.png", "PointSmp");
				break;
			case Location::FOREST:
				selectAreaRenderer_->On();
				selectAreaRenderer_->SetImage("Map_Forest_Pin.png", "PointSmp");
				break;
			case Location::FACTORY:
				selectAreaRenderer_->On();
				selectAreaRenderer_->SetImage("Map_Factory_Pin.png", "PointSmp");
				break;
			case Location::CHAPEL:
				selectAreaRenderer_->On();
				selectAreaRenderer_->SetImage("Map_Church_Pin.png", "PointSmp");
				break;
			case Location::SCHOOL:
				selectAreaRenderer_->On();
				selectAreaRenderer_->SetImage("Map_School_Pin.png", "PointSmp");
				break;
			case Location::RESERCH_CENTRE:
				selectAreaRenderer_->On();
				selectAreaRenderer_->SetImage("Map_Laboratory_Pin.png", "PointSmp");
				break;
			case Location::ESCAPE_DOCK:
				selectAreaRenderer_->Off();
				break;
			case Location::MAX:
				selectAreaRenderer_->Off();
				break;
			default:
				break;
			}
		}
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
	//테두리 삭제 + 값을 테두리의 색상값보다 높임

	if (Color.z < 0.1f && Color.y < 0.1f && Color.z < 0.1f)
	{
		//0, 0, 0 일경우
		SelectedLocation = Location::NONE;
		return SelectedLocation;
	}

	if (Color.z > 0.95f)
	{
		//255, *, * 인경우

		if (Color.y < 0.1f && Color.x < 0.1f)
		{
			//255, 0, 0
			SelectedLocation = Location::ALLEY;
		}

		if (Color.y > 0.95f && Color.x < 0.1f)
		{
			//255, 255, 0
			SelectedLocation = Location::ARCHERY_RANGE;
		}

		if (Color.y < 0.1f && Color.x > 0.95f)
		{
			//255, 0, 255
			SelectedLocation = Location::FACTORY;
		}

		if (Color.y > 0.95f && Color.x > 0.95f)
		{
			//255, 255, 255
			SelectedLocation = Location::CEMETERY;
		}

		if (Color.y < 0.6f && Color.y > 0.4f && Color.x < 0.1f)
		{
			//255, 128, 0
			SelectedLocation = Location::RESERCH_CENTRE;
		}

		if (Color.y < 0.6f && Color.y > 0.4f && Color.x > 0.95f)
		{
			//255, 128, 255
			SelectedLocation = Location::POND;
		}

		if (Color.y < 0.1f && Color.x < 0.6f && Color.x > 0.4f)
		{
			//255, 0, 128
			SelectedLocation = Location::TEMPLE;
		}

		if (Color.y > 0.95f && Color.x < 0.6f && Color.x > 0.4f)
		{
			//255, 255, 128
			SelectedLocation = Location::UPTOWN;
		}
	}
	else if (Color.z > 0.4f && Color.z < 0.6f)
	{
		// 128, *, *

		if (Color.y < 0.1f && Color.x < 0.1f)
		{
			//128, 0, 0
			SelectedLocation = Location::FOREST;
		}

		if (Color.y > 0.95f && Color.x < 0.1f)
		{
			//128, 255, 0
			SelectedLocation = Location::DOCK;
		}

		if (Color.y > 0.95f && Color.x > 0.95f)
		{
			//128, 255, 255
			SelectedLocation = Location::HOSPITAL;
		}
	}
	else if (Color.z < 0.05f)
	{
		//0, *, *

		if (Color.y > 0.95f && Color.x < 0.1f)
		{
			//0, 255, 0
			//교회
			SelectedLocation = Location::CHAPEL;
		}

		if (Color.y > 0.95f && Color.x > 0.95f)
		{
			//0, 255, 255
			//다운타운
			SelectedLocation = Location::AVENUE;
		}

		if (Color.y < 0.05f && Color.x > 0.95f)
		{
			//0, 0, 255
			SelectedLocation = Location::SCHOOL;
		}


		if (Color.y < 0.6f && Color.y > 0.4f && Color.x < 0.1f)
		{
			//0, 128, 0
			SelectedLocation = Location::HOTEL;
		}

		if (Color.y < 0.6f && Color.y > 0.4f && Color.x > 0.95f)
		{
			//0, 128, 255
			SelectedLocation = Location::BEACH;
		}

		if (Color.y < 0.6f && Color.y > 0.4f && Color.x < 0.6f && Color.x > 0.4f)
		{
			//0, 128, 128
			SelectedLocation = Location::SCHOOL;
		}
	}


	return SelectedLocation;
}