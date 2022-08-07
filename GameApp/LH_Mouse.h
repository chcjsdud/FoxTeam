#pragma once
#include <GameEngine/GameEngineActor.h>
#include "Enums.h"

	// 분류 : 마우스
	// 용도 : UI 상호 및 플레이어 이동
	// 설명 : 
class GameEngineUIRenderer;
class GameEngineFBXMesh;
class GameEngineCollision;
class LH_Mouse : public GameEngineActor
{
private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	LH_Mouse();
	~LH_Mouse();

public:
	bool Mouse_GetCollision(CollisionGroup _CollisionGroup);
	bool Mouse_GetCollisionMesh(GameEngineFBXMesh* _Mesh);
	GameEngineActor* Mouse_GetCollisionActor(CollisionGroup _CollisionGroup);
	//GameEngineActor* Mouse_GetCollisionMesh(GameEngineFBXMesh* _Mesh);

private:

private:
	GameEngineUIRenderer* Renderer_;
	GameEngineCollision* Collider_;

private:
	class LH_Ray;
	LH_Ray* Ray_;

	float4 MousePos_;

public:
	const float4 Mouse_GetMousePos()
	{
		return MousePos_;
	}

private:

	class LH_Ray : public GameEngineActor
	{
	public: // 광선체크
		bool IsPicked(const float4& _MousePos, float4& _PickedPos, CollisionGroup _CollisionGroup);
		bool IsPickedMesh(const float4& _MousePos, float4& _PickedPos, GameEngineFBXMesh* _Mesh);
		GameEngineActor* IsPickedActor(const float4& _MousePos, CollisionGroup _CollisionGroup);
		//GameEngineActor* IsPickedMeshActor(const float4& _MousePos, GameEngineFBXMesh* _Mesh);

	protected:

	private:
		bool RayAtViewSpace(float _MousePosX, float _MousePosY);
		bool RayAtViewSpace(float4 _MousePos);
	private:
		void Start() override;
		void Update(float _DeltaTime) override;

	public:
		LH_Ray();
		~LH_Ray();

	private:
		float4 OriginPos_;				// 원점
		float4 Direction_;				// 카메라가 바라보는 방향

	private:
		LH_Ray(const LH_Ray& _other) = delete;
		LH_Ray(LH_Ray&& _other) = delete;
		LH_Ray& operator=(const LH_Ray& _other) = delete;
		LH_Ray& operator=(const LH_Ray&& _other) = delete;
	};

	LH_Mouse(const LH_Mouse& _other) = delete;
	LH_Mouse(LH_Mouse&& _other) = delete;
	LH_Mouse& operator=(const LH_Mouse& _other) = delete;
	LH_Mouse& operator=(const LH_Mouse&& _other) = delete;
};


