#include "PreCompile.h"
#include "TitleUIController.h"
#include "TitleIDInput.h"
#include "TitleBG.h"
#include "TitleStartButton.h"
#include "TitlePrologueBG.h"
#include "TitleFoxLogo.h"
TitleUIController::TitleUIController() // default constructer 디폴트 생성자
{

}

TitleUIController::~TitleUIController() // default destructer 디폴트 소멸자
{

}

TitleUIController::TitleUIController(TitleUIController&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void TitleUIController::InitUI()
{
	idInput_ = GetLevel()->CreateActor<TitleIDInput>();
	titlePrologueBG_ = GetLevel()->CreateActor<TitlePrologueBG>();
	titleBG_ = GetLevel()->CreateActor<TitleBG>();
	startButton_ = GetLevel()->CreateActor<TitleStartButton>();
	logo_ = GetLevel()->CreateActor<TitleFoxLogo>();
}

void TitleUIController::Start()
{
	InitUI();
}

void TitleUIController::Update(float _DeltaTime)
{

}

