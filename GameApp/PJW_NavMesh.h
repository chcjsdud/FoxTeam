#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngine/EngineVertex.h>
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
		// 플레이어가 특정 네브타일에 있다 나갔다
		// 그럼 이 함수를 호출해 링크된 그 타일로 바꾼다


		return nullptr;
	}
};

// 판 위에서 움직이는 모든 액터는
// 다 자기 네비메시를 가지게?? 네브타일을 가지게??
// 액터가 가질 수 있는 것은 네브 에이전트
// 마치 반복자마냥 네브 타일들을 훑어 내 체크하는 역할을 한다.
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

