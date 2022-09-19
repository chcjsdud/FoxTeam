#pragma once
#include <GameEngine/GameEngineActor.h>
#include "NavMesh.h"

// Ό³Έν : 
class NavActor : public GameEngineActor
{
public:
	NavActor();
	~NavActor();
	NavActor(const NavActor& _other) = delete; 
	NavActor(NavActor&& _other) noexcept = delete;
	NavActor& operator=(const NavActor& _other) = delete;
	NavActor& operator=(const NavActor&& _other) = delete;

	void SetNaviMesh(NavMesh* _NaviMesh);

	NavFace* GetCurrentNavi()
	{
		return currentNavFace_;
	}

	void SetCurrentNavi(NavFace* _Navi)
	{
		currentNavFace_ = _Navi;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void OpenItemBox();
	void GetItem();
	void CloseItemBox();


protected:
	GameEngineCollision* Collision_;

	NavFace* currentNavFace_;
	NavMesh* currentNavMesh_;
};

