#pragma once
#include <GameEngine/GameEngineActor.h>

//#define USERSAVE

class ItemBoxInfo
{
public:
	std::string Name;
	int Index;

	float4 Pos;
	float4 Scale;

	ItemBoxInfo()
		: Index(0)
	{}
};

class GameEngineCollision;
class YSJ_Ray;
class ItemBox
{
public:
	ItemBoxInfo Info;
	GameEngineCollision* Col;

	ItemBox()
		: Col(nullptr)
	{}
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

	void UserSave(const std::string& _Path);
	void UserLoad(const std::string& _Path);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void CreateItemBoxInfo(const std::string& _Name);
	void BoxSelectUpdate();

private:
	std::map<std::string, std::vector<ItemBox>> ItemBoxs;
	ItemBox* SelectBox;
};

