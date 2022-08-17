#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineImageRenderer.h>

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

	void SetItemType(ItemType _Type)
	{
		Type = _Type;
	}
	void SetImage(const std::string& _ImageName, const std::string& _Sampler = "");

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	ItemType Type;
	GameEngineImageRenderer* Renderer;
};

