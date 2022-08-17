#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineRenderer.h>
#include "Item.h"

// ���� : 

// Equipment	���
// Useable		�Һ�
// Misc			��Ÿ
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
	GameEngineRenderer* Renderer;
};

