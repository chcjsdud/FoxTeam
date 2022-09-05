#pragma once

#include <GameEngine/GameEngineActor.h>
#include "GHNavMesh.h"
#include "GHNode.h"

#include <queue>

class GameEngineFBXRenderer;
class LumiaMap : public GameEngineActor
{
public:
	LumiaMap();
	~LumiaMap();
	LumiaMap(const LumiaMap& _other) = delete;
	LumiaMap(LumiaMap&& _other) = delete;
	LumiaMap& operator=(const LumiaMap& _other) = delete;
	LumiaMap& operator=(const LumiaMap&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

public:
	GHNavMesh* GetCurrentNavMesh(const float4& _position);

	bool IsIntersectionMesh(const GHNavMesh& _mesh, const float4& _position);
	GHNavMesh* FindAdjacentMeshIntersect(const GHNavMesh& _currentMesh, const float4& _position);

	std::vector<float4> FindPath(const float4& _startPosition, const float4& _endPosition);

private:
	bool checkNavMeshAdjacency(const GHNavMesh& _left, const GHNavMesh& _right);

	void makeAStarNode(float _intervalX, float _intervalZ);
	void checkASterNodeObstacle();
	void updateAStarNodeVertexInfo();

private:
	// Ground Check
	std::vector<GHNavMesh> navMeshes_;

	// A*
	std::vector<std::vector<GHNode>> allNodes_;
	std::vector<GameEngineVertex> tileVertices_;
	std::vector<UINT> tileIndices_;
	GameEngineVertexBuffer* tileVertexBuffer_;
	GameEngineIndexBuffer* tileIndexBuffer_;

	float intervalX_;
	float intervalZ_;
	float gridStartX_;
	float gridStartZ_;
	int gridZCount_;
	int gridXCount_;

	// Rendering Object
	GameEngineFBXRenderer* navMeshRenderer_;
	GameEngineFBXRenderer* downTownRenderer_;
};
