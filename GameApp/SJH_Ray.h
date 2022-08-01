#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 광선
// 용도 : 카메라가 바라보는 뱡향으로 발사하는 방향벡터
// 설명 : 
class GameEngineCollision;
class SJH_Ray : public GameEngineActor
{
public: // 해당 마우스 클릭지점과 충돌하는 객체를 찾아내 그 객체의 위치값을 반환
	float4 RayAtViewSpace(float _MousePosX, float _MousePosY);
	float4 RayAtViewSpace(float4 _MousePos);

public: // 
	bool IsPicked(GameEngineCollision* _Collider);

protected:

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	SJH_Ray();
	~SJH_Ray();

protected:		// delete constructer
	SJH_Ray(const SJH_Ray& _other) = delete;
	SJH_Ray(SJH_Ray&& _other) noexcept = delete;

private:		//delete operator
	SJH_Ray& operator=(const SJH_Ray& _other) = delete;
	SJH_Ray& operator=(const SJH_Ray&& _other) = delete;

public:
	
protected:

private:
	float4 OriginPos_;				// 마우스 클릭좌표
	float4 Direction_;				// 카메라가 바라보는 방향
};

