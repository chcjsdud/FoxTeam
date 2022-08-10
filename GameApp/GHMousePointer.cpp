#include "PreCompile.h"
#include "GHMousePointer.h"

#include <GameEngine/GameEngineCollision.h>

GHMousePointer::GHMousePointer()
	: rayCollision_(nullptr)
	, rayDirection_(0.0f, 0.0f, 1.0f, 1.0f)
{

}

GHMousePointer::~GHMousePointer()
{

}

void GHMousePointer::Start()
{
	rayCollision_ = CreateTransformComponent<GameEngineCollision>();
	rayCollision_->SetCollisionType(CollisionType::Ray);
	rayCollision_->SetCollisionGroup(eCollisionGroup::MouseRay);

}

void GHMousePointer::Update(float _deltaTime)
{
	updateMouseRay();
	rayCollision_->SetRayData(rayOrigin_, rayDirection_);
}

void GHMousePointer::updateMouseRay()
{
	// ����Ʈ ������ �����ɴϴ�. ���� ����Ʈ�� �ϳ��� ����ϰ�, �ϳ��� ������ ���̱� ������ 1
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
