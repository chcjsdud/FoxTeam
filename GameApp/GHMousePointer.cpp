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

	// ����Ʈ ������ �����ɴϴ�. �ϳ��� ������ ���̱� ������ 1
	UINT numViewport = 1;
	D3D11_VIEWPORT viewports = { 0, };

	// �Էµ� �� ��ŭ ����Ʈ�� �����ɴϴ�.
	// ���� 2��° ���ڰ� NULL �̸�, ù��° ���ڷ� ���� UINT�� ���� ä�����ϴ�.
	// ���� ���� �����Ͷ������� ���ε� �� ����Ʈ �� �Դϴ�.
	//dc->RSGetViewports(&numViewport, nullptr);
	dc->RSGetViewports(&numViewport, &viewports);

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

	// ����Ʈ�� ��Ⱦ�� ����Ͽ� ���� ����� ����Ͽ� ���� �����մϴ�
	float4x4 matProjection = GetLevel()->GetMainCameraActor()->GetProjectionMatrix();

	pointX = pointX / matProjection._11;
	pointY = pointY / matProjection._22;

	// �� ����� ���Լ��� ���մϴ�.
	float4x4 matView = GetLevel()->GetMainCameraActor()->GetViewMatrix();
	matView = matView.InverseReturn();

	float4 direction;
	direction.x = pointX;
	direction.y = pointY;
	direction.z = 1.0f;
	direction.w = 0.0f;

	direction = direction * matView;

}
