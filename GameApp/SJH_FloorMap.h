#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngineBase/GameEnginePathFind.h>

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

protected:

private:
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
	GameEngineFBXRenderer* FloorMap_;

private:
	std::vector<SJH_NaviCell*> NavigationCellInfos_;			// NavigationCellInfos_[FaceNumber]

private:
	//std::map<NavigationObjectType, std::vector<GameEnginePathFind*>> Navigation_;	// ������ ���ͺ� �ش� ���� �̵���� ����
};

