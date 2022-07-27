#include "PreCompile.h"
#include "PJW_Level.h"

#include "PJW_Hyunwoo.h"
#include "PJW_Map.h"
#include "GameEngine/LightActor.h"

PJW_Level::PJW_Level() // default constructer ����Ʈ ������
{

}

PJW_Level::~PJW_Level() // default destructer ����Ʈ �Ҹ���
{

}

PJW_Level::PJW_Level(PJW_Level&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}



void PJW_Level::LevelStart()
{
	Init_Actors();
}

void PJW_Level::LevelUpdate(float _DeltaTime)
{

}

void PJW_Level::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{

}

void PJW_Level::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{

}

void PJW_Level::Init_Actors()
{
	// �÷��̾� ĳ���� ����
	{
		player_ = CreateActor<PJW_Hyunwoo>();
	}

	// �� ����
	{
		//map_ = CreateActor<PJW_Map>();
	}

	// �� ����
	{
		light_A = CreateActor<LightActor>();
		light_A->GetLight()->SetDiffusePower(0.3f);
		light_A->GetLight()->SetSpacularLightPow(50.0f);

		light_B = CreateActor<LightActor>();
		light_B->GetLight()->SetDiffusePower(0.3f);
		light_B->GetLight()->SetSpacularLightPow(50.0f);
		light_B->GetTransform()->SetLocalRotationDegree({ 45.0f, 0.0f, 0.0f });
	}
}
