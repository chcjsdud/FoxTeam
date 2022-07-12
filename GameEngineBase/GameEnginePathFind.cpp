#include "PreCompile.h"
#include "GameEnginePathFind.h"

GameEnginePathFind::GameEnginePathFind() 
	: PoolCount(0)
{
}

GameEnginePathFind::~GameEnginePathFind() 
{
}

std::list<PathIndex> GameEnginePathFind::AStarFind4Way(PathIndex Start, PathIndex End, std::function<bool(PathIndex)> _OpenFunction, int _Maximum /*= 1000*/)
{
	PathIndex ArrDir[4] = { {1, 0 }, {-1, 0},{0, 1},{0, -1} };

	PoolCount = 0;
	NodePool.resize(_Maximum);
	OpenList.clear();
	OpenKeys.clear();
	CloseList.clear();
	CloseKeys.clear();

	AStarNode* NewNode = CreateNode(Start, End, nullptr);
	OpenList.insert(std::make_pair(NewNode->TotalLen, NewNode));
	OpenKeys.insert(NewNode->Index.Key);

	while (0 != OpenList.size())
	{
		// �޸��� �Ѱ���� ã������ ���������� �������� ���ߴ�.
		if (PoolCount >= NodePool.size())
		{
			return std::list<PathIndex>();
		}

		AStarNode* FirstNode = OpenList.begin()->second;
		OpenList.erase(OpenList.begin());
		OpenKeys.erase(FirstNode->Index.Key);
		CloseList.push_back(FirstNode);
		CloseKeys.insert(FirstNode->Index.Key);

		for (size_t i = 0; i < 4; i++)
		{
			PathIndex FindIndex = FirstNode->Index + ArrDir[i];

			// ���¸���Ʈ���� ����
			if (OpenKeys.end() != OpenKeys.find(FindIndex.Key))
			{
				continue;
			}

			// Ŭ���� ����Ʈ���� ����
			if (CloseKeys.end() != CloseKeys.find(FindIndex.Key))
			{
				continue;
			}

			// �̰� ����ϴ� ����� �������δ� ����� ã���� �ȵǴ� ���̴�.
			if (_OpenFunction(FindIndex))
			{
				continue;
			}

			AStarNode* NewNode = CreateNode(FindIndex, End, FirstNode);

			if (End == FindIndex)
			{
				return CalReturn(NewNode);
			}

			OpenList.insert(std::make_pair(NewNode->TotalLen, NewNode));
			OpenKeys.insert(FirstNode->Index.Key);
		}
	}

	return std::list<PathIndex>();
	
}

std::list<PathIndex> GameEnginePathFind::CalReturn(AStarNode* _EndNode) 
{
	// ���� �������� �� �־�����?
	std::list<PathIndex> ReturnList;
	AStarNode* Node = _EndNode;

	while (nullptr != Node->Parent)
	{
		ReturnList.push_front(Node->Index);
		Node = Node->Parent;
	}
	return ReturnList;
}


AStarNode* GameEnginePathFind::CreateNode(PathIndex _Index, PathIndex End, AStarNode* _ParentNode)
{
	if (PoolCount >= NodePool.size())
	{
		return nullptr;
	}

	AStarNode* NewNode = &NodePool[PoolCount++];
	NewNode->Parent = _ParentNode;
	NewNode->Reset();
	NewNode->Index = _Index;
	NewNode->CalLen(End);
	return NewNode;
}