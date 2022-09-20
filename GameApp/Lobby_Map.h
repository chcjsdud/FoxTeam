#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineImageRenderer.h>

using namespace std;

class LocationNode : public GameEngineActor
{
	// 접속된 캐릭터 수만큼 활성화되는 클래스
	// 맵에 꽂힐 핀을 렌더러로 가진다.
	// 겹치면??
	friend class Lobby_Map;

public:
	LocationNode();
	~LocationNode();

protected:
	GameEngineImageRenderer* nodeRenderer_;
	Location SelectedLocation;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

public:
	int GetSelectedLocation() { return static_cast<int>(SelectedLocation); }
	GameEngineImageRenderer* GetRenderer() { return nodeRenderer_; }
	void SetRenderer(GameEngineImageRenderer* _renderer) { nodeRenderer_ = _renderer; }
	void ChangeLocation(int _location) { SelectedLocation = static_cast<Location>(_location); }
};

class GameEngineUIRenderer;
class Lobby_Map : public GameEngineActor
{
public:


public:
	Lobby_Map();
	~Lobby_Map();

	int GetSelectLocation(float4 _Position);



	GameEngineCollision* GetCollision() { return textureCollision_; }
	std::vector<LocationNode*> GetNodeList() { return selectAreaNodeList_; }

public:
	GameEngineImageRenderer* fullMapRenderer_;

	GameEngineImageRenderer* areaChoiceMapRenderer_;
	
	std::vector<LocationNode*> selectAreaNodeList_;

	std::vector<GameEngineUIRenderer*> capacityRendererList_;

	GameEngineCollision* textureCollision_;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	Lobby_Map(const Lobby_Map& _Other) = delete;
	Lobby_Map(Lobby_Map&& _Other) noexcept = delete;
	Lobby_Map& operator=(const Lobby_Map& _Other) = delete;
	Lobby_Map& operator=(Lobby_Map&& _Other) noexcept = delete;
};

