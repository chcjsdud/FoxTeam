#pragma once
#include <GameEngine/GameEngineActor.h>

//#define USERSAVE

// ���� : 
class ItemBox;
class UI_ItemBox;
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
	void UserAllLoad(GameEngineDirectory _Dir);

	void UserSave_ItemListInfo();
	void UserLoad_ItemListInfo();

	ItemBox* GetSelectBox()
	{
		return SelectBox;
	}

	UI_ItemBox* GetItemBoxUI()
	{
		return ItemBoxUI_;
	}


protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void CreateItemBoxInfo(const std::string& _Name);
	void BoxSelectUpdate();

private:
	std::string ItemBoxInfoPath;
	std::map<std::string, std::vector<ItemBox*>> ItemBoxs;
	ItemBox* SelectBox;

	//�̰�ȣ UI������ �ڽ� �۾�
private:
	UI_ItemBox* ItemBoxUI_;
};

