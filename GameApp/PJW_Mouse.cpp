#include "PreCompile.h"
#include "PJW_Mouse.h"
#include <GameEngine/GameEngineLevel.h>
#include <GameEngine/GameEngineCollision.h>
#include "PJW_Level.h"

PJW_Mouse::PJW_Mouse()
	: originPos_(float4::ZERO), direction_(float4::ZERO)
{

}

PJW_Mouse::~PJW_Mouse()
{

}

bool PJW_Mouse::isPicked(const float4& _mousePos, float4& _pickedPos, GameEngineFBXRenderer* _mesh)
{
	if (nullptr == _mesh)
	{
		// �޽ð� null �̸�... �浹ü ��ŷ����
		return isCollPicked(_mousePos, _pickedPos);
	}
	else
	{
		return isMeshPicked(_mesh, _mousePos, _pickedPos);
	}

	// �̵����� �ƴϸ�..
	return false;
}

bool PJW_Mouse::isCollPicked(const float4& _mousePos, float4& _pickedPos, GameEngineCollision* _collision)
{
	if (false == Ray2Dto3D(_mousePos))
	{
		// ���콺 ��ġ ��ȯ
		// �����ϸ� false ����
		return false; 
	}

	// �ش� ���� ��� �ݸ��� ����Ʈ�� �����´�.
	std::map<int, std::list<GameEngineCollision*>> allCollisions = GetLevel()->GetAllCollision();


	// ������ ����� ������ �����ϴ� �浹ü�� Ž���Ѵ�.
	int groupCount = static_cast<int>(allCollisions.size());

	for (int group = 0; group < groupCount; group++)
	{
		std::list<GameEngineCollision*>::iterator startIter = allCollisions[group].begin();
		std::list<GameEngineCollision*>::iterator endIter = allCollisions[group].end();

		for (;startIter != endIter; startIter++)
		{
			float distance = 0.0f;
			// �Ÿ� �ʱ�ȭ

			if (true == (*startIter)->BoundingToRayCollision((*startIter)->GetCollisionType(), originPos_, direction_, distance))
			{
				// �� ������ ������ �������� �� ������ �ش� �ݸ����� ������?
				// �׷��ٸ� ������ ������ ��ǥ�� ��ȯ
				_pickedPos = originPos_ + (direction_ * distance);
				// ���� ���Ϳ� + ����*�Ÿ��� ���� ��ġ ���͸� ���ؼ� ã�Ƴ���.
				return true;

			}

		}
	}

	return false;
}


bool PJW_Mouse::isMeshPicked(GameEngineFBXRenderer* _mesh, const float4& _mousePos, float4& _pickedPos)
{
	if (false == Ray2Dto3D(_mousePos))
	{
		return false;
	}

	if (nullptr == _mesh)
	{
		return false;
	}

	float distance = 0.0f;

	// �����۵� �޽��� ���ͼ����� �̿�����.
	if (true == _mesh->CheckIntersects(originPos_, direction_, distance))
	{
		_pickedPos = originPos_ + (direction_ * distance);
		return true;
	}

	return false;
}



void PJW_Mouse::Start()
{
}

void PJW_Mouse::Update(float _deltaTime)
{
}

bool PJW_Mouse::Ray2Dto3D(float _mouseX, float _mouseY)
{
	// 2������ ����� ��ǥ�� �Է¹޾�
	// ���忡���� ���콺 ��ġ�� ��ȯ���ݴϴ�.
	// �ٸ� ���ϰ��� ����/���з� ���ϵǸ�,
	// ��ȯ�� ���� ���� ��� ������ originPos �� direction �� �����մϴ�.

	float4 originPos = { 0.f,0.f,0.f,0.f };
	// �� ��Ʈ �������� (�츮�� �� ��Ʈ�� �Ѱ��� ����)
	UINT viewPortNum = 1;
	D3D11_VIEWPORT viewPort = {};

	GameEngineDevice::GetInst().GetContext()->RSGetViewports(&viewPortNum, &viewPort);
	// ���ؽ�Ʈ�� ���� ����ϴ� �� ��Ʈ�� �����ͼ� ������ �����Ѵ�.

	// 1. ����Ʈ -> ������ȯ
	float PointX = ((2.0f * _mouseX) / viewPort.Width) - 1.0f;
	float PointY = (((2.0f * _mouseY) / viewPort.Height) - 1.0f) * -1.0f;
	float PointZ = 1.0f;
	// ���콺�� 2D ��ǥ�� -1 ~ 1 ���̷� �ִ´�

	// 2. ���� -> �� ��ȯ
	// �������� �� ��ȯ�� 
	float4x4 tempProjectionMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().Projection_;
	// ������� ��������
	PointX = PointX / tempProjectionMat._11;
	PointY = PointY / tempProjectionMat._22;
	// �� �κ��� �� �𸣰���... ��?

	// 3. �� -> ���庯ȯ
	// ī�޶��� ������� ���ϸ�, ����� �ٷ� �ٲ��! (��� ���忡�� ī�޶��� ������ ������ ���̱� ������)
	float4x4 tempCameraMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().View_;
	// InverseReturn() ����� ī�޶��� ������� ���Ѵ�
	float4x4 tempCameraInverseMat = tempCameraMat.InverseReturn();

	// �̰� �� �������, ��� �����鿡�� ���� �ֱ�...
	direction_.x = PointX;
	direction_.y = PointY;
	direction_.z = 1.0f;
	direction_.w = 0.0f;

	direction_ = direction_ * tempCameraInverseMat;
	direction_.Normalize3D();
	// ���������� ������ 2D->3D ȭ�� ���͸� ����ȭ�� ���� ���ͷ� �����!
	direction_.Normalize3D();

	// ���������� ���� pos ���� �־�����
	// �� ī�޶��� ������� 4���� ������ ��ġ����?
	originPos_.x = tempCameraInverseMat._41;
	originPos_.y = tempCameraInverseMat._42;
	originPos_.z = tempCameraInverseMat._43;
	originPos_.w = 0.0f;
	
	return true;
}


bool PJW_Mouse::Ray2Dto3D(float4 _mousePos)
{
	return Ray2Dto3D(_mousePos.x, _mousePos.y);
}

