#pragma once

#include <GameEngine/GameEngineActor.h>
#include "GHNavMesh.h"

class GameEngineFBXRenderer;
class GHMap : public GameEngineActor
{
public:
	GHMap(); 
	~GHMap();
	GHMap(const GHMap& _other) = delete; 
	GHMap(GHMap&& _other) = delete; 
	GHMap& operator=(const GHMap& _other) = delete;
	GHMap& operator=(const GHMap&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

public:
	GHNavMesh* GetCurrentNavMesh(const float4& _position);

	bool IsIntersectionMesh(const GHNavMesh& _mesh, const float4& _position);
	GHNavMesh* FindAdjacentMeshIntersect(const GHNavMesh& _currentMesh, const float4& _position);

private:
	bool checkNavMeshAdjacency(const GHNavMesh& _left, const GHNavMesh& _right);

private:
	GameEngineFBXRenderer* navMeshRenderer_;

	std::vector<GHNavMesh> navMeshes_;
};

