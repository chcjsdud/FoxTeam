#pragma once
#include <GameEngine/GameEngineActor.h>

class ItemBoxInfo
{
public:
	float4 Pos;
	float4 Scale;
};

class GameEngineCollision;
class ItemBox
{
public:
	ItemBoxInfo Info;
	GameEngineCollision* Col;
};

// Ό³Έν : 
class GameEngineFBXRenderer;
class ItemBoxManager : public GameEngineActor
{
public:
	ItemBoxManager();
	~ItemBoxManager();

	ItemBoxManager(const ItemBoxManager& _other) = delete; 
	ItemBoxManager(ItemBoxManager&& _other) noexcept = delete;
	ItemBoxManager& operator=(const ItemBoxManager& _other) = delete;
	ItemBoxManager& operator=(const ItemBoxManager&& _other) = delete;

	void CreateItemBoxInfo(const std::string& _Name);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::vector<ItemBox> ItemBoxs;
};

