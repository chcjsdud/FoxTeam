#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngineBase/GameEnginePathFind.h>

// 분류 : 
// 용도 : 
// 설명 : 
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
	bool MoveFacePath(const SJH_NaviCell* _StartCell, const SJH_NaviCell* _TargetCell, std::vector<SJH_NaviCell*>& _MovePath);

protected:

private:
	void CreateAllNaviCellInfo();
	void FindAdjacentFaces();

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
	GameEngineFBXRenderer* NaviMeshRenderer_;					// NaviMesh(실질적인 네비게이션)
	GameEngineFBXRenderer* NaviColMeshRenderer_;				// NaviMesh Col(벽 or ..)

private:
	std::vector<SJH_NaviCell*> NavigationCellInfos_;			// NavigationCellInfos_[FaceNumber]

private:
	//std::map<NavigationObjectType, std::vector<GameEnginePathFind*>> Navigation_;	// 각각의 액터별 해당 맵의 이동경로 관리
};

