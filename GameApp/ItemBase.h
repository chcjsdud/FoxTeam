#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineImageRenderer.h>

// 설명 : 

// Equipment	장비
// Useable		소비
// Misc			기타

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

