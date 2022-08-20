#pragma once

#include <GameEngine/GameEngineActor.h>
#include "GHNavMesh.h"
#include "GHNode.h"

#include <queue>

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

	std::vector<float4> FindPath(const float4& _startPosition, const float4& _endPosition);

private:
	bool checkNavMeshAdjacency(const GHNavMesh& _left, const GHNavMesh& _right);

	void makeAStarNode(float _intervalX, float _intervalZ);
	void checkASterNodeObstacle();
	void updateAStarNodeVertexInfo();

private:
	// For Ground Check
	GameEngineFBXRenderer* navMeshRenderer_;
	std::vector<GHNavMesh> navMeshes_;

	// For A*
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
};

