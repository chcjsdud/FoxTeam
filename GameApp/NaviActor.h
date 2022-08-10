#pragma once
#include <GameEngine/GameEngineActor.h>

// Ό³Έν : 
class Navi;
class NaviMesh;
class NaviActor : public GameEngineActor
{
public:
	NaviActor();
	~NaviActor();
	NaviActor(const NaviActor& _other) = delete; 
	NaviActor(NaviActor&& _other) noexcept = delete;
	NaviActor& operator=(const NaviActor& _other) = delete;
	NaviActor& operator=(const NaviActor&& _other) = delete;

	void SetNaviMesh(NaviMesh* _NaviMesh);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	Navi* CurrentNavi_;
	NaviMesh* NaviMesh_;
};

