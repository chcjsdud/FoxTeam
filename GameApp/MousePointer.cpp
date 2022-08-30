#include "PreCompile.h"
#include "MousePointer.h"

#include <GameEngine/GameEngineCollision.h>

MousePointer::MousePointer()
	: rayCollision_(nullptr)
	, rayDirection_(0.0f, 0.0f, 1.0f, 1.0f)
{

}

MousePointer::~MousePointer()
{

}

void MousePointer::Start()
{
	rayCollision_ = CreateTransformComponent<GameEngineCollision>();
	rayCollision_->SetCollisionType(CollisionType::Ray);
	rayCollision_->SetCollisionGroup(eCollisionGroup::MouseRay);
}

void MousePointer::Update(float _deltaTime)
{
	updateMouseRay();
	rayCollision_->SetRayData(rayOrigin_, rayDirection_);
}

float4 MousePointer::GetIntersectionYAxisPlane(float _height, float _rayLength)
{
	return DirectX::XMPlaneIntersectLine({ 0.0f, 1.0f, 0.0f, -_height }, rayOrigin_.DirectVector, (rayOrigin_ + rayDirection_ * _rayLength).DirectVector);;
}

void MousePointer::updateMouseRay()
{
	// 뷰포트 개수를 가져옵니다. 뷰포트는 하나만 사용했고, 하나만 가져올 것이기 때문에 1 로 초기화합니다.
	UINT numViewport = 1;
	D3D11_VIEWPORT viewports = { 0, };

	// 입력된 첫 번째 인자 수 만큼 뷰포트를 가져옵니다.
	// 만약 2번째 인자가 NULL 이면, 첫번째 인자로 넣은 UINT에 값이 채워집니다.
	// 채워진 값은 현재 래스터라이저에 바인딩 된 뷰포트 수 입니다.
	//dc->RSGetViewports(&numViewport, nullptr);
	GameEngineDevice::GetContext()->RSGetViewports(&numViewport, &viewports);

	if (viewports.Width == 0.0f || viewports.Height == 0.0f)
	{
		return;
	}

	// 인풋 클래스에서 마우스를 위치를 가져옵니다.
	float4 mousePosition = GameEngineInput::GetInst().GetMousePos();
	float pointX = mousePosition.x;
	float pointY = mousePosition.y;

	// 마우스의 위치는 뷰포트를 벗어날 수 없습니다.
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

	// 마우스 커서 좌표를 -1에서 +1 범위로 바꿉니다. (NDC)
	pointX = ((2.0f * pointX) / viewports.Width) - 1.0f;
	pointY = (((2.0f * pointY) / viewports.Height) - 1.0f) * -1.0f;

	// 뷰포트의 종횡비를 고려하여 투영 행렬을 사용해 점을 조정합니다
	float4x4 matProjection = GetLevel()->GetMainCameraActor()->GetProjectionMatrix();

	pointX = pointX / matProjection._11;
	pointY = pointY / matProjection._22;

	// 뷰 행렬의 역행렬을 구합니다.
	float4x4 inverseViewMatrix = GetLevel()->GetMainCameraActor()->GetViewMatrix().InverseReturn();

	// 뷰 역행렬을 곱해 방향을 알아냅니다.
	rayDirection_.x = pointX;
	rayDirection_.y = pointY;
	rayDirection_.z = 1.0f;
	rayDirection_.w = 0.0f;

	rayDirection_ = rayDirection_ * inverseViewMatrix;
	rayDirection_.Normalize3D();

	// 뷰 역행렬의 4행은 곧 카메라의 위치입니다.
	rayOrigin_ = inverseViewMatrix.vw;
}
