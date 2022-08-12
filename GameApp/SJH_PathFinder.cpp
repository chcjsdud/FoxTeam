#include "Precompile.h"
#include "SJH_PathFinder.h"

std::list<SJH_NaviCell*> SJH_PathFinder::AStarMovePath(SJH_NaviCell* _StartCell, SJH_NaviCell* _EndCell, int _Maximum)
{
	// �ʱ�ȭ
	PoolCount_ = 0;
	NodePool_.resize(_Maximum);
	OpenList_.clear();
	OpenKeys_.clear();
	CloseList_.clear();
	CloseKeys_.clear();

	// ���۳�� ����
	SJH_AStarNode* StartNode = CreateNode(_StartCell, _EndCell);
	OpenList_.insert(std::make_pair(StartNode->TotalLen_, StartNode));
	OpenKeys_.insert(StartNode->CellInfo_->GetCellInfomationIndex());

	// ������� ��� �Ҹ�ñ��� �ݺ�
	while (0 != OpenList_.size())
	{
		// �޸� �ִ�ġ Ž���� �Ͼ�ٸ� ��θ� ã������
		if (PoolCount_ >= NodePool_.size())
		{
			return std::list<SJH_NaviCell*>();
		}

		// ��� Ž�� ����
		std::vector<SJH_NaviCell*> AdjacentTri;
		AdjacentTri.clear();

		// ���۳�� ���� �� �ش� ��忡 ������ �ﰢ������ ����
		SJH_AStarNode* FirstNode = OpenList_.begin()->second;
		AdjacentTri = FirstNode->CellInfo_->GetAdjacentTriangles();

		// ������� ��Ͽ��� �ش� ��带 ����
		OpenList_.erase(OpenList_.begin());
		OpenKeys_.erase(FirstNode->CellInfo_->GetCellInfomationIndex());

		// ������� ��Ͽ� �ش� ��� �߰�
		CloseList_.push_back(FirstNode);
		CloseKeys_.insert(FirstNode->CellInfo_->GetCellInfomationIndex());

		// �ش� ��忡 ������ ��尹����ŭ üũ
		int AdjacentTriCount = static_cast<int>(AdjacentTri.size());
		for (int Tri = 0; Tri < AdjacentTriCount; ++Tri)
		{
			// 
			SJH_NaviCell* CheckNode = AdjacentTri[Tri];

			// ��������Ͽ� �ش� ��尡 �����ϸ� Continue
			if (OpenKeys_.end() != OpenKeys_.find(CheckNode->GetCellInfomationIndex()))
			{
				continue;
			}

			// ��������Ͽ� �ش� ��尡 �����ϸ� Continue
			if (CloseKeys_.end() != CloseKeys_.find(CheckNode->GetCellInfomationIndex()))
			{
				continue;
			}

			// �������� ��� �ƴ϶�� �̵����� ���� �Ǵ�
			SJH_AStarNode* NewNode = CreateNode(CheckNode, _EndCell, FirstNode);
			if (nullptr != NewNode)
			{
				// ��� ������ �������̶�� 
				NewNode->Parent_ = FirstNode;

				// ��ǥ ��忡 �����ߴٸ� �̵���� ��ȯ
				if (_EndCell == CheckNode)
				{
					return CalReturn(NewNode);
				}

				// �ƴ϶�� ������Ͽ� �߰�
				OpenList_.insert(std::make_pair(NewNode->TotalLen_, NewNode));
				OpenKeys_.insert(FirstNode->CellInfo_->GetCellInfomationIndex());
			}
		}
	}

	return std::list<SJH_NaviCell*>();
}

SJH_AStarNode* SJH_PathFinder::CreateNode(SJH_NaviCell* _StartCell, SJH_NaviCell* _EndCell, SJH_AStarNode* _ParentNode)
{
	// �ִ� ������Ƚ�� �Ѿ�� ��� �����Ұ�
	if (PoolCount_ >= NodePool_.size())
	{
		return nullptr;
	}

	// ��� ����
	SJH_AStarNode* NewNode = &NodePool_[PoolCount_++];
	NewNode->Reset();
	NewNode->CellInfo_ = _StartCell;
	NewNode->CalLen(_EndCell);

	return NewNode;
}

std::list<SJH_NaviCell*> SJH_PathFinder::CalReturn(SJH_AStarNode* _EndNode)
{
	// ��ǥ������ ��� ~ �������� ��带 ��ȸ�ϸ� ��� ����
	std::list<SJH_NaviCell*> ReturnList;

	SJH_AStarNode* FindNode = _EndNode;
	while (nullptr != FindNode->Parent_)
	{
		ReturnList.push_front(FindNode->CellInfo_);

		// �θ���� �̵�
		FindNode = FindNode->Parent_;
	}

	return ReturnList;
}

SJH_PathFinder::SJH_PathFinder()
	: PoolCount_(0)
{
}

SJH_PathFinder::~SJH_PathFinder()
{
}
