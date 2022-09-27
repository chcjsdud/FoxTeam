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

	GameEngineCollision* GetCollision() { return col; }
	ItemBase* GetItem(int _index);
	bool DeleteItem(int _index);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	ItemBoxInfo info;
	GameEngineCollision* col;
	std::string area;

	std::list<ItemBase*> itemList;
};

