#pragma once
#include "ItemBase.h"

// Ό³Έν : 
class UseableItem : public ItemBase
{
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

};

