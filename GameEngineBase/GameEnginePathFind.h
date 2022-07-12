#pragma once
#include <list>
#include <map>
#include <set>
#include "GameEngineMath.h"
#include <functional>


class PathIndex 
{
public:
	union 
	{
		struct 
		{
			int X;
			int Y;
		};

		__int64 Key;
	};

public:
	PathIndex operator + (const PathIndex& _Other)
	{
		return { _Other.X + X, _Other.Y + Y };
	}

	bool operator == (const PathIndex& _Other)
	{
		return _Other.X == X && _Other.Y == Y;
	}

	bool operator != (const PathIndex& _Other)
	{
		return _Other.X != X || _Other.Y != Y;
	}

public:
	PathIndex() 
	{

	}

	PathIndex(int _X, int _Y)
		: X(_X), Y(_Y)
	{

	}
};

///////////////////////////////////////////// AStar
class AStarNode
{
public:
	PathIndex Index;
	AStarNode* Parent;
	float PathLen;
	float EndLen;
	float TotalLen;

public:
	void CalLen(PathIndex _EndIndex)
	{
		if (nullptr == Parent)
		{
			PathLen = 0.0f;
		}
		else 
		{
			float4 ParentIndex = { static_cast<float>(Parent->Index.X), static_cast<float>(Parent->Index.Y) };
			float4 ThisIndex = { static_cast<float>(Index.X),static_cast<float>(Index.Y) };
			PathLen = Parent->PathLen + (ParentIndex - ThisIndex).Len2D();
		}

		float4 EndIndex = { static_cast<float>(_EndIndex.X), static_cast<float>(_EndIndex.Y) };
		float4 ThisIndex = { static_cast<float>(Index.X),static_cast<float>(Index.Y) };
		EndLen = (EndIndex - ThisIndex).Len2D();

		TotalLen = PathLen + EndLen;
	}

	void Reset() 
	{
		memset(this, 0x00, sizeof(*this));
	}
};


// 지역변수로 사용하지 마세요.
// 설명 :
class GameEnginePathFind
{
public:
	// constrcuter destructer
	GameEnginePathFind();
	~GameEnginePathFind();

	// delete Function
	GameEnginePathFind(const GameEnginePathFind& _Other) = delete;
	GameEnginePathFind(GameEnginePathFind&& _Other) noexcept = delete;
	GameEnginePathFind& operator=(const GameEnginePathFind& _Other) = delete;
	GameEnginePathFind& operator=(GameEnginePathFind&& _Other) noexcept = delete;

	// 대각선 안탐색
	std::list<PathIndex> AStarFind4Way(PathIndex Start, PathIndex End, std::function<bool(PathIndex)> _OpenFunction, int _Maximum = 1000);

	// FindTile(0, 2);

	// 대각선도 탐색
	std::list<PathIndex> AStarFind8Way(PathIndex Start, PathIndex End, std::function<bool(PathIndex)> _OpenFunction, int _Maximum = 1000);

protected:
	AStarNode* CreateNode(PathIndex _Index, PathIndex End, AStarNode* _ParentNode = nullptr);

private:
	int PoolCount;
	std::vector<AStarNode> NodePool;

	// new AStarNode(Start);
	std::multimap<float, AStarNode*> OpenList; // 찾아야할 리스트들.
	std::set<__int64> OpenKeys;
	std::list<AStarNode*> CloseList; // 닫힌 노드.
	std::set<__int64> CloseKeys;

	std::list<PathIndex> CalReturn(AStarNode* _EndNode);
};

