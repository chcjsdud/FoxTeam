#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngine/EngineVertex.h>
#include <GameEngine/GameEngineActor.h>
#include "GameEngine/GameEngineTransform.h"
// 분류 : 
// 용도 : 
// 설명 : 

class PJW_NavTile_Info
{
public:
	int index_; // 몇 번째 네브메시 타일인가?
	// 아니, 버텍스의 인덱스를 말씀하시는 건가?? 
	float4 vertex_[3]; // 구성되어 있는 버텍스의 좌표의 배열 3개(삼각형이니)
	std::vector<UINT> link_; // 주변에 같은 좌표를 공유하는 버텍스의 모음 (같은 좌표가 공유된다 -> 인접한 거리에 있는 네브메시 타일이다!)

};

class PJW_NavMesh;
class PJW_NavTile
{
	friend PJW_NavMesh;

public:
	PJW_NavTile_Info info_;
	PJW_NavMesh* parentNavMesh_;
	std::vector<PJW_NavTile>* allNavi_;


	// 하기의 함수들은 네브메시 내에서 for 문으로 모든 Navtile 들을 돌면서 실행시켜줄 함수들
	// *** 주의!!! 실시간 플레이 중 사용하면 메모리 지옥을 맛 볼것.
// 그냥 디버깅이나 개발 중 특정 위치를 리턴받아 알고 싶을 때나 사용하길.
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
		// 플레이어가 특정 네브타일에 있다 나갔다
		// 그럼 이 함수를 호출해 링크된 그 타일로 바꾼다


		return nullptr;
	}
};


class PJW_NavTile;
class PJW_NavAgent : public GameEngineActor
{
	friend PJW_NavTile;
	// 자신의 부모와 연동된 트랜스폼을 가지고
	// 그 위치를 Update() 에서 체크해서 현재 위치한 NavTile 을 알아낸다.
	// 다만 이것도 삼각형일 필요가 있을까?
	
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
	// 현재 위치를 받아, 어느 타일에 위치해있는가? 를 알아내야 한다.
	// 고정된 삼각형(버텍스 3개의 위치) 범위 밖으로 벗어나면, 타일처럼 똑같이 OutCheck() 후 다음 타일을 찾아 값 업데이트
	// 

private:
	PJW_NavTile* curTile_;
};



class PJW_NavMesh
{
private:	// member Var
	std::vector<PJW_NavTile> navTiles_;

public:
	PJW_NavMesh(); // default constructer 디폴트 생성자
	~PJW_NavMesh(); // default destructer 디폴트 소멸자
	PJW_NavMesh(const PJW_NavMesh& _other) = delete; // default Copy constructer 디폴트 복사생성자
	PJW_NavMesh(PJW_NavMesh&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	PJW_NavMesh& operator=(const PJW_NavMesh& _other) = delete; // default Copy operator 디폴트 대입 연산자
	PJW_NavMesh& operator=(const PJW_NavMesh&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void CreateNavMesh(std::vector<GameEngineVertex> _vertex, std::vector<UINT> _Index);

	PJW_NavTile* CurNavTileCheck(GameEngineTransform* _check, float4& _dir);
	// 파라미터 1에 특정 액터의 트랜스폼을 넣으면, 현재 위치한 네브타일을 리턴한다!!!
};

