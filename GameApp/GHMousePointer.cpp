#include "PreCompile.h"
#include "GHMousePointer.h"

GHMousePointer::GHMousePointer()
{

}

GHMousePointer::~GHMousePointer()
{

}

void GHMousePointer::Start()
{
}

void GHMousePointer::Update(float _deltaTime)
{
	ID3D11DeviceContext* dc = GameEngineDevice::GetContext();

	// 뷰포트 개수를 가져옵니다. 하나만 가져올 것이기 때문에 1
	UINT numViewport = 1;
	D3D11_VIEWPORT viewports = { 0, };

	// 입력된 수 만큼 뷰포트를 가져옵니다.
	// 만약 2번째 인자가 NULL 이면, 첫번째 인자로 넣은 UINT에 값이 채워집니다.
	// 값은 현재 래스터라이저에 바인딩 된 뷰포트 수 입니다.
	//dc->RSGetViewports(&numViewport, nullptr);
	dc->RSGetViewports(&numViewport, &viewports);

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

	// 뷰포트의 종횡비를 고려하여 투영 행렬을 사용하여 점을 조정합니다
	float4x4 matProjection = GetLevel()->GetMainCameraActor()->GetProjectionMatrix();

	pointX = pointX / matProjection._11;
	pointY = pointY / matProjection._22;

	// 뷰 행렬의 역함수를 구합니다.
	float4x4 matView = GetLevel()->GetMainCameraActor()->GetViewMatrix();
	matView = matView.InverseReturn();

	float4 direction;
	direction.x = pointX;
	direction.y = pointY;
	direction.z = 1.0f;
	direction.w = 0.0f;

	direction = direction * matView;

}
