#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineUIRenderer.h>
#include "Item.h"

// Ό³Έν : 
class ItemBase : public GameEngineActor
{
public:
	ItemBase();
	~ItemBase();

	ItemBase(const ItemBase& _other) = delete; 
	ItemBase(ItemBase&& _other) noexcept = delete;
	ItemBase& operator=(const ItemBase& _other) = delete;
	ItemBase& operator=(const ItemBase&& _other) = delete;

	//ItemBase* CombineItem(ItemBase* _Other);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	ItemType Type;
	GameEngineUIRenderer* Renderer;
};

