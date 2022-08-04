#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 광선(Ray)
// 용도 : 
// 설명 : 
class GameEngineCollision;
class SJH_Ray : public GameEngineActor
{
public:
	SJH_Ray* RayAtViewSpace(float _MousePosX, float _MousePosY);
	SJH_Ray* RayAtViewSpace(float4 _MousePos);

public: // 
	bool IsPicked(float4& _PickedPos);

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
	float4 OriginPos_;				// 원점
	float4 Direction_;				// 카메라가 바라보는 방향
};

