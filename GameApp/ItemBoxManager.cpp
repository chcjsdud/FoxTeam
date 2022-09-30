#include "PreCompile.h"
#include "ItemBoxManager.h"
#include <GameEngine/GameEngineCollision.h>
#include "MousePointer.h"
#include "Enums.h"
#include "ItemBase.h"
#include "EquipmentItem.h"
#include "UseableItem.h"
#include "MiscItem.h"
#include "ItemBox.h"
#include <GameApp/UI_ItemBox.h>
#include "LumiaLevel.h"

ItemBoxManager::ItemBoxManager()
	: selectBox_(nullptr)
	, itemBoxUI_(nullptr)
	, bOpen_(false)
{

}

ItemBoxManager::~ItemBoxManager()
{

}


void ItemBoxManager::CreateItemBoxInfo(const std::string& _Name)
{
	GameEngineFBXMesh* FBXMesh = GameEngineFBXMeshManager::GetInst().Find(_Name);

	std::vector<FbxMeshSet>& AllMesh = FBXMesh->GetAllMeshMap();
	std::vector<FbxExMeshInfo>& MeshInfos = FBXMesh->GetMeshInfos();

	std::string AreaName = _Name;
	AreaName = GameEngineString::toupper(AreaName);

	if (std::string::npos != AreaName.find(".FBX"))
	{
		AreaName.erase(AreaName.find(".FBX"));
	}

	std::vector<ItemBox*> vecItemBox;
	int ItemBoxIndex = 0;

	for (size_t i = 0; i < AllMesh.size(); i++)
	{
		if (std::string::npos == MeshInfos[i].Name.find("ItemBox") &&
			std::string::npos == MeshInfos[i].Name.find("OBJ"))
		{
			continue;
		}

		ItemBox* Item = GetLevel()->CreateActor<ItemBox>();

		vecItemBox.push_back(Item);

		float4 AllAddVtxPos = float4::ZERO;
		float4 MaxVtxValue = float4::ZERO;

		for (const auto& Vertex : AllMesh[i].Vertexs)
		{
			AllAddVtxPos += Vertex.POSITION;

			if (abs(Vertex.POSITION.x) > abs(MaxVtxValue.x))
			{
				MaxVtxValue.x = Vertex.POSITION.x;
			}

			if (abs(Vertex.POSITION.y) > abs(MaxVtxValue.y))
			{
				MaxVtxValue.y = Vertex.POSITION.y;
			}

			if (abs(Vertex.POSITION.z) > abs(MaxVtxValue.z))
			{
				MaxVtxValue.z = Vertex.POSITION.z;
			}
		}

		AllAddVtxPos = AllAddVtxPos / static_cast<float>(AllMesh[i].Vertexs.size());

		Item->info.BoxType = MeshInfos[i].Name;
		Item->info.Index = ItemBoxIndex++;
		Item->info.Pos = AllAddVtxPos;

		float4 fScale = float4::ZERO;
		fScale.x = abs(MaxVtxValue.x - AllAddVtxPos.x);
		fScale.y = abs(MaxVtxValue.y);
		fScale.z = abs(MaxVtxValue.z - AllAddVtxPos.z);

		Item->info.Scale = fScale;

		Item->col = CreateTransformComponent<GameEngineCollision>(1);
		Item->col->GetTransform()->SetLocalPosition(Item->info.Pos);
		Item->col->GetTransform()->SetLocalScaling(Item->info.Scale);

		Item->area = AreaName;
	}	

	itemBoxs_.insert(std::pair(AreaName, vecItemBox));

	UserSave(AreaName);
}

void ItemBoxManager::BoxSelectUpdate()
{
	GameEngineCollision* col = GetLevelConvert<LumiaLevel>()->GetMousePointer()->GetPickCollision(static_cast<int>(eCollisionGroup::ItemBox));
	//GameEngineCollision* col = GetLevelConvert<SettingItemLevel>()->GetMousePointer()->GetPickCollision(static_cast<int>(eCollisionGroup::ItemBox));

	if (nullptr == col)
	{
		selectBox_ = nullptr;
		return;
	}

	GetLevel()->PushDebugRender(col->GetTransform(), CollisionType::AABBBox3D, float4::BLUE);

	if (false == GameEngineInput::GetInst().Down("LBUTTON"))
	{
		return;
	}

	for (const auto& ItemBox : itemBoxs_)
	{
		for (size_t i = 0; i < ItemBox.second.size(); i++)
		{
			if (col != ItemBox.second[i]->col)
			{
				continue;
			}

			selectBox_ = ItemBox.second[i];
		}
	}
}

void ItemBoxManager::DebugRender()
{
	for (const auto& ItemBox : itemBoxs_)
	{
		for (size_t i = 0; i < ItemBox.second.size(); i++)
		{
			if (selectBox_ == ItemBox.second[i])
			{
				GetLevel()->PushDebugRender(ItemBox.second[i]->col->GetTransform(), CollisionType::AABBBox3D, float4::BLUE);
				continue;
			}

			GetLevel()->PushDebugRender(ItemBox.second[i]->col->GetTransform(), CollisionType::AABBBox3D);
		}
	}
}

void ItemBoxManager::PushRandomItem(const std::string& _area, const std::string& _item, int _amount)
{
	std::string areaUpperName = GameEngineString::toupper(_area);

	std::map<std::string, std::vector<ItemBox*>>::iterator iter = itemBoxs_.find(areaUpperName);

	if (itemBoxs_.end() == iter)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 지역입니다.");
		return;
	}

	ItemBase* findItem = nullptr;

	for (const auto& item : allItemList_)
	{
		if (item->GetName() != _item)
		{
			continue;
		}

		findItem = item;
	}
	
	if (nullptr == findItem)
	{
		GameEngineDebug::MsgBoxError("아이템을 찾지 못했습니다.");
		return;
	}
	
	int randomValue = 0;

	for (size_t i = 0; i < _amount; i++)
	{
		// std::vector<ItemBox*>
		randomValue = randomManager_.RandomInt(0, static_cast<int>(iter->second.size()) - 1);

		std::list<ItemBase*>& curList = iter->second[randomValue]->itemList;

		bool SameItem = false;

		for (const auto& item : curList)
		{
			if (item->GetName() == _item)
			{
				SameItem = true;
				break;
			}
		}

		int Count = 0;

		while (SameItem)
		{
			randomValue = randomManager_.RandomInt(0, static_cast<int>(iter->second.size()) - 1);

			curList = iter->second[randomValue]->itemList;

			bool Check = false;

			// 아이템 최대 갯수
			if (curList.size() >= 6)
			{
				Check = true;
			}
			else
			{
				for (const auto& item : curList)
				{
					if (item->GetName() == _item)
					{
						Check = true;
						break;
					}
				}
			}

			if (false == Check)
			{
				SameItem = false;
				Count;

				int a = 0;
			}

			++Count;
		}

		if (false == SameItem)
		{
			curList.push_back(findItem->Copy());

			int a = 0;
		}
	}

}

void ItemBoxManager::OpenItemBox()
{
	if (true == bOpen_)
	{
		return;
	}

	if (itemBoxUI_ != nullptr)
	{
		//이건호: 아이템박스가 켜져있는 상태에서 다시 박스를 클릭하면 원래 켜져 있던 박스는 없어지도록 했습니다
		itemBoxUI_->Release();
		itemBoxUI_ = nullptr;
	}
	itemBoxUI_ = GetLevel()->CreateActor<UI_ItemBox>();
	itemBoxUI_->RenderOn();
	itemBoxUI_->GetItemBoxInfo(GetSelectBox());

	bOpen_ = true;
}

void ItemBoxManager::CloseItemBox()
{
	if (false == bOpen_)
	{
		return;
	}

	if (itemBoxUI_ != nullptr)
	{
		itemBoxUI_->Release();
		itemBoxUI_ = nullptr;
	}

	bOpen_ = false;
}

ItemBase* ItemBoxManager::GetItemFromItemBox(int _index)
{
	if (nullptr == selectBox_)
	{
		return nullptr;
	}

	return selectBox_->GetItem(_index);
}

void ItemBoxManager::DeleteItemFromItemBox(int _index)
{
	if (nullptr == selectBox_)
	{
		return;
	}

	selectBox_->DeleteItem(_index);

	//이건호 : 기존 UI의 아이템 슬롯에 들어 와 있던 정보들을 싹 비우고 바뀐 박스정보를 넣습니다 
	itemBoxUI_->EmptySlot();
	itemBoxUI_->GetItemBoxInfo(GetSelectBox());
}

void ItemBoxManager::Start()
{
#ifdef USERSAVE
	GameEngineDirectory tempDir;

	tempDir.MoveParent("FoxTeam");
	tempDir.MoveChild("Resources");
	tempDir.MoveChild("FBX");
	tempDir.MoveChild("YSJ");
	tempDir.MoveChild("ItemBox");

	std::vector<GameEngineFile> vecFile = tempDir.GetAllFile(".FBX");

	for (size_t i = 0; i < vecFile.size(); i++)
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(vecFile[i].GetFullPath());
		CreateItemBoxInfo(vecFile[i].FileName());
	}

	GameEngineDebug::MsgBoxError("UserSave 완료!");
#endif
	int a = 0;

	CreateAllItemList();
	CreateAllItemRecipes();
}

void ItemBoxManager::Update(float _DeltaTime)
{
	DebugRender();	

	BoxSelectUpdate();
}

void ItemBoxManager::UserSave(const std::string& _Path)
{
	GameEngineFile NewFile = GameEngineFile(_Path + ".ItemBoxInfo", "wb");

	std::string AreaName = _Path;
	AreaName = GameEngineString::toupper(AreaName);

	std::map<std::string, std::vector<ItemBox*>>::iterator iter = itemBoxs_.find(AreaName);

	if (itemBoxs_.end() == iter)
	{
		GameEngineDebug::MsgBoxError("if (itemBoxs_.end() == itemBoxs_.find(AreaName))");
		return;
	}

	NewFile.Write(AreaName);
	NewFile.Write(static_cast<int>((*iter).second.size()));

	for (size_t i = 0; i < (*iter).second.size(); i++)
	{
		NewFile.Write((*iter).second[i]->info.BoxType);
		NewFile.Write((*iter).second[i]->info.Index);
		NewFile.Write((*iter).second[i]->info.Pos);
		NewFile.Write((*iter).second[i]->info.Scale);
		NewFile.Write((*iter).second[i]->area);
	}
}

void ItemBoxManager::UserLoad(const std::string& _Path)
{
	GameEngineFile NewFile = GameEngineFile(_Path, "rb");

	std::string AreaName;

	NewFile.Read(AreaName);

	int Size = 0;

	NewFile.Read(Size);

	if (0 == Size)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 ItemBoxInfo 파일입니다.");
	}

	std::vector<ItemBox*> vecItemBox;

	itemBoxs_.insert(std::pair(AreaName, vecItemBox));
	
	std::map<std::string, std::vector<ItemBox*>>::iterator iter = itemBoxs_.find(AreaName);

	if (itemBoxs_.end() == iter)
	{
		GameEngineDebug::MsgBoxError("if (itemBoxs_.end() == itemBoxs_.find(AreaName))");
		return;
	}

	(*iter).second.resize(Size);

	for (auto& Data : (*iter).second)
	{
		Data = GetLevel()->CreateActor<ItemBox>();

		NewFile.Read(Data->info.BoxType);
		NewFile.Read(Data->info.Index);
		NewFile.Read(Data->info.Pos);
		NewFile.Read(Data->info.Scale);
		NewFile.Read(Data->area);

		Data->col = CreateTransformComponent<GameEngineCollision>(static_cast<int>(eCollisionGroup::ItemBox));
		Data->col->GetTransform()->SetLocalPosition(Data->info.Pos);
		Data->col->GetTransform()->SetLocalScaling(Data->info.Scale);
		Data->col->SetCollisionType(CollisionType::AABBBox3D);
	}
}

void ItemBoxManager::UserAllLoad(GameEngineDirectory _Dir)
{
	std::vector<GameEngineFile> vecFile = _Dir.GetAllFile(".ItemBoxInfo");

	for (size_t i = 0; i < vecFile.size(); i++)
	{
		UserLoad(vecFile[i].GetFullPath());
	}
	
	itemBoxInfoPath_ = _Dir.GetFullPath();

	//UserLoad_ItemListInfo();
}

void ItemBoxManager::UserSave_ItemListInfo()
{
	GameEngineFile NewFile = GameEngineFile(itemBoxInfoPath_ + "\\ItemList.ItemListInfo", "wb");

	NewFile.Write(static_cast<int>(itemBoxs_.size()));

	for (const auto& ItemBox : itemBoxs_)
	{
		NewFile.Write(ItemBox.first);
		NewFile.Write(static_cast<int>(ItemBox.second.size()));
		for (size_t i = 0; i < ItemBox.second.size(); i++)
		{
			NewFile.Write(static_cast<int>(ItemBox.second[i]->itemList.size()));

			for (const auto& Item : ItemBox.second[i]->itemList)
			{
				NewFile.Write(Item->GetName());
				NewFile.Write(static_cast<int>(Item->type_));
				if (nullptr == Item->renderer_)
				{
					std::string str = "";
					NewFile.Write(str);
				}
				else
				{
					NewFile.Write(Item->renderer_->GetName());
				}
			}
		}
	}
}

void ItemBoxManager::UserLoad_ItemListInfo()
{
	GameEngineFile NewFile = GameEngineFile(itemBoxInfoPath_ + "\\ItemList.ItemListInfo", "rb");

	int mapSize = 0;

	// map 갯수
	NewFile.Read(mapSize);

	for (size_t i = 0; i < mapSize; i++)
	{
		std::string KeyName;
		NewFile.Read(KeyName);

		int vecItemBoxSize = 0;
		NewFile.Read(vecItemBoxSize);

		for (size_t j = 0; j < vecItemBoxSize; j++)
		{
			int ItemListSize = 0;

			NewFile.Read(ItemListSize);

			for (size_t k = 0; k < ItemListSize; k++)
			{
				std::string ItemName;
				NewFile.Read(ItemName);
				int Type = 0;
				NewFile.Read(Type);

				ItemBase* Item = nullptr;
				
				switch (static_cast<ItemType>(Type))
				{
				case ItemType::NONE:
					break;
				case ItemType::EQUIPMENT:
					Item = GetLevel()->CreateActor<EquipmentItem>();
					break;
				case ItemType::USEABLE:
					Item = GetLevel()->CreateActor<UseableItem>();
					break;
				case ItemType::MISC:
					Item = GetLevel()->CreateActor<MiscItem>();
					break;
				default:
					break;
				}

				if (nullptr != Item)
				{
					Item->SetName(ItemName);
					Item->type_ = static_cast<ItemType>(Type);
				}

				std::string ImageName;
				NewFile.Read(ImageName);
				if ("" != ImageName)
				{
					Item->SetImage(ImageName);
				}

				itemBoxs_.find(KeyName)->second[j]->itemList.push_back(Item);
			}
		}
	}
}

