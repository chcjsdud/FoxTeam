#pragma once
#include <GameEngine/GameEngineActor.h>

#include "SJH_PathFinder.h"

// �з� : 
// �뵵 : 
// ���� : 
class GameEngineCollision;
class GameEngineFBXMesh;
class GameEngineFBXRenderer;
class SJH_NaviCell;
class SJH_FloorMap : public GameEngineActor
{
public:
	static SJH_FloorMap* FloorMap;

public:
	GameEngineFBXMesh* GetFloorMapMesh();
	GameEngineFBXRenderer* GetFloorMapRenderer();

public:
	SJH_NaviCell* GetNaviCellInfo(const float4& _Vertex0, const float4& _Vertex1, const float4& _Vertex2);

public:
	SJH_NaviCell* SearchCurrentPosToNaviCell(const float4& _Position);
	bool MoveFacePath(const float4& _StartPos, SJH_NaviCell* _StartCell, SJH_NaviCell* _TargetCell, std::list<SJH_NaviCell*>& _MovePath);

protected:

private:
	void CreateAllNaviCellInfo();
	void FindAdjacentFaces();

private: // A*�� �˾Ƴ� ��ο��� ������ ������θ� �˾Ƴ��� �Լ�
	bool StupidFunnelAlgorithm(const float4& _StartPos, std::list<SJH_NaviCell*>& _MovePath);

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	SJH_FloorMap();
	~SJH_FloorMap();

protected:
	SJH_FloorMap(const SJH_FloorMap& _other) = delete;
	SJH_FloorMap(SJH_FloorMap&& _other) noexcept = delete;

private:
	SJH_FloorMap& operator=(const SJH_FloorMap& _other) = delete;
	SJH_FloorMap& operator=(const SJH_FloorMap&& _other) = delete;

public:

protected:

private:
	GameEngineFBXRenderer* NaviMeshRenderer_;					// NaviMesh(�������� �׺���̼�)
	GameEngineFBXRenderer* NaviColMeshRenderer_;				// NaviMesh Col(�� or ..)

private:
	std::vector<SJH_NaviCell*> NavigationCellInfos_;			// NavigationCellInfos_[FaceNumber]

private:
	SJH_PathFinder* PathFinder_;								// A* ���Ž��
};

