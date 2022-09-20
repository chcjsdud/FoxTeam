#pragma once
#include <GameEngine/GameEngineActor.h>
#include "NavMesh.h"

// Ό³Έν : 
class ItemBoxManager;
class Character : public GameEngineActor
{
public:
	Character();
	~Character();
	Character(const Character& _other) = delete; 
	Character(Character&& _other) noexcept = delete;
	Character& operator=(const Character& _other) = delete;
	Character& operator=(const Character&& _other) = delete;

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
	void CheckCurrentNav();

	void GetItem();

	void CheckItemBox();

protected:
	GameEngineCollision* collision_;

	NavFace* currentNavFace_;
	NavMesh* currentNavMesh_;

	ItemBoxManager* manager_;
};

