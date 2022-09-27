#pragma once
#include "ItemBase.h"

// Ό³Έν : 
class EquipmentItem : public ItemBase
{
public:
	EquipmentItem();
	~EquipmentItem();

	EquipmentItem(const EquipmentItem& _other) = delete; 
	EquipmentItem(EquipmentItem&& _other) noexcept = delete;
	EquipmentItem& operator=(const EquipmentItem& _other) = delete;
	EquipmentItem& operator=(const EquipmentItem&& _other) = delete;

	void SetEquipType(EquipmentType _type)
	{
		type_ = _type;
	}

	EquipmentType GetEquipType()
	{
		return type_;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	EquipmentType type_;
};

