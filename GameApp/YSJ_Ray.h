#pragma once
#include <GameEngine/GameEngineComponent.h>

// 분류 : 광선(Ray)
// 용도 : 
// 설명 : 
class GameEngineFBXMesh;
class GameEngineCollision;
class GameEngineFBXRenderer;
class YSJ_Ray : public GameEngineComponent
{
public: // 광선체크
	bool IsPicked(const float4& _MousePos, float4& _PickedPos, GameEngineFBXRenderer* _Mesh = nullptr);
	bool IsColliderPicked(const float4& _MousePos, float4& _PickedPos);
	GameEngineCollision* GetPickCollision(const float4& _MousePos, int _Order);
	bool IsMeshPicked(GameEngineFBXRenderer* _Mesh, const float4& _MousePos, float4& _PickedPos);

protected:

private:
	bool RayAtViewSpace(float _MousePosX, float _MousePosY);
	bool RayAtViewSpace(float4 _MousePos);

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	YSJ_Ray();
	~YSJ_Ray();

protected:		// delete constructer
	YSJ_Ray(const YSJ_Ray& _other) = delete;
	YSJ_Ray(YSJ_Ray&& _other) noexcept = delete;

private:		//delete operator
	YSJ_Ray& operator=(const YSJ_Ray& _other) = delete;
	YSJ_Ray& operator=(const YSJ_Ray&& _other) = delete;

public:
	
protected:

private:
	float4 OriginPos_;				// 원점
	float4 Direction_;				// 카메라가 바라보는 방향
};

