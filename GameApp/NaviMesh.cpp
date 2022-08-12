#include "PreCompile.h"
#include "NaviMesh.h"

NaviMesh::NaviMesh()
	: NaviRenderer(nullptr)
	, NewVertex(nullptr)
	, NewIndex(nullptr)
	, Color(float4::BLACK)
{
	NewVertex = new GameEngineVertexBuffer();
	NewIndex = new GameEngineIndexBuffer();
}

NaviMesh::~NaviMesh()
{
	delete NewVertex;
	delete NewIndex;
}

void NaviMesh::CreateNaviMesh(const std::vector<GameEngineVertex>& _Vertex, 
	const std::vector<UINT>& _Index)
{
	Navis.resize(_Index.size() / 3);

	for (UINT i = 0; i < Navis.size(); i++)
	{
		Navis[i].Info.Vertex[0] = _Vertex[_Index[i * 3 + 0]].POSITION;
		Navis[i].Info.Vertex[1] = _Vertex[_Index[i * 3 + 1]].POSITION;
		Navis[i].Info.Vertex[2] = _Vertex[_Index[i * 3 + 2]].POSITION;

		Navis[i].Info.Index = i;
		Navis[i].AllNavi = &Navis;
		Navis[i].Parent = this;
	}

	// Link 정보 세팅
	for (UINT i = 0; i < Navis.size(); i++)
	{
		Navi& Navi = Navis[i];

		for (UINT j = 0; j < Navis.size(); j++)
		{
			if (i == j)
			{
				continue;
			}

			if (LinkCheck(Navi, Navis[j]))
			{
				Navi.Info.Link.push_back(j);
			}
		}
	}

	NewVertex->Create(_Vertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	NewIndex->Create(_Index, D3D11_USAGE::D3D11_USAGE_DEFAULT);

	NaviRenderer = CreateTransformComponent<GameEngineRenderer>(GetTransform());
	NaviRenderer->SetRenderingPipeLine("Color");
	NaviRenderer->SetMesh(NewVertex, NewIndex);
	NaviRenderer->ShaderHelper.SettingConstantBufferLink("ResultColor", Color);
}

void NaviMesh::CreateNaviMesh(GameEngineFBXRenderer* _FBXRenderer)
{
	std::vector<FbxMeshSet>& AllMesh = _FBXRenderer->GetMesh()->GetAllMeshMap();

	int Index = 0;

	for (size_t i = 0; i < AllMesh.size(); i++)
	{
		for (size_t j = 0; j < AllMesh[i].Indexs[0][0].size() / 3; j++)
		{
			Navi& Navi = Navis.emplace_back();

			if (1 != AllMesh[i].GameEngineIndexBuffers[0].size())
			{
				GameEngineDebug::MsgBoxError("서브셋이 존재합니다.");
			}

			Navi.Info.Vertex[0] = AllMesh[i].Vertexs[AllMesh[i].Indexs[0][0][j * 3 + 0]].POSITION;
			Navi.Info.Vertex[1] = AllMesh[i].Vertexs[AllMesh[i].Indexs[0][0][j * 3 + 1]].POSITION;
			Navi.Info.Vertex[2] = AllMesh[i].Vertexs[AllMesh[i].Indexs[0][0][j * 3 + 2]].POSITION;

			Navi.Info.Index = Index++;
			Navi.AllNavi = &Navis;
			Navi.Parent = this;
		}
	}

	// Link 정보 세팅
	for (UINT i = 0; i < Navis.size(); i++)
	{
		Navi& Navi = Navis[i];

		for (UINT j = 0; j < Navis.size(); j++)
		{
			if (i == j)
			{
				continue;
			}

			if (LinkCheck(Navi, Navis[j]))
			{
				Navi.Info.Link.push_back(j);
			}
		}
	}

	NaviRenderers.resize(AllMesh.size());

	for (size_t i = 0; i < AllMesh.size(); i++)
	{
		NaviRenderers[i] = CreateTransformComponent<GameEngineRenderer>(GetTransform());
		NaviRenderers[i]->SetRenderingPipeLine("Color");
		NaviRenderers[i]->GetGameEngineRenderingPipeLine()->SetRasterizer("EngineBaseRasterizerWireFrame");
		NaviRenderers[i]->SetMesh(AllMesh[i].GameEngineVertexBuffers[0], AllMesh[i].GameEngineIndexBuffers[0][0]);

		if (1 != AllMesh[i].GameEngineIndexBuffers[0].size())
		{
			GameEngineDebug::MsgBoxError("서브셋이 존재합니다.");
		}

		NaviRenderers[i]->ShaderHelper.SettingConstantBufferLink("ResultColor", Color);
	}
}

Navi* NaviMesh::CurrentCheck(GameEngineTransform* _Transform, const float4& _Dir)
{
	float4 RayPos = _Transform->GetWorldPosition() + float4(0.0f, 100.0f, 0.0f);
	float Dist = 0.0f;

	bool Check = false;


	for (auto& Navi : Navis)
	{
		float4 V0 = Navi.Info.Vertex[0] * Navi.Parent->GetTransform()->GetTransformData().WorldWorld_;
		float4 V1 = Navi.Info.Vertex[1] * Navi.Parent->GetTransform()->GetTransformData().WorldWorld_;
		float4 V2 = Navi.Info.Vertex[2] * Navi.Parent->GetTransform()->GetTransformData().WorldWorld_;

		Check = DirectX::TriangleTests::Intersects(RayPos.DirectVector,
			_Dir.DirectVector,
			V0.DirectVector,
			V1.DirectVector,
			V2.DirectVector,
			Dist);

		if (true == Check)
		{
			return &Navi;
		}
	}

	return nullptr;
}

bool NaviMesh::LinkCheck(const Navi& _Left, const Navi& _Right)
{
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			if (_Left.Info.Vertex[i] == _Right.Info.Vertex[j])
			{
				return true;
			}
		}
	}

	return false;
}

float Navi::YCheck(GameEngineTransform* _Transform)
{
	float Dist = 0.0f;

	if (true == OutCheck(_Transform, Dist))
	{
		return -1.0f;
	}

	return Dist - 100.0f;
}

bool Navi::OutCheck(GameEngineTransform* _Transform, float& _Dist)
{
	float4 RayPos = _Transform->GetWorldPosition() + float4(0.0f, 100.0f, 0.0f);

	bool Check = false;

	float4 V0 = Info.Vertex[0] * Parent->GetTransform()->GetTransformData().WorldWorld_;
	float4 V1 = Info.Vertex[1] * Parent->GetTransform()->GetTransformData().WorldWorld_;
	float4 V2 = Info.Vertex[2] * Parent->GetTransform()->GetTransformData().WorldWorld_;

	Check = DirectX::TriangleTests::Intersects(RayPos.DirectVector,
		float4::DOWN.DirectVector,
		V0.DirectVector,
		V1.DirectVector,
		V2.DirectVector,
		_Dist);

	if (false == Check)
	{
		return true;
	}

	return false;
}

Navi* Navi::MoveCheck(GameEngineTransform* _Transform)
{
	for (size_t i = 0; i < Info.Link.size(); i++)
	{
		Navi* Navi = &AllNavi->at(Info.Link[i]);

		if (false == Navi->OutCheck(_Transform))
		{
			return Navi;
		}
	}

	return nullptr;
}

