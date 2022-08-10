#include "PreCompile.h"
#include "PJW_NavMesh.h"

PJW_NavMesh::PJW_NavMesh() // default constructer ����Ʈ ������
{

}

PJW_NavMesh::~PJW_NavMesh() // default destructer ����Ʈ �Ҹ���
{

}

PJW_NavMesh::PJW_NavMesh(PJW_NavMesh&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}


bool Check(PJW_NavTile& _left, PJW_NavTile& _right)
{
	// �� �׺� Ÿ���� �� ���ؽ��� 3x3 ���Ƽ�
	// �����ϴ� ���θ� bool ������ �����մϴ�.
	{
		for (size_t o = 0; o < 3; o++)
		{
			for (size_t p = 0; p < 3; p++)
			{
				if (_left.info_.vertex_[o] == _right.info_.vertex_[p])
				{
					// ���� �� �ϳ��� ������ "����" ���� �Ǵ��Ѵ�!
					return true;
				} 
			}
		}

		return false;
	}
}

// �׺�޽ø� ����� �Լ�. (���� 1 : / ���� 2 : 
// ���� �ѹ� �ε��� �� �ѹ��� ���̳ʸ��� ��� ������ �� �ֵ��� �ϴ� �� ���� ����?
void PJW_NavMesh::CreateNavMesh(std::vector<GameEngineVertex> _vertex, std::vector<UINT> _Index)
{
	navTiles_.resize(_Index.size()/3);
	// �׺�Ÿ���� ���Ͱ��� ������ ������ ������/3 �� �̿��� �ڸ��� ����� 

	for (size_t i = 0; i < navTiles_.size(); i++)
	{
		navTiles_[i].info_.vertex_[i + 0] = _vertex[_Index[i + 0]].POSITION;
		navTiles_[i].info_.vertex_[i + 1] = _vertex[_Index[i + 1]].POSITION;
		navTiles_[i].info_.vertex_[i + 2] = _vertex[_Index[i + 2]].POSITION;

		// �ѹ� ���� �ɷ� ���ؽ� �� �� -> �ﰢ���� �׺�޽�Ÿ���� ���������.
		// �׺�Ÿ�� ����Ʈ �ε��� �ϳ��� ���ؽ��� ?
	}

	for (size_t i = 0; i < navTiles_.size(); i++)
	{
		PJW_NavTile& navTile = navTiles_[i];

		for (size_t j = 0; j < navTiles_.size(); j++)
		{

			if (i == j)
			{
				continue;
			}

			if (true == Check(navTile, navTiles_[j]))
			{	// ���� ���� Ÿ�ϰ� �ٸ� Ÿ�ϵ��� �޿����� �� �����ϴ��� üũ���ش�.
				// �����ϸ� �� ���η� ������

				// ���� �� �׺�Ÿ�Ͽ� ������ navTile �ε������� Ǫ�ù� �� �ش�.
				navTile.info_.link_.push_back(j);

				// ���° Ÿ���ϰ� ��ũ��?

				// �׺�޽ÿ��� �̵��Ѵٴ� ��
				// �ﰢ������ �ٸ� �ﰢ������ �Ѿ�ٴ� ��.
				
				// �̷���, ������ �����̵� ���� �ʴ� �̻� ��� ���ȴ�.
			}



		}
	}
}

// *** ����!!! �ǽð� �÷��� �� ����ϸ� �޸� ������ �� ����.
// �׳� ������̳� ���� �� Ư�� ��ġ�� ���Ϲ޾� �˰� ���� ���� ����ϱ�.
PJW_NavTile* PJW_NavMesh::CurNavTileCheck(GameEngineTransform* _check, float4& _dir)
{
	// NavTile �� �ܺο��� ����� �� �ְ�
	for (size_t i = 0; i < navTiles_.size(); i++)
	{
		return &navTiles_[i];
	}
}