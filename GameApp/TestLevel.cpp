#include "PreCompile.h"
#include "TestLevel.h"

#include "Rio.h"

TestLevel::TestLevel()
{

}

TestLevel::~TestLevel()
{

}

void TestLevel::LevelStart()
{
}

void TestLevel::LevelUpdate(float _DeltaTime)
{
}

void TestLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	Rio::ReleaseResource();
}

void TestLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	Rio::LoadResource();

	CreateActor<Rio>();
}
