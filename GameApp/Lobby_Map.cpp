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
		//�׻� �ڿ� ������ �Ⱥ��̰���
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
		//���콺�ݸ����� ���ݸ����� �浹���϶��� ���÷����Ѵ�
		
	}
}


Location Lobby_Map::GetSelectLocation(float4 _Position)
{
	//�÷������� ���� �� ���콺�� ��ġ�� �޾ƿͼ� �� ��ǥ�� �÷����� �޾ƿ´��� ���� ���������� �Ǵ��ϴ� �Լ�

	//Gimp�� �̿�� RGB�� �ƴ϶� BGR�� �� ���� ��?
	//->���ʿ� ���ȼ��ϸ� BGRA�� ���ִ�. xyz = bgr

	float4 ImagePos = fullMapRenderer_->GetTransform()->GetWorldPosition();
	float4 ImagehalfSize = {-155.0f, 178.0f, 0.0f, 0.0f};

	//�̹��� ��������� ��ǥ�� ���Ѵ�
	float4 ImageLeftTop = ImagePos + ImagehalfSize;

	//���ҽ��� �Ǻ��� ����
	float4 returnPos = { (_Position.x - ImageLeftTop.x), (ImageLeftTop.y - _Position.y)};
	float4 Color = areaChoiceMapRenderer_->GetCurrentTexture()->GetPixel(returnPos.ix(), (returnPos.iy()));

	//Red�� = z
	//���繮��->�̹��� ũ�������� ���� �� �׵θ��� ���ذ���
	if (Color.z > 0.9f)
	{
		int a = 0;
	}

	

	return SelectedLocation;
}