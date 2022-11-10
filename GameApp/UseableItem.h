#pragma once
#include "ItemBase.h"

enum class UseableItemType
{
	HP,
	SP
};

// ���� : 
class UseableItem : public ItemBase
{
	friend class ItemBoxManager;
public:
	UseableItem();
	~UseableItem();

	UseableItem(const UseableItem& _other) = delete; 
	UseableItem(UseableItem&& _other) noexcept = delete;
	UseableItem& operator=(const UseableItem& _other) = delete;
	UseableItem& operator=(const UseableItem&& _other) = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	UseableItemType Type;

	float HPRegen_;
	float SPRegen_;
	float RegenTime_;
};

