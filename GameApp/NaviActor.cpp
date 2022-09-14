#include "PreCompile.h"
#include "NaviActor.h"
#include "LumiaMap.h"

NaviActor::NaviActor()
	: CurrentNavi_(nullptr)
	, NaviMesh_(nullptr)
{

}

NaviActor::~NaviActor()
{

}

void NaviActor::SetNaviMesh(NaviMesh* _NaviMesh)
{
	NaviMesh_ = _NaviMesh;
	CurrentNavi_ = NaviMesh_->CurrentCheck(GetTransform(), float4::DOWN);
}

void NaviActor::Start()
{
}

void NaviActor::Update(float _DeltaTime)
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
		if (nullptr != LumiaMap::MainMap)
		{
			SetCurrentNavi(LumiaMap::MainMap->GetNavMesh()->CurrentCheck(GetTransform(), float4::DOWN));
		}
	}
}

