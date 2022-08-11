#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngine/EngineVertex.h>
#include <GameEngine/GameEngineActor.h>
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
	friend PJW_NavMesh;

public:
	PJW_NavTile_Info info_;
	PJW_NavMesh* parentNavMesh_;
	std::vector<PJW_NavTile>* allNavi_;


	// �ϱ��� �Լ����� �׺�޽� ������ for ������ ��� Navtile ���� ���鼭 ��������� �Լ���
	// *** ����!!! �ǽð� �÷��� �� ����ϸ� �޸� ������ �� ����.
// �׳� ������̳� ���� �� Ư�� ��ġ�� ���Ϲ޾� �˰� ���� ���� ����ϱ�.
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


class PJW_NavTile;
class PJW_NavAgent : public GameEngineActor
{
	friend PJW_NavTile;
	// �ڽ��� �θ�� ������ Ʈ�������� ������
	// �� ��ġ�� Update() ���� üũ�ؼ� ���� ��ġ�� NavTile �� �˾Ƴ���.
	// �ٸ� �̰͵� �ﰢ���� �ʿ䰡 ������?
	
public:
	PJW_NavTile* GetCurNavTile()
	{
		return curTile_;
	}

public:
	PJW_NavAgent();
	~PJW_NavAgent();
	PJW_NavAgent(const PJW_NavAgent& _other) = delete;
	PJW_NavAgent(PJW_NavAgent&& _other) noexcept;
	PJW_NavAgent& operator=(const PJW_NavAgent& _other) = delete;
	PJW_NavAgent& operator=(const PJW_NavAgent&& _other) = delete;

protected:
	virtual void Start();
	virtual	void Update();
	void CheckCurTile();
	// ���� ��ġ�� �޾�, ��� Ÿ�Ͽ� ��ġ���ִ°�? �� �˾Ƴ��� �Ѵ�.
	// ������ �ﰢ��(���ؽ� 3���� ��ġ) ���� ������ �����, Ÿ��ó�� �Ȱ��� OutCheck() �� ���� Ÿ���� ã�� �� ������Ʈ
	// 

private:
	PJW_NavTile* curTile_;
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

