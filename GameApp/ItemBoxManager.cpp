#include "PreCompile.h"
#include "ItemBoxManager.h"
#include <GameEngine/GameEngineCollision.h>
#include "YSJ_Mouse.h"
#include "Enums.h"

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

