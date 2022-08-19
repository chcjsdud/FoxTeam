#include "PreCompile.h"
#include "ItemBoxManager.h"
#include <GameEngine/GameEngineCollision.h>
#include "YSJ_Mouse.h"
#include "Enums.h"
#include "ItemBase.h"
#include "EquipmentItem.h"
#include "UseableItem.h"
#include "MiscItem.h"
#include <GameApp/UI_ItemBox.h>

ItemBoxManager::ItemBoxManager()
	: SelectBox(nullptr)
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

	std::vector<ItemBox> vecItemBox;
	int ItemBoxIndex = 0;

	for (size_t i = 0; i < AllMesh.size(); i++)
	{
		//if (std::string::npos == MeshInfos[i].Name.find("ItemBox"))
		//{
		//	continue;
		//}

		ItemBox& Item = vecItemBox.emplace_back();

		float4 AllAddVtxPos = float4::ZERO;

		for (auto& Vertex : AllMesh[i].Vertexs)
		{
			AllAddVtxPos += Vertex.POSITION;
		}

		AllAddVtxPos = AllAddVtxPos / static_cast<float>(AllMesh[i].Vertexs.size());

		Item.Info.BoxType = MeshInfos[i].Name;
		Item.Info.Index = ItemBoxIndex++;
		Item.Info.Pos = AllAddVtxPos;
		Item.Info.Scale = { 2.0f, 2.0f, 2.0f };

		Item.Col = CreateTransformComponent<GameEngineCollision>(1);
		Item.Col->GetTransform()->SetLocalPosition(Item.Info.Pos);
		Item.Col->GetTransform()->SetLocalScaling(Item.Info.Scale);

		Item.Area = AreaName;
	}	

	ItemBoxs.insert(std::pair(AreaName, vecItemBox));

	UserSave(AreaName);
}

void ItemBoxManager::BoxSelectUpdate()
{
	GameEngineCollision* Col = YSJ_Mouse::MainMouse->GetPickCollision(GameEngineInput::GetInst().GetMousePos(), static_cast<int>(CollisionGroup::ItemBox));

	if (nullptr != Col)
	{
		GetLevel()->PushDebugRender(Col->GetTransform(), CollisionType::AABBBox3D, float4::BLUE);

		if (GameEngineInput::GetInst().Down("LBUTTON"))
		{
			for (auto& ItemBox : ItemBoxs)
			{
				for (size_t i = 0; i < ItemBox.second.size(); i++)
				{
					if (Col == ItemBox.second[i].Col)
					{
						if (ItemBoxUI_ != nullptr)
						{
							//이건호: 아이템박스가 켜져있는 상태에서 다시 박스를 클릭하면 원래 켜져 있던 박스는 없어지도록 했습니다
							ItemBoxUI_->Release();
							ItemBoxUI_ = nullptr;
						}
						ItemBoxUI_ = GetLevel()->CreateActor<UI_ItemBox>();
						ItemBoxUI_->RenderOn();
						SelectBox = &ItemBox.second[i];
					} 
				}
			}
		}
	}
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

#endif
	int a = 0;
}

void ItemBoxManager::Update(float _DeltaTime)
{
	for (auto& ItemBox : ItemBoxs)
	{
		for (size_t i = 0; i < ItemBox.second.size(); i++)
		{
			if (SelectBox == &ItemBox.second[i])
			{
				GetLevel()->PushDebugRender(ItemBox.second[i].Col->GetTransform(), CollisionType::AABBBox3D, float4::BLUE);
				continue;
			}

			GetLevel()->PushDebugRender(ItemBox.second[i].Col->GetTransform(), CollisionType::AABBBox3D);
		}
	}

	BoxSelectUpdate();
}

void ItemBoxManager::UserSave(const std::string& _Path)
{
	GameEngineFile NewFile = GameEngineFile(_Path + ".ItemBoxInfo", "wb");

	std::string AreaName = _Path;
	AreaName = GameEngineString::toupper(AreaName);

	std::map<std::string, std::vector<ItemBox>>::iterator iter = ItemBoxs.find(AreaName);

	if (ItemBoxs.end() == iter)
	{
		GameEngineDebug::MsgBoxError("if (ItemBoxs.end() == ItemBoxs.find(AreaName))");
		return;
	}

	NewFile.Write(AreaName);
	NewFile.Write(static_cast<int>((*iter).second.size()));

	for (size_t i = 0; i < (*iter).second.size(); i++)
	{
		NewFile.Write((*iter).second[i].Info.BoxType);
		NewFile.Write((*iter).second[i].Info.Index);
		NewFile.Write((*iter).second[i].Info.Pos);
		NewFile.Write((*iter).second[i].Info.Scale);
		NewFile.Write((*iter).second[i].Area);
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

	std::vector<ItemBox> vecItemBox;

	ItemBoxs.insert(std::pair(AreaName, vecItemBox));
	
	std::map<std::string, std::vector<ItemBox>>::iterator iter = ItemBoxs.find(AreaName);

	if (ItemBoxs.end() == iter)
	{
		GameEngineDebug::MsgBoxError("if (ItemBoxs.end() == ItemBoxs.find(AreaName))");
		return;
	}

	(*iter).second.resize(Size);

	for (auto& Data : (*iter).second)
	{
		NewFile.Read(Data.Info.BoxType);
		NewFile.Read(Data.Info.Index);
		NewFile.Read(Data.Info.Pos);
		NewFile.Read(Data.Info.Scale);
		NewFile.Read(Data.Area);

		Data.Col = CreateTransformComponent<GameEngineCollision>(static_cast<int>(CollisionGroup::ItemBox));
		Data.Col->GetTransform()->SetLocalPosition(Data.Info.Pos);
		Data.Col->GetTransform()->SetLocalScaling(Data.Info.Scale);
		Data.Col->SetCollisionType(CollisionType::AABBBox3D);
	}
}

void ItemBoxManager::UserAllLoad(GameEngineDirectory _Dir)
{
	std::vector<GameEngineFile> vecFile = _Dir.GetAllFile(".ItemBoxInfo");

	for (size_t i = 0; i < vecFile.size(); i++)
	{
		UserLoad(vecFile[i].GetFullPath());
	}
	
	ItemBoxInfoPath = _Dir.GetFullPath();

	//UserLoad_ItemListInfo();
}

void ItemBoxManager::UserSave_ItemListInfo()
{
	GameEngineFile NewFile = GameEngineFile(ItemBoxInfoPath + "\\ItemList.ItemListInfo", "wb");

	NewFile.Write(static_cast<int>(ItemBoxs.size()));

	for (auto& ItemBox : ItemBoxs)
	{
		NewFile.Write(ItemBox.first);
		NewFile.Write(static_cast<int>(ItemBox.second.size()));
		for (size_t i = 0; i < ItemBox.second.size(); i++)
		{
			NewFile.Write(static_cast<int>(ItemBox.second[i].ItemList.size()));

			for (auto& Item : ItemBox.second[i].ItemList)
			{
				NewFile.Write(Item->GetName());
				NewFile.Write(static_cast<int>(Item->Type));
				if (nullptr == Item->Renderer)
				{
					std::string str = "";
					NewFile.Write(str);
				}
				else
				{
					NewFile.Write(Item->Renderer->GetName());
				}
			}
		}
	}
}

void ItemBoxManager::UserLoad_ItemListInfo()
{
	GameEngineFile NewFile = GameEngineFile(ItemBoxInfoPath + "\\ItemList.ItemListInfo", "rb");

	int mapSize = 0;

	// map 갯수
	NewFile.Read(mapSize);

	for (size_t i = 0; i < mapSize; i++)
	{
		std::string KeyName;
		NewFile.Read(KeyName);

		std::vector<ItemBox> vecItemBox;

		int vecItemBoxSize = 0;
		NewFile.Read(vecItemBoxSize);

		vecItemBox.resize(vecItemBoxSize);

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
				case ItemType::None:
					break;
				case ItemType::Equipment:
					Item = GetLevel()->CreateActor<EquipmentItem>();
					break;
				case ItemType::Useable:
					Item = GetLevel()->CreateActor<UseableItem>();
					break;
				case ItemType::Misc:
					Item = GetLevel()->CreateActor<MiscItem>();
					break;
				default:
					break;
				}

				if (nullptr != Item)
				{
					Item->SetName(ItemName);
					Item->Type = static_cast<ItemType>(Type);
				}

				std::string ImageName;
				NewFile.Read(ImageName);
				if ("" != ImageName)
				{
					Item->SetImage(ImageName);
				}

				vecItemBox[j].ItemList.push_back(Item);
			}

			std::map<std::string, std::vector<ItemBox>>::iterator iter = ItemBoxs.find(KeyName);

			if (ItemBoxs.end() == iter)
			{
				GameEngineDebug::MsgBoxError("if (ItemBoxs.end() == ItemBoxs.find(AreaName))");
				return;
			}

			(*iter).second[j].ItemList = vecItemBox[j].ItemList;
		}
	}
}

