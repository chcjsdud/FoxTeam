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
		// 메시가 null 이면... 충돌체 피킹으로
		return isCollPicked(_mousePos, _pickedPos);
	}
	else
	{
		return isMeshPicked(_mesh, _mousePos, _pickedPos);
	}

	// 이도저도 아니면..
	return false;
}

bool PJW_Mouse::isCollPicked(const float4& _mousePos, float4& _pickedPos, GameEngineCollision* _collision)
{
	if (false == Ray2Dto3D(_mousePos))
	{
		// 마우스 위치 변환
		// 실패하면 false 리턴
		return false; 
	}

	// 해당 씬의 모든 콜리젼 리스트를 가져온다.
	std::map<int, std::list<GameEngineCollision*>> allCollisions = GetLevel()->GetAllCollision();


	// 위에서 계산한 광선과 교차하는 충돌체를 탐색한다.
	int groupCount = static_cast<int>(allCollisions.size());

	for (int group = 0; group < groupCount; group++)
	{
		std::list<GameEngineCollision*>::iterator startIter = allCollisions[group].begin();
		std::list<GameEngineCollision*>::iterator endIter = allCollisions[group].end();

		for (;startIter != endIter; startIter++)
		{
			float distance = 0.0f;
			// 거리 초기화

			if (true == (*startIter)->BoundingToRayCollision((*startIter)->GetCollisionType(), originPos_, direction_, distance))
			{
				// 그 레이의 원점과 방향으로 쭉 나가서 해당 콜리젼과 만난다?
				// 그렇다면 교차한 지점의 좌표를 반환
				_pickedPos = originPos_ + (direction_ * distance);
				// 원점 벡터에 + 방향*거리로 실제 위치 벡터를 더해서 찾아낸다.
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

	// 기제작된 메쉬의 인터섹츠를 이용하자.
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
	// 2차원의 모니터 좌표를 입력받아
	// 월드에서의 마우스 위치로 변환해줍니다.
	// 다만 리턴값은 성공/실패로 리턴되며,
	// 변환된 값은 내부 멤버 변수인 originPos 와 direction 을 갱신합니다.

	float4 originPos = { 0.f,0.f,0.f,0.f };
	// 뷰 포트 가져오기 (우리는 뷰 포트를 한개만 쓴다)
	UINT viewPortNum = 1;
	D3D11_VIEWPORT viewPort = {};

	GameEngineDevice::GetInst().GetContext()->RSGetViewports(&viewPortNum, &viewPort);
	// 컨텍스트에 현재 사용하는 뷰 포트를 가져와서 변수에 저장한다.

	// 1. 뷰포트 -> 투영변환
	float PointX = ((2.0f * _mouseX) / viewPort.Width) - 1.0f;
	float PointY = (((2.0f * _mouseY) / viewPort.Height) - 1.0f) * -1.0f;
	float PointZ = 1.0f;
	// 마우스의 2D 좌표를 -1 ~ 1 사이로 넣는다

	// 2. 투영 -> 뷰 변환
	// 투영에서 뷰 변환은 
	float4x4 tempProjectionMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().Projection_;
	// 투영행렬 가져오기
	PointX = PointX / tempProjectionMat._11;
	PointY = PointY / tempProjectionMat._22;
	// 이 부분은 잘 모르겠음... 왜?

	// 3. 뷰 -> 월드변환
	// 카메라의 역행렬을 곱하면, 월드로 바로 바뀐다! (뷰는 월드에서 카메라의 시점을 대입한 것이기 때문에)
	float4x4 tempCameraMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().View_;
	// InverseReturn() 사용해 카메라의 역행렬을 구한다
	float4x4 tempCameraInverseMat = tempCameraMat.InverseReturn();

	// 이게 이 역행렬을, 멤버 변수들에게 곱해 주기...
	direction_.x = PointX;
	direction_.y = PointY;
	direction_.z = 1.0f;
	direction_.w = 0.0f;

	direction_ = direction_ * tempCameraInverseMat;
	direction_.Normalize3D();
	// 마지막으로 구해진 2D->3D 화된 벡터를 정규화해 방향 벡터로 만든다!
	direction_.Normalize3D();

	// 마지막으로 원점 pos 역시 넣어주자
	// 왜 카메라의 역행렬의 4행이 원점의 위치였나?
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

