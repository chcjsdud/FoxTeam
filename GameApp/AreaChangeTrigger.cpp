#include "PreCompile.h"
#include "AreaChangeTrigger.h"
#include <GameEngine/GameEngineCollision.h>
#include "Character.h"
#include "PlayerUIController.h"
AreaChangeTrigger::AreaChangeTrigger() // default constructer ����Ʈ ������
	: entryPoint0_(Location::NONE), entryPoint1_(Location::NONE), entryCollision_(nullptr), bIsIn_(false)
{

}

AreaChangeTrigger::~AreaChangeTrigger() // default destructer ����Ʈ �Ҹ���
{

}

AreaChangeTrigger::AreaChangeTrigger(AreaChangeTrigger&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}

void AreaChangeTrigger::SetLocations(Location _loca0, const std::string& _loca0Name, Location _loca1, const std::string& _loca1Name)
{
	entryPoint0_ = _loca0;
	entryPoint1_ = _loca1;

	entryPointName0_ = _loca0Name;
	entryPointName1_ = _loca1Name;
}

void AreaChangeTrigger::SetCollision(float4 _pos, float4 _scale)
{
	entryCollision_->GetTransform()->SetWorldPosition(_pos);
	entryCollision_->GetTransform()->SetLocalScaling(_scale);
}

void AreaChangeTrigger::Start()
{
	entryCollision_ = CreateTransformComponent<GameEngineCollision>();
	entryCollision_->SetCollisionGroup(eCollisionGroup::Trigger);
	entryCollision_->SetCollisionType(CollisionType::AABBBox3D);
}

void AreaChangeTrigger::Update(float _DeltaTime)
{
	auto colList = entryCollision_->GetCollisionList(eCollisionGroup::Player);
	GetLevel()->PushDebugRender(entryCollision_->GetTransform(), CollisionType::AABBBox3D, float4::RED);

	if (true == bIsIn_ && false == entryCollision_->Collision(eCollisionGroup::Player))
	{
		bIsIn_ = false;
		return;
	}

	for (GameEngineCollision* col : colList)
	{
		GameEngineActor* actor = col->GetActor();
		Character* character = nullptr;

		if (false == bIsIn_ && nullptr != actor)
		{
			character = dynamic_cast<Character*>(actor);

			if (nullptr != character && true == character->IsFocused())
			{
				if (entryPoint0_ == static_cast<Location>(character->GetStat()->curLocation))
				{
					// ��Ʈ������Ʈ 0 �� �ִٰ� 1�� �Ѿ�ٴ� �ڵ���

					character->GetStat()->curLocation = static_cast<int>(entryPoint1_);
					character->GetUIController()->GetNoticeUI()->SetText(entryPointName1_, 3.f);
				}
				else if (entryPoint1_ == static_cast<Location>(character->GetStat()->curLocation))
				{
					// ��Ʈ������Ʈ 1 �� �ִٰ� 0���� �Ѿ�ٴ� �ڵ���

					character->GetStat()->curLocation = static_cast<int>(entryPoint0_);
					character->GetUIController()->GetNoticeUI()->SetText(entryPointName0_, 3.f);
				}
				else
				{
					GameEngineDebug::MsgBox("Ư�� AreaChangeTrigger ���Ϳ� �������� ���� �Ķ���Ͱ� �ֽ��ϴ�.");
				}

				bIsIn_ = true;
			}
		}
	}
}

