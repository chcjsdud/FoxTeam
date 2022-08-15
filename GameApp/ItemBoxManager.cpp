#include "PreCompile.h"
#include "ItemBoxManager.h"
#include <GameEngine/GameEngineCollision.h>

ItemBoxManager::ItemBoxManager()
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
	for (auto& Mesh : AllMesh)
	{
		ItemBox& Item = ItemBoxs.emplace_back();

		float4 AllAddVtxPos = float4::ZERO;

		for (auto& Vertex : Mesh.Vertexs)
		{
			AllAddVtxPos += Vertex.POSITION;
		}

		AllAddVtxPos = AllAddVtxPos / static_cast<float>(Mesh.Vertexs.size());

		Item.Info.Pos = AllAddVtxPos;
		Item.Info.Scale = { 2.0f, 2.0f, 2.0f };

		Item.Col = CreateTransformComponent<GameEngineCollision>();
		Item.Col->GetTransform()->SetLocalPosition(Item.Info.Pos);
		Item.Col->GetTransform()->SetLocalScaling(Item.Info.Scale);

		/*std::vector<std::string> checkname;

		for (size_t i = 0; i < MeshInfos.size(); i++)
		{
			std::vector<std::string>::iterator iter = checkname.begin();
			
			bool check = false;

			for (; iter != checkname.end(); iter++)
			{
				if ((*iter) == MeshInfos[i].Name)
				{
					check = true;
					break;
				}
			}

			if (check == false)
			{
				checkname.push_back(MeshInfos[i].Name);
			}
		}*/
	}

	int a = 0;

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
}

