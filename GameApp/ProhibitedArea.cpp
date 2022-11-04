#include "PreCompile.h"
#include "ProhibitedArea.h"
#include "Character.h"
#include "CharStatPacket.h"
#include "PlayerUIController.h"
ProhibitedArea::ProhibitedArea() // default constructer 디폴트 생성자
{

}

ProhibitedArea::~ProhibitedArea() // default destructer 디폴트 소멸자
{

}

ProhibitedArea::ProhibitedArea(ProhibitedArea&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void ProhibitedArea::Init(Location _location, const std::string& _fbxName, float4 _pos)
{
	location_ = _location;
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh(_fbxName, "ColorProhibitedArea", false);
	renderer_->GetRenderSet(0).ShaderHelper->SettingConstantBufferSet("ResultColor", float4{ 1.0f, 0.0f, 0.0f, 0.1f });
	renderer_->GetTransform()->SetLocalPosition(_pos);
	renderer_->GetTransform()->SetLocalScaling(100.0f);
}

void ProhibitedArea::AddTriggerCollision(float4 _pos, float4 _scale)
{
	GameEngineCollision* collision = CreateTransformComponent<GameEngineCollision>();
	collision->GetTransform()->SetWorldPosition(_pos);
	collision->GetTransform()->SetWorldScaling(_scale);
	collision->SetCollisionGroup(eCollisionGroup::Trigger);
	collision->SetCollisionType(CollisionType::AABBBox3D);

	triggerList_.push_back(collision);

}

void ProhibitedArea::SetProhibited(bool _tf)
{
	isProhibited_ = _tf;

	if (true == _tf)
	{
		renderer_->On();
	}
	else
	{
		renderer_->Off();
	}

}

void ProhibitedArea::Start()
{
}

void ProhibitedArea::Update(float _DeltaTime)
{
	if (triggerList_.empty())
	{
		return;
	}

	for (int i = 0; i < triggerList_.size(); i++)
	{
		auto colList = triggerList_[i]->GetCollisionList(eCollisionGroup::Player);
		GetLevel()->PushDebugRender(triggerList_[i]->GetTransform(), CollisionType::AABBBox3D, float4::RED);

		for (GameEngineCollision* col : colList)
		{
			GameEngineActor* actor = col->GetActor();
			Character* character = nullptr;

			if (nullptr != actor)
			{
				character = dynamic_cast<Character*>(actor);

				if (nullptr != character && true == character->IsFocused())
				{
					if (location_ == static_cast<Location>(character->GetStat()->curLocation))
					{
						return;
					}

					character->GetStat()->curLocation = static_cast<int>(location_);

					if (true == isProhibited_)
					{
						character->SetTriggerProhibit(true);
					}
					else
					{
						character->SetTriggerProhibit(false);
					}

					switch (location_)
					{
					case Location::NONE:
						break;
					case Location::DOCK:
						character->GetUIController()->GetNoticeUI()->SetText("항구", 5.f);
						break;
					case Location::POND:
						character->GetUIController()->GetNoticeUI()->SetText("연못", 5.f);
						break;
					case Location::BEACH:
						character->GetUIController()->GetNoticeUI()->SetText("해변", 5.f);
						break;
					case Location::UPTOWN:
						character->GetUIController()->GetNoticeUI()->SetText("고급 주택가", 5.f);
						break;
					case Location::ALLEY:
						character->GetUIController()->GetNoticeUI()->SetText("골목길", 5.f);
						break;
					case Location::HOTEL:
						character->GetUIController()->GetNoticeUI()->SetText("호텔", 5.f);
						break;
					case Location::AVENUE:
						character->GetUIController()->GetNoticeUI()->SetText("번화가", 5.f);
						break;
					case Location::HOSPITAL:
						character->GetUIController()->GetNoticeUI()->SetText("병원", 5.f);
						break;
					case Location::TEMPLE:
						character->GetUIController()->GetNoticeUI()->SetText("절", 5.f);
						break;
					case Location::ARCHERY_RANGE:
						character->GetUIController()->GetNoticeUI()->SetText("양궁장", 5.f);
						break;
					case Location::CEMETERY:
						character->GetUIController()->GetNoticeUI()->SetText("묘지", 5.f);
						break;
					case Location::FOREST:
						character->GetUIController()->GetNoticeUI()->SetText("숲", 5.f);
						break;
					case Location::FACTORY:
						character->GetUIController()->GetNoticeUI()->SetText("공장", 5.f);
						break;
					case Location::CHAPEL:
						character->GetUIController()->GetNoticeUI()->SetText("성당", 5.f);
						break;
					case Location::SCHOOL:
						character->GetUIController()->GetNoticeUI()->SetText("학교", 5.f);
						break;
					case Location::RESERCH_CENTRE:
						break;
					case Location::ESCAPE_DOCK:
						break;
					case Location::MAX:
						break;
					default:
						break;
					}
				}
			}
		}
	}
}

