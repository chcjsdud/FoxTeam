#pragma once
#include <GameEngine/GameEngineActor.h>

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

protected:

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
	GameEngineFBXRenderer* FloorMap_;

private:
	std::vector<SJH_NaviCell*> NavigationCellInfos_;	
};

