#pragma once
#include <GameEngine/GameEngineActor.h>

class NaviInfo
{
	friend class Navi;
	friend class NaviMesh;
	friend class NaviActor;
public:
	NaviInfo()
		: Index(0)
	{

	}
	~NaviInfo()
	{

	}
private:
	float4 Vertex[3];
	int Index;
	std::vector<UINT> Link;
};

class Navi
{
	friend class NaviMesh;
public:
	Navi()
		: AllNavi(nullptr)
		, Parent(nullptr)
	{

	}
	~Navi()
	{

	}

	// _Transform ���� NaviMesh ������ �Ÿ��� ��ȯ�Ѵ�.
	float YCheck(GameEngineTransform* _Transform);

	// NaviActor�� �������ִ� CurrentNavi_ �ٱ����� �������� ���θ� üũ�Ѵ�.
	bool OutCheck(GameEngineTransform* _Transform)
	{
		float Dist = 0.0f;
		return OutCheck(_Transform, Dist);
	}

	Navi* MoveCheck(GameEngineTransform* _Transform);

private:
	bool OutCheck(GameEngineTransform* _Transform, float& _Dist);

	NaviInfo Info;
	std::vector<Navi>* AllNavi;
	NaviMesh* Parent;
};

class NaviMesh : public GameEngineActor
{
public:
	NaviMesh();
	~NaviMesh();

	NaviMesh(const NaviMesh& _other) = delete; 
	NaviMesh(NaviMesh&& _other) noexcept = delete;
	NaviMesh& operator=(const NaviMesh& _other) = delete;
	NaviMesh& operator=(const NaviMesh&& _other) = delete;

	void CreateNaviMesh(const std::vector<GameEngineVertex>& _Vertex, const std::vector<UINT>& _Index);

	// ���� ���̺� ������ ���ٸ� �����, �ִٸ� UserLoad()�� �����Ѵ�. 
	void CreateNaviMesh(GameEngineFBXRenderer* _FBXRenderer, const std::string& _FileName = "Default");

	Navi* CurrentCheck(GameEngineTransform* _Transform, const float4& _Dir);

	void SetColor(float4 _Color)
	{
		Color = _Color;
	}

	GameEngineRendererBase* GetNaviRenderer()
	{
		return NaviRenderer;
	}

protected:

private:
	void SaveNavisData(const std::vector<FbxMeshSet>& _AllMesh);

	void UserLoad(const std::string& _Path);
	void UserSave(const std::string& _Path);

	bool LinkCheck(const Navi& _Left, const Navi& _Right);

	GameEngineDirectory NaviMeshFolder;

	std::vector<Navi> Navis;

	GameEngineRendererBase* NaviRenderer;

	GameEngineVertexBuffer* NewVertex;
	GameEngineIndexBuffer* NewIndex;

	float4 Color;
};

