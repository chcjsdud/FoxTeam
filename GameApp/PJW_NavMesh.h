#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngine/EngineVertex.h>
#include "GameEngine/GameEngineTransform.h"
// �з� : 
// �뵵 : 
// ���� : 

class PJW_NavTile_Info
{
public:
	int index_; // �� ��° �׺�޽� Ÿ���ΰ�?
	// �ƴ�, ���ؽ��� �ε����� �����Ͻô� �ǰ�?? 
	float4 vertex_[3]; // �����Ǿ� �ִ� ���ؽ��� ��ǥ�� �迭 3��(�ﰢ���̴�)
	std::vector<UINT> link_; // �ֺ��� ���� ��ǥ�� �����ϴ� ���ؽ��� ���� (���� ��ǥ�� �����ȴ� -> ������ �Ÿ��� �ִ� �׺�޽� Ÿ���̴�!)

};

class PJW_NavMesh;
class PJW_NavTile
{
public:

	PJW_NavTile_Info info_;
	PJW_NavMesh* parentNavMesh_;
	std::vector<PJW_NavTile>* allNavi_;

	float YCheck(GameEngineTransform& _transform)
	{
		return true;
	}

	bool OutCheck(GameEngineTransform& _transform)
	{
		return true;
	}

	PJW_NavTile* MoveCheck(GameEngineTransform& _transform)
	{
		// �÷��̾ Ư�� �׺�Ÿ�Ͽ� �ִ� ������
		// �׷� �� �Լ��� ȣ���� ��ũ�� �� Ÿ�Ϸ� �ٲ۴�


		return nullptr;
	}
};

// �� ������ �����̴� ��� ���ʹ�
// �� �ڱ� �׺�޽ø� ������?? �׺�Ÿ���� ������??
// ���Ͱ� ���� �� �ִ� ���� �׺� ������Ʈ
// ��ġ �ݺ��ڸ��� �׺� Ÿ�ϵ��� �Ⱦ� �� üũ�ϴ� ������ �Ѵ�.
class PJW_NavAgent
{
public:
	GameEngineTransform* transform_;

	void Update();
};



class PJW_NavMesh
{
private:	// member Var
	std::vector<PJW_NavTile> navTiles_;

public:
	PJW_NavMesh(); // default constructer ����Ʈ ������
	~PJW_NavMesh(); // default destructer ����Ʈ �Ҹ���
	PJW_NavMesh(const PJW_NavMesh& _other) = delete; // default Copy constructer ����Ʈ ���������
	PJW_NavMesh(PJW_NavMesh&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	PJW_NavMesh& operator=(const PJW_NavMesh& _other) = delete; // default Copy operator ����Ʈ ���� ������
	PJW_NavMesh& operator=(const PJW_NavMesh&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void CreateNavMesh(std::vector<GameEngineVertex> _vertex, std::vector<UINT> _Index);

	PJW_NavTile* CurNavTileCheck(GameEngineTransform* _check, float4& _dir);
	// �Ķ���� 1�� Ư�� ������ Ʈ�������� ������, ���� ��ġ�� �׺�Ÿ���� �����Ѵ�!!!

	

};

