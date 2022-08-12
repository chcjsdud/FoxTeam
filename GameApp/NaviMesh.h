#pragma once
#include <GameEngine/GameEngineActor.h>

// 파일입출력 적용

class NaviInfo
{
	friend class Navi;
	friend class NaviMesh;
	friend class NaviActor;
public:
	NaviInfo()
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
	friend class NaviActor;
public:
	Navi()
		: AllNavi(nullptr)
		, Parent(nullptr)
	{

	}
	~Navi()
	{

	}

	float YCheck(GameEngineTransform* _Transform);

	bool OutCheck(GameEngineTransform* _Transform)
	{
		float Dist = 0.0f;
		return OutCheck(_Transform, Dist);
	}

	bool OutCheck(GameEngineTransform* _Transform, float& _Dist);

	Navi* MoveCheck(GameEngineTransform* _Transform);

private:
	NaviInfo Info;
	std::vector<Navi>* AllNavi;
	GameEngineActor* Parent;
};

class NaviMesh : public GameEngineActor
{
	friend class Navi;
public:
	NaviMesh();
	~NaviMesh();

	NaviMesh(const NaviMesh& _other) = delete; 
	NaviMesh(NaviMesh&& _other) noexcept = delete;
	NaviMesh& operator=(const NaviMesh& _other) = delete;
	NaviMesh& operator=(const NaviMesh&& _other) = delete;

	void CreateNaviMesh(const std::vector<GameEngineVertex>& _Vertex, const std::vector<UINT>& _Index);
	void CreateNaviMesh(GameEngineFBXRenderer* _FBXRenderer);

	Navi* CurrentCheck(GameEngineTransform* _Transform, const float4& _Dir);

	void SetColor(float4 _Color)
	{
		Color = _Color;
	}

protected:

private:
	bool LinkCheck(const Navi& _Left, const Navi& _Right);

	std::vector<Navi> Navis;
	std::vector<GameEngineRenderer*> NaviRenderers;

	GameEngineRenderer* NaviRenderer;
	GameEngineVertexBuffer* NewVertex;
	GameEngineIndexBuffer* NewIndex;

	float4 Color;
};

