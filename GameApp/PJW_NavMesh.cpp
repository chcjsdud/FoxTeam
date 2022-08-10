#include "PreCompile.h"
#include "PJW_NavMesh.h"

PJW_NavMesh::PJW_NavMesh() // default constructer 디폴트 생성자
{

}

PJW_NavMesh::~PJW_NavMesh() // default destructer 디폴트 소멸자
{

}

PJW_NavMesh::PJW_NavMesh(PJW_NavMesh&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}


bool Check(PJW_NavTile& _left, PJW_NavTile& _right)
{
	// 두 네브 타일의 세 버텍스를 3x3 돌아서
	// 인접하는 여부를 bool 값으로 리턴합니다.
	{
		for (size_t o = 0; o < 3; o++)
		{
			for (size_t p = 0; p < 3; p++)
			{
				if (_left.info_.vertex_[o] == _right.info_.vertex_[p])
				{
					// 같은 게 하나라도 있으면 "인접" 으로 판단한다!
					return true;
				} 
			}
		}

		return false;
	}
}

// 네브메시를 만드는 함수. (인자 1 : / 인자 2 : 
// 맵을 한번 로딩할 때 한번에 바이너리로 모두 가져올 수 있도록 하는 게 좋을 지도?
void PJW_NavMesh::CreateNavMesh(std::vector<GameEngineVertex> _vertex, std::vector<UINT> _Index)
{
	navTiles_.resize(_Index.size()/3);
	// 네브타일의 벡터값을 점들의 순번의 사이즈/3 을 이용해 자리를 만든다 

	for (size_t i = 0; i < navTiles_.size(); i++)
	{
		navTiles_[i].info_.vertex_[i + 0] = _vertex[_Index[i + 0]].POSITION;
		navTiles_[i].info_.vertex_[i + 1] = _vertex[_Index[i + 1]].POSITION;
		navTiles_[i].info_.vertex_[i + 2] = _vertex[_Index[i + 2]].POSITION;

		// 한번 도는 걸로 버텍스 세 개 -> 삼각형의 네브메시타일이 만들어진다.
		// 네브타일 리스트 인덱스 하나에 버텍스가 ?
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
			{	// 현재 만든 타일과 다른 타일들을 쭈우우우우욱 다 인접하는지 체크해준다.
				// 인접하면 이 내부로 들어오게

				// 이제 그 네브타일에 인접한 navTile 인덱스값을 푸시백 해 준다.
				navTile.info_.link_.push_back(j);

				// 몇번째 타일하고 링크네?

				// 네브메시에서 이동한다는 건
				// 삼각형에서 다른 삼각형으로 넘어간다는 것.
				
				// 이러면, 완전히 순간이동 하지 않는 이상 모두 통용된다.
			}



		}
	}
}

// *** 주의!!! 실시간 플레이 중 사용하면 메모리 지옥을 맛 볼것.
// 그냥 디버깅이나 개발 중 특정 위치를 리턴받아 알고 싶을 때나 사용하길.
PJW_NavTile* PJW_NavMesh::CurNavTileCheck(GameEngineTransform* _check, float4& _dir)
{
	// NavTile 은 외부에서 사용할 수 있게
	for (size_t i = 0; i < navTiles_.size(); i++)
	{
		return &navTiles_[i];
	}
}