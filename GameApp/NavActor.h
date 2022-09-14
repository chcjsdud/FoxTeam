#pragma once
#include <GameEngine/GameEngineActor.h>
#include "NavMesh.h"

// ���� : 
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
		return CurrentNavi_;
	}

	void SetCurrentNavi(NavFace* _Navi)
	{
		CurrentNavi_ = _Navi;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	NavFace* CurrentNavi_;
	NavMesh* NaviMesh_;
};

