#include "PreCompile.h"
#include "LobbyUIController.h"
#include "LobbyLevel.h"

#include "LobbyBackground.h"
#include "Lobby_ButtonOne.h"
#include "Lobby_ButtonTwo.h"
#include "Lobby_PortraitBg.h"
#include "Lobby_CharFull.h"
#include "Lobby_StartButton.h"
#include "Lobby_Map.h"

JobType LobbyUIController::SelectedChar = JobType::MAX;
bool LobbyUIController::CharSelectOn = false;


LobbyUIController::LobbyUIController() // default constructer 디폴트 생성자
{

}

LobbyUIController::~LobbyUIController() // default destructer 디폴트 소멸자
{

}

LobbyUIController::LobbyUIController(LobbyUIController&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void LobbyUIController::InitUI()
{
	{
		backGroundUI_ = GetLevel()->CreateActor<LobbyBackground>();
		hostButtonUI_ = GetLevel()->CreateActor<Lobby_ButtonOne>();
		clientButtonUI_ = GetLevel()->CreateActor<Lobby_ButtonTwo>();
		charFullUI_= GetLevel()->CreateActor<Lobby_CharFull>();
		startButtonUI_ = GetLevel()->CreateActor<Lobby_StartButton>();
		mapUI_ = GetLevel()->CreateActor<Lobby_Map>();
	}

	{
		for (int x = 0; x < (int)JobType::MAX; x++)
		{
			Lobby_PortraitBg* Portrait = GetLevel()->CreateActor<Lobby_PortraitBg>();

			float4 BasicPosition = { -580.0f, 200.0f, -102.0f };
			int Vertical = (x / 4);
			int Horizonal = x - (Vertical * 4);

			Portrait->GetTransform()->SetLocalPosition(BasicPosition + float4{ float(Horizonal) * 72.0f, (float(Vertical) * -115.0f), 0.0f });
			Portrait->SetChar(static_cast<JobType>(x));

			PortraitBg.emplace_back(Portrait);
		}
	}
}

Lobby_PortraitBg* LobbyUIController::GetPortraitVector(int _index)
{
	return PortraitBg[_index];
}

void LobbyUIController::Start()
{
	InitUI();
}

void LobbyUIController::Update(float _DeltaTime)
{

}

