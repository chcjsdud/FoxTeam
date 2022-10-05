#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineImageRenderer.h>

// Equipment	���
// Useable		�Һ�
// Misc			��Ÿ

class ItemBase : public GameEngineActor
{
	friend class ItemBoxManager;
public:
	ItemBase();
	~ItemBase();

	ItemBase(const ItemBase& _other) = delete; 
	ItemBase(ItemBase&& _other) noexcept = delete;
	ItemBase& operator=(const ItemBase& _other) = delete;
	ItemBase& operator=(const ItemBase&& _other) = delete;

	//ItemBase* CombineItem(ItemBase* _Other);

	void SetItemType(ItemType _type)
	{
		type_ = _type;
	}
	void SetItemTier(ItemTier _tier)
	{
		tier_ = _tier;
	}
	
	ItemType GetItemType()
	{
		return type_;
	}
	ItemTier GetItemTier()
	{
		return tier_;
	}

	void SetImage(const std::string& _imageName, const std::string& _sampler = "");

	// ItemType ���� Copy �� ������ �ٸ� virtual
	ItemBase* Copy();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	ItemType type_;
	ItemTier tier_;
	GameEngineImageRenderer* renderer_;
	bool isMyBuild_;
};

