#include "PreCompile.h"
#include "ItemBoxManager.h"
#include <GameEngine/GameEngineCollision.h>

ItemBoxManager::ItemBoxManager()
{

}

ItemBoxManager::~ItemBoxManager()
{

}

int ItemBoxIndex = 0;

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

	for (size_t i = 0; i < AllMesh.size(); i++)
	{
		//if (std::string::npos == MeshInfos[i].Name.find("ItemBox"))
		//{
		//	continue;
		//}

		ItemBox& Item = ItemBoxs.emplace_back();

		float4 AllAddVtxPos = float4::ZERO;

		for (auto& Vertex : AllMesh[i].Vertexs)
		{
			AllAddVtxPos += Vertex.POSITION;
		}

		AllAddVtxPos = AllAddVtxPos / static_cast<float>(AllMesh[i].Vertexs.size());

		Item.Info.Name = MeshInfos[i].Name;
		Item.Info.Index = ItemBoxIndex++;
		Item.Info.Pos = AllAddVtxPos;
		Item.Info.Scale = { 2.0f, 2.0f, 2.0f };

		Item.Col = CreateTransformComponent<GameEngineCollision>();
		Item.Col->GetTransform()->SetLocalPosition(Item.Info.Pos);
		Item.Col->GetTransform()->SetLocalScaling(Item.Info.Scale);

		Item.Area = AreaName;
	}

	UserSave(AreaName);
}

void ItemBoxManager::BoxSelectUpdate()
{
}

void ItemBoxManager::Start()
{
}

void ItemBoxManager::Update(float _DeltaTime)
{
	for (size_t i = 0; i < ItemBoxs.size(); i++)
	{
		GetLevel()->PushDebugRender(ItemBoxs[i].Col->GetTransform(), CollisionType::AABBBox3D);
	}

	BoxSelectUpdate();
}

void ItemBoxManager::UserSave(const std::string& _Path)
{
	GameEngineFile NewFile = GameEngineFile(_Path + ".ItemBoxInfo", "wb");

	NewFile.Write(static_cast<int>(ItemBoxs.size()));

	for (auto& Data : ItemBoxs)
	{
		NewFile.Write(Data.Info.Name);
		NewFile.Write(Data.Info.Index);
		NewFile.Write(Data.Info.Pos);
		NewFile.Write(Data.Info.Scale);
		NewFile.Write(Data.Area);
	}
}

void ItemBoxManager::UserLoad(const std::string& _Path)
{
	GameEngineFile NewFile = GameEngineFile(_Path, "rb");

	int Size = 0;

	NewFile.Read(Size);

	if (0 == Size)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 ItemBoxInfo 파일입니다.");
	}

	ItemBoxs.resize(Size);

	for (auto& Data : ItemBoxs)
	{
		NewFile.Read(Data.Info.Name);
		NewFile.Read(Data.Info.Index);
		NewFile.Read(Data.Info.Pos);
		NewFile.Read(Data.Info.Scale);
		NewFile.Read(Data.Area);

		Data.Col = CreateTransformComponent<GameEngineCollision>();
		Data.Col->GetTransform()->SetLocalPosition(Data.Info.Pos);
		Data.Col->GetTransform()->SetLocalScaling(Data.Info.Scale);
	}
}

