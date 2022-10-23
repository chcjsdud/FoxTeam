#include "PreCompile.h"
#include "EndingUIController.h"

EndingUIController::EndingUIController() // default constructer 디폴트 생성자
	: backgroundRenderer_(nullptr), deskRenderer_(nullptr), personRenderer_(nullptr), creditTextRenderer_(nullptr), foxTeamRenderer_(nullptr)
{

}

EndingUIController::~EndingUIController() // default destructer 디폴트 소멸자
{

}

EndingUIController::EndingUIController(EndingUIController&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void EndingUIController::Start()
{
	//GameEngineDirectory dir;
	//dir.MoveParent("FoxTeam");
	//dir / "Resources" / "Image" / "EndingLevel";
	//
	//std::vector<GameEngineFile> allFile = dir.GetAllFile("png");
	//for (GameEngineFile& file : allFile)
	//{
	//	GameEngineTextureManager::GetInst().Load(file.GetFileName(), file.GetFullPath());
	//}

	backgroundRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	backgroundRenderer_->SetImage("endingBackground.png", "PointSmp");

	backgroundRenderer_->GetTransform()->SetLocalScaling(backgroundRenderer_->GetCurrentTexture()->GetTextureSize());

	deskRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	deskRenderer_->SetImage("endingDesk.png", "PointSmp");
//	deskRenderer_->GetTransform()->SetLocalScaling(deskRenderer_->GetCurrentTexture()->GetTextureSize());
	deskRenderer_->SetAlpha(0.0f);

	personRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	personRenderer_->SetImage("endingPerson.png", "PointSmp");
//	personRenderer_->GetTransform()->SetLocalScaling(personRenderer_->GetCurrentTexture()->GetTextureSize());
	personRenderer_->SetAlpha(0.0f);

	creditTextRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	creditTextRenderer_->SetTextColor(float4::WHITE);
	creditTextRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 250.0f, 1.0f });
	creditTextRenderer_->TextSetting("굴림", " CREDIT \n\n\n  신종하\n유석진\n박종원\n이건호\n조규현", 40, FW1_CENTER);
	creditTextRenderer_->Off();

	foxTeamRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	foxTeamRenderer_->SetImage("Foxteam.png", "PointSmp");
	foxTeamRenderer_->GetTransform()->SetLocalScaling(foxTeamRenderer_->GetCurrentTexture()->GetTextureSize() * 2);
	foxTeamRenderer_->Off();
}

void EndingUIController::Update(float _DeltaTime)
{
}

