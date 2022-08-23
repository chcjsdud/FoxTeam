#pragma once
#include <GameEngine/GameEngineActor.h>

class ItemBoxInfo
{
public:
	std::string BoxType;
	int Index;

	float4 Pos;
	float4 Scale;

	ItemBoxInfo()
		: Index(0)
	{}
};

// Ό³Έν : 
class YSJ_Ray;
class ItemBase;
class GameEngineCollision;
class ItemBox : public GameEngineActor
{
	friend class ItemBoxManager;
	friend class ItemBoxWindow;
public:
	ItemBox();
	~ItemBox();
	ItemBox(const ItemBox& _other) = delete; 
	ItemBox(ItemBox&& _other) noexcept = delete;
	ItemBox& operator=(const ItemBox& _other) = delete;
	ItemBox& operator=(const ItemBox&& _other) = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	ItemBoxInfo Info;
	GameEngineCollision* Col;
	std::string Area;

	std::list<ItemBase*> ItemList;
};

