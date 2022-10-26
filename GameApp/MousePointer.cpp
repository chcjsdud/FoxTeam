#include "PreCompile.h"
#include "MousePointer.h"

#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "Enums.h"

MousePointer* MousePointer::InGameMouse = nullptr;

float4 MousePointer::GetIntersectionYAxisPlane(float _height, float _rayLength)
{
	return DirectX::XMPlaneIntersectLine({ 0.0f, 1.0f, 0.0f, -_height }, rayOrigin_.DirectVector, (rayOrigin_ + rayDirection_ * _rayLength).DirectVector);;
}

GameEngineCollision* MousePointer::GetPickCollision(int _Order)
{
	// ���� ������ ��� �浹ü ��� Get
	std::map<int, std::list<GameEngineCollision*>> AllList = GetLevel()->GetAllCollision();

	// �ش� ������ �����ϴ� �浹ü Ž��
	std::list<GameEngineCollision*>::iterator StartIter = AllList[_Order].begin();
	std::list<GameEngineCollision*>::iterator EndIter = AllList[_Order].end();

	for (; StartIter != EndIter; ++StartIter)
	{
		// ������ ���������������� ���õ� �浹ü�� �����ϴ� ���������� �Ÿ�
		float Dist = 0.0f;
		if (true == (*StartIter)->BoundingToRayCollision((*StartIter)->GetCollisionType(), rayOrigin_, rayDirection_, Dist))
		{
			return (*StartIter);
		}
	}

	return nullptr;
}

void MousePointer::updateMouseRay()
{
	// ����Ʈ ������ �����ɴϴ�. ����Ʈ�� �ϳ��� ����߰�, �ϳ��� ������ ���̱� ������ 1 �� �ʱ�ȭ�մϴ�.
	UINT numViewport = 1;
	D3D11_VIEWPORT viewports = { 0, };

	// �Էµ� ù ��° ���� �� ��ŭ ����Ʈ�� �����ɴϴ�.
	// ���� 2��° ���ڰ� NULL �̸�, ù��° ���ڷ� ���� UINT�� ���� ä�����ϴ�.
	// ä���� ���� ���� �����Ͷ������� ���ε� �� ����Ʈ �� �Դϴ�.
	//dc->RSGetViewports(&numViewport, nullptr);
	GameEngineDevice::GetContext()->RSGetViewports(&numViewport, &viewports);

	if (viewports.Width == 0.0f || viewports.Height == 0.0f)
	{
		return;
	}

	// ��ǲ Ŭ�������� ���콺�� ��ġ�� �����ɴϴ�.
	float4 mousePosition = GameEngineInput::GetInst().GetMousePos();
	float pointX = mousePosition.x;
	float pointY = mousePosition.y;

	// ���콺�� ��ġ�� ����Ʈ�� ��� �� �����ϴ�.
	if (pointX <= 0.0f)
	{
		pointX = 0.0f;
	}
	else if (pointX >= viewports.Width)
	{
		pointX = viewports.Width;
	}

	if (pointY <= 0.0f)
	{
		pointY = 0.0f;
	}
	else if (pointY >= viewports.Height)
	{
		pointY = viewports.Height;
	}

	// ���콺 Ŀ�� ��ǥ�� -1���� +1 ������ �ٲߴϴ�. (NDC)
	pointX = ((2.0f * pointX) / viewports.Width) - 1.0f;
	pointY = (((2.0f * pointY) / viewports.Height) - 1.0f) * -1.0f;

	// ����Ʈ�� ��Ⱦ�� ����Ͽ� ���� ����� ����� ���� �����մϴ�
	float4x4 matProjection = GetLevel()->GetMainCameraActor()->GetProjectionMatrix();

	pointX = pointX / matProjection._11;
	pointY = pointY / matProjection._22;

	// �� ����� ������� ���մϴ�.
	float4x4 inverseViewMatrix = GetLevel()->GetMainCameraActor()->GetViewMatrix().InverseReturn();

	// �� ������� ���� ������ �˾Ƴ��ϴ�.
	rayDirection_.x = pointX;
	rayDirection_.y = pointY;
	rayDirection_.z = 1.0f;
	rayDirection_.w = 0.0f;

	rayDirection_ = rayDirection_ * inverseViewMatrix;
	rayDirection_.Normalize3D();

	// �� ������� 4���� �� ī�޶��� ��ġ�Դϴ�.
	rayOrigin_ = inverseViewMatrix.vw;
}

void MousePointer::Start()
{
	// ���� �浹ü ����
	rayCollision_ = CreateTransformComponent<GameEngineCollision>();
	rayCollision_->SetCollisionType(CollisionType::Ray);
	rayCollision_->SetCollisionGroup(eCollisionGroup::MouseRay);

	// ���콺 ������ ����
	MouseRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(static_cast<int>(RenderOrder::MOUSE));
	MouseRenderer_->SetImage(GameEngineString::toupper("Cursor_01.png"));
	MouseRenderer_->GetTransform()->SetLocalPosition({ 12.0f, -10.0f, 0.0f });
	MouseRenderer_->GetTransform()->SetLocalScaling({ 30.0f, 30.0f, 30.0f });

	// ���콺 �浹ü ����
	MouseCollider_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(RenderOrder::MOUSE));
	MouseCollider_->GetTransform()->SetLocalScaling(float4{ 30.0f, 30.0f, 30.0f });
	MouseCollider_->SetCollisionInfo(static_cast<int>(eCollisionGroup::MousePointer), CollisionType::AABBBox3D);

#pragma region Ű����
	if (false == GameEngineInput::GetInst().IsKey("LButton"))
	{
		GameEngineInput::GetInst().CreateKey("LButton", VK_LBUTTON);
	}

	if (false == GameEngineInput::GetInst().IsKey("RButton"))
	{
		GameEngineInput::GetInst().CreateKey("RButton", VK_RBUTTON);
	}
#pragma endregion
}

void MousePointer::Update(float _deltaTime)
{
	// Update Ray
	updateMouseRay();
	rayCollision_->SetRayData(rayOrigin_, rayDirection_);

	// 
	GetTransform()->SetWorldPosition(GameEngineInput::GetInst().GetMouse3DPos());

	// Űüũ
	if (true == GameEngineInput::GetInst().Down("LBUTTON"))
	{

	}

	if (true == GameEngineInput::GetInst().Down("RBUTTON"))
	{

	}
}

void MousePointer::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
}

void MousePointer::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	// ������������ ������
	if (std::string::npos != _NextLevel->GetName().find("Ending"))
	{
		return;
	}
	// ���� �׽�Ʈ�������� ������
	else if (std::string::npos != _NextLevel->GetName().find("MeshLoadLevel"))
	{
		return;
	}
	else if (std::string::npos != _NextLevel->GetName().find("OutLineTestLevel"))
	{
		return;
	}
	else if (std::string::npos != _NextLevel->GetName().find("TestLevel"))
	{
		return;
	}

	// ���콺 �����̵�
	GetLevel()->SetLevelActorMove(_NextLevel, this);
}

MousePointer::MousePointer()
	: rayDirection_(0.0f, 0.0f, 1.0f, 1.0f)
	, rayOrigin_(float4::ZERO)
	, rayCollision_(nullptr)
	, MouseRenderer_(nullptr)
	, MouseCollider_(nullptr)
{
}

MousePointer::~MousePointer()
{
}
