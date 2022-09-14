#include "PreCompile.h"
#include "NavActor.h"
#include "LumiaLevel.h"
#include "LumiaMap.h"

NavActor::NavActor()
	: CurrentNavi_(nullptr)
	, NaviMesh_(nullptr)
{

}

NavActor::~NavActor()
{

}

void NavActor::SetNaviMesh(NavMesh* _NaviMesh)
{
	NaviMesh_ = _NaviMesh;
	CurrentNavi_ = NaviMesh_->CurrentCheck(GetTransform(), float4::DOWN);
}

void NavActor::Start()
{
}

void NavActor::Update(float _DeltaTime)
{
	if (nullptr != CurrentNavi_)
	{
		if (true == CurrentNavi_->OutCheck(GetTransform()))
		{
			CurrentNavi_ = CurrentNavi_->MoveCheck(GetTransform());
		}
	}
	else
	{
		if (nullptr != GetLevelConvert<LumiaLevel>()->GetMap())
		{
			SetCurrentNavi(GetLevelConvert<LumiaLevel>()->GetMap()->GetNavMesh()->CurrentCheck(GetTransform(), float4::DOWN));
		}
	}
}

