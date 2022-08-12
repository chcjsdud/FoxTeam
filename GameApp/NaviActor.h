#pragma once
#include <GameEngine/GameEngineActor.h>
#include "NaviMesh.h"

// Ό³Έν : 
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

	Navi* GetCurrentNavi()
	{
		return CurrentNavi_;
	}

	void SetCurrentNavi(Navi* _Navi)
	{
		CurrentNavi_ = _Navi;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	Navi* CurrentNavi_;
	NaviMesh* NaviMesh_;
};

