#include "PreCompile.h"
#include "UI_Minimap.h"
#include "PlayerInfoManager.h"
#include <GameEngine/GameEngineUIRenderer.h>
#include "UI_CalculateHelper.h"
#include "LumiaLevel.h"
#include <GameEngine/GameEngineLevel.h>

UI_Minimap::UI_Minimap() // default constructer ����Ʈ ������
	: bIsOn_(false)
{

}

UI_Minimap::~UI_Minimap() // default destructer ����Ʈ �Ҹ���
{

}

UI_Minimap::UI_Minimap(UI_Minimap&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}

void UI_Minimap::InitCharList()
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();


	for (int i = 0; i < pm->GetPlayerList().size(); i++)
	{
		GameEngineUIRenderer* charIcon = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), static_cast<int>(UIRenderOrder::FONT));

		JobType type = static_cast<JobType>(pm->GetPlayerList()[i].character_);

		switch (type)
		{
		case JobType::NONE:
			break;
		case JobType::YUKI:
			charIcon->SetImage("CharMap_Yuki_S000.png", "LinerSmp");
			break;
		case JobType::HYUNWOO:
			charIcon->SetImage("CharMap_Hyunwoo_S000.png", "LinerSmp");
			break;
		case JobType::JACKIE:
			charIcon->SetImage("CharMap_Jackie_S000.png", "LinerSmp");
			break;
		case JobType::RIO:
			charIcon->SetImage("CharMap_Rio_S000.png", "LinerSmp");
			break;
		case JobType::AYA:
			charIcon->SetImage("CharMap_Aya_S000.png", "LinerSmp");
			break;
		case JobType::DUMMY:
			charIcon->SetImage("CharMap_Rio_S000.png", "LinerSmp");
			break;
		case JobType::MAX:
			break;
		default:
			break;
		}

		float4 tmp = minimapRenderer_->GetTransform()->GetWorldPosition();
		charIcon->GetTransform()->SetLocalPosition({ minimapRenderer_->GetTransform()->GetLocalPosition().x - 100.0f, minimapRenderer_->GetTransform()->GetLocalPosition().y - 100.0f });
		charIcon->GetTransform()->SetLocalScaling(charIcon->GetCurrentTexture()->GetTextureSize());

		charIconList_.push_back(charIcon);
	}
}

void UI_Minimap::InitAreaRenderList()
{
}

void UI_Minimap::Toggle()
{
	if (false == bIsOn_)
	{
		renderState_ << "Awake";
		bIsOn_ = true;
		return;
	}
	else if (true == bIsOn_)
	{
		renderState_ << "Sleep";
		bIsOn_ = false;
		return;
	}

}



void UI_Minimap::SetNextProhibitedArea(Location _location)
{
}

void UI_Minimap::SetProhibitedArea(Location _location)
{
}

void UI_Minimap::CheckCharPos()
{
}

void UI_Minimap::Start()
{
	minimapRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), static_cast<int>(UIRenderOrder::UI));
	minimapRenderer_->SetImage("minimapTMP.png", "LinerSmp");
	minimapRenderer_->GetTransform()->SetLocalScaling(minimapRenderer_->GetCurrentTexture()->GetTextureSize());
	minimapRenderer_->GetTransform()->SetLocalPosition({ 400.0f, 0.0f });
	calhelper_ = GetLevel()->CreateActor<UI_CalculateHelper>();

	renderState_.CreateState(MakeState(UI_Minimap, Sleep));
	renderState_.CreateState(MakeState(UI_Minimap, Awake));
	renderState_ << "Sleep";

}

void UI_Minimap::Update(float _deltaTime)
{

	renderState_.Update(_deltaTime);

}

void UI_Minimap::startSleep()
{
	minimapRenderer_->Off();
	
	for (int i = 0; i < charIconList_.size(); i++)
	{
		charIconList_[i]->Off();
	}
}

void UI_Minimap::updateSleep(float _deltaTime)
{
}

void UI_Minimap::startAwake()
{
	
	LumiaLevel* lv = dynamic_cast<LumiaLevel*>(GetLevel());

	minimapRenderer_->On();

	for (int i = 0; i < charIconList_.size(); i++)
	{
		
		float4 beforePos = lv->GetCharacterActorList()[i]->GetTransform()->GetLocalPosition();

		beforePos.Rotate3DDegree({0.12f, 69.9f ,0.0f});

		//float4 afterPos = calhelper_->Cal3Dto2D(beforePos);

		//float adjustPosx = afterPos.x;
		//float adjustPosy = afterPos.z;

		float adjustedPosX = beforePos.x;
		float adjustedPosY = beforePos.z;
		//
		//
		adjustedPosX /= 86.0f;//43.8f;
		adjustedPosY /= 86.0f;//87.6f;
		float4 adjustedResult = { adjustedPosX + 350.0f, adjustedPosY - 40.0f };
		
		//float4 rotationResult = adjustedResult.Rotate3DDegree({ 60.0f, 0.0f, 0.0f });
		//
		//adjustPosx /= 89.0f;
		//adjustPosy /= 87.6f;
		//

		charIconList_[i]->GetTransform()->SetWorldPosition(adjustedResult);
		charIconList_[i]->On();
	}
}

void UI_Minimap::updateAwake(float _deltaTime)
{
	LumiaLevel* lv = dynamic_cast<LumiaLevel*>(GetLevel());

	for (int i = 0; i < charIconList_.size(); i++)
	{



		float4 beforePos = lv->GetCharacterActorList()[i]->GetTransform()->GetLocalPosition();

		beforePos.Rotate3DDegree({0.12f, 69.9f , 0.0f});

		//float4 afterPos = calhelper_->Cal3Dto2D(beforePos);

		//float adjustPosx = afterPos.x;
		//float adjustPosy = afterPos.z;

		float adjustedPosX = beforePos.x;
		float adjustedPosY = beforePos.z;
		//
		//
		adjustedPosX /= 86.0f;//43.8f;
		adjustedPosY /= 86.0f;
		float4 adjustedResult = { adjustedPosX + 350.0f, adjustedPosY - 40.0f};

		//float4 rotationResult = adjustedResult.Rotate3DDegree({ 60.0f, 0.0f, 0.0f });
		//
		//adjustPosx /= 89.0f;
		//adjustPosy /= 87.6f;
		//

		charIconList_[i]->GetTransform()->SetWorldPosition(adjustedResult);
	}
}

