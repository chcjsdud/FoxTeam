#include "PreCompile.h"
#include "TitleUIController.h"
#include "TitleIDInput.h"
#include "TitleBG.h"
#include "TitleStartButton.h"
#include "TitlePrologueBG.h"
#include "TitleFoxLogo.h"
TitleUIController::TitleUIController() // default constructer ����Ʈ ������
{

}

TitleUIController::~TitleUIController() // default destructer ����Ʈ �Ҹ���
{

}

TitleUIController::TitleUIController(TitleUIController&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
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

