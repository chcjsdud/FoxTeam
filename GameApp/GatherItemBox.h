#pragma once
#include <GameEngine/GameEngineActor.h>
#include "ItemBox.h"

// Ό³Έν : 
class ItemBase;
class GameEngineCollision;
class GatherItemBox : public GameEngineActor
{
	friend class ItemBoxManager;
	friend class ItemBoxWindow;
public:
	GatherItemBox();
	~GatherItemBox();
	GatherItemBox(const GatherItemBox& _other) = delete;
	GatherItemBox(GatherItemBox&& _other) noexcept = delete;
	GatherItemBox& operator=(const GatherItemBox& _other) = delete;
	GatherItemBox& operator=(const GatherItemBox&& _other) = delete;

	GameEngineCollision* GetCollision() { return col; }
	//ItemBase* GetItem();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	ItemBoxInfo info;
	GameEngineCollision* col;
	std::string area;

	ItemBase* item;
};

