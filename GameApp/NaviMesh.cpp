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

	NaviMeshFolder.MoveParent("FoxTeam");
	NaviMeshFolder.MoveChild("Resources");
	NaviMeshFolder.MoveChild("FBX");
	NaviMeshFolder.MoveChild("UserMesh");
	NaviMeshFolder.MoveChild("Map");
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

	/*GameEngineRenderer* Renderer = CreateTransformComponent<GameEngineRenderer>(GetTransform());
	Renderer->SetRenderingPipeLine("Color");
	Renderer->SetMesh(NewVertex, NewIndex);
	Renderer->ShaderHelper.SettingConstantBufferLink("ResultColor", Color);
	NaviRenderer = Renderer;*/
}

void NaviMesh::CreateNaviMesh(GameEngineFBXRenderer* _FBXRenderer, 
	const std::string& _FileName /*= "Default"*/)
{
	std::vector<FbxMeshSet>& AllMesh = _FBXRenderer->GetMesh()->GetAllMeshMap();

	std::string FileName = _FileName;
	FileName.append(".NaviMesh");

	std::vector<GameEngineFile> vecFile = NaviMeshFolder.GetAllFile(".NaviMesh");

	bool isLoaded = false;

	for(GameEngineFile& File : vecFile)
	{
		if (std::string::npos != File.GetFileName().find(_FileName))
		{
			UserLoad(File.GetFullPath());
			isLoaded = true;
		}
	}

	if (false == isLoaded)
	{
		SaveNavisData(AllMesh);
		UserSave(NaviMeshFolder.PathToPlusFileName(FileName));
	}

	NaviRenderer = _FBXRenderer;
}

Navi* NaviMesh::CurrentCheck(GameEngineTransform* _Transform, const float4& _Dir)
{
	if (nullptr == NaviRenderer)
	{
		GameEngineDebug::MsgBoxError("NaviRenderer가 지정되지 않았습니다.");
	}

	float4 RayPos = _Transform->GetWorldPosition() + float4(0.0f, 100.0f, 0.0f);
	float Dist = 0.0f;

	bool Check = false;

	for (auto& Navi : Navis)
	{
		float4 V0 = Navi.Info.Vertex[0] * NaviRenderer->GetTransform()->GetTransformData().WorldWorld_;
		float4 V1 = Navi.Info.Vertex[1] * NaviRenderer->GetTransform()->GetTransformData().WorldWorld_;
		float4 V2 = Navi.Info.Vertex[2] * NaviRenderer->GetTransform()->GetTransformData().WorldWorld_;

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

bool NaviMesh::CheckIntersects(const float4& _Position, const float4& _Direction, float& _Distance)
{
	bool Check = false;

	if (nullptr == NaviRenderer)
	{
		GameEngineDebug::MsgBoxError("NaviRenderer가 지정되지 않았습니다.");
	}

	float4 Dir = _Direction.NormalizeReturn3D();

	float4x4 matWorld = NaviRenderer->GetTransform()->GetTransformData().WorldWorld_;

	for (size_t i = 0; i < Navis.size(); i++)
	{
		float4 V0 = Navis[i].Info.Vertex[0] * matWorld;
		float4 V1 = Navis[i].Info.Vertex[1] * matWorld;
		float4 V2 = Navis[i].Info.Vertex[2] * matWorld;

		Check = DirectX::TriangleTests::Intersects(_Position.DirectVector,
			Dir.DirectVector,
			V0.DirectVector,
			V1.DirectVector,
			V2.DirectVector,
			_Distance);

		if (true == Check)
		{
			return true;
		}
	}

	return false;
}

float4 NaviMesh::CalculateCameraDir()
{
	float4 MousePos = GameEngineInput::GetInst().GetMousePos();

	UINT ViewPortNo = 1;
	D3D11_VIEWPORT ViewPort_ = {};
	GameEngineDevice::GetInst().GetContext()->RSGetViewports(&ViewPortNo, &ViewPort_);

	if (0 == ViewPortNo)
	{
		return float4::ZERO;
	}

	float4 Dir;

	Dir.x = ((2.0f * MousePos.x) / ViewPort_.Width) - 1.0f;
	Dir.y = (((2.0f * MousePos.y) / ViewPort_.Height) - 1.0f) * -1.0f;
	Dir.z = 1.0f;
	Dir.w = 0.0f;

	// 2. 광선을 투영영역 -> 뷰영역
	float4x4 ProjMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().Projection_;

	Dir.x /= ProjMat._11;
	Dir.y /= ProjMat._22;

	// 3. 광선을 뷰영역 -> 월드영역
	float4x4 ViewMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().View_;
	float4x4 InverseViewMat = ViewMat.InverseReturn();

	Dir *= InverseViewMat;

	return Dir.NormalizeReturn3D();
}

float4 NaviMesh::GetMousePos()
{
	float4 Dir = CalculateCameraDir();

	float4 CameraPos = GetLevel()->GetMainCamera()->GetTransform()->GetWorldPosition();

	float Dist = 0.0f;

	// 교차성공시 교차점까지의 거리를 이용하여 해당 좌표를 반환
	if (true == CheckIntersects(CameraPos, Dir, Dist))
	{
		return CameraPos + (Dir * Dist);;
	}

	return float4::ZERO;
}

bool NaviMesh::IsMouseIntersects()
{
	float4 Dir = CalculateCameraDir();
	float4 CameraPos = GetLevel()->GetMainCamera()->GetTransform()->GetWorldPosition();
	float Dist = 0.0f;

	return CheckIntersects(CameraPos, Dir, Dist);
}

void NaviMesh::SaveNavisData(const std::vector<FbxMeshSet>& _AllMesh)
{
	int Index = 0;

	for (size_t i = 0; i < _AllMesh.size(); i++)
	{
		for (size_t j = 0; j < _AllMesh[i].Indexs[0][0].size() / 3; j++)
		{
			Navi& Navi = Navis.emplace_back();

			if (1 != _AllMesh[i].GameEngineIndexBuffers[0].size())
			{
				GameEngineDebug::MsgBoxError("서브셋이 존재합니다.");
			}

			Navi.Info.Vertex[0] = _AllMesh[i].Vertexs[_AllMesh[i].Indexs[0][0][j * 3 + 0]].POSITION;
			Navi.Info.Vertex[1] = _AllMesh[i].Vertexs[_AllMesh[i].Indexs[0][0][j * 3 + 1]].POSITION;
			Navi.Info.Vertex[2] = _AllMesh[i].Vertexs[_AllMesh[i].Indexs[0][0][j * 3 + 2]].POSITION;

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
}

void NaviMesh::UserLoad(const std::string& _Path)
{
	GameEngineFile NewFile = GameEngineFile(_Path, "rb");

	int Size = 0;

	NewFile.Read(Size);

	if (0 == Size)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 NaviMesh 파일입니다.");
	}

	Navis.resize(Size);

	for (auto& Data : Navis)
	{
		NewFile.Read(Data.Info.Vertex[0]);
		NewFile.Read(Data.Info.Vertex[1]);
		NewFile.Read(Data.Info.Vertex[2]);
		NewFile.Read(Data.Info.Index);

		int LinkSize = 0;

		NewFile.Read(LinkSize);

		Data.Info.Link.resize(LinkSize);

		for (size_t i = 0; i < Data.Info.Link.size(); i++)
		{
			NewFile.Read(Data.Info.Link[i]);
		}

		Data.AllNavi = &Navis;
		Data.Parent = this;
	}
}

void NaviMesh::UserSave(const std::string& _Path)
{
	GameEngineFile NewFile = GameEngineFile(_Path, "wb");

	NewFile.Write(static_cast<int>(Navis.size()));

	for (auto& Data : Navis)
	{
		NewFile.Write(Data.Info.Vertex[0]);
		NewFile.Write(Data.Info.Vertex[1]);
		NewFile.Write(Data.Info.Vertex[2]);
		NewFile.Write(Data.Info.Index);

		NewFile.Write(static_cast<int>(Data.Info.Link.size()));

		for (size_t i = 0; i < Data.Info.Link.size(); i++)
		{
			NewFile.Write(Data.Info.Link[i]);
		}
	}
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

	if (nullptr == Parent->GetNaviRenderer())
	{
		GameEngineDebug::MsgBoxError("NaviRenderer가 지정되지 않았습니다.");
	}

	float4 V0 = Info.Vertex[0] * Parent->GetNaviRenderer()->GetTransform()->GetTransformData().WorldWorld_;
	float4 V1 = Info.Vertex[1] * Parent->GetNaviRenderer()->GetTransform()->GetTransformData().WorldWorld_;
	float4 V2 = Info.Vertex[2] * Parent->GetNaviRenderer()->GetTransform()->GetTransformData().WorldWorld_;

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

void NaviMesh::Start()
{

}
void NaviMesh::Update(float _DeltaTime)
{
	
}