#pragma once

#include <GameEngine/GameEngineLevel.h>
#include <GameEngine/GameEngineFSM.h>
#include "LH_Unit.h"
#include "UserGame.h"

class PlayerController;
class PlayLevel : public GameEngineLevel
{
public:
	PlayLevel();
	~PlayLevel();

public:
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

	template<typename UnitType>
	UnitType* CreateUnit(int _UpdateOrder = 0);

	static unsigned int* UnitStaticIDNumbers_;
protected:

	PlayerController* PlayerController_;
	GameEngineFSM LoadState_;
	std::map<unsigned int, class Unit*> UnitMap_;

public:
	Unit* GetUnitPtr(unsigned int UnitID)
	{
		return UnitMap_.find(UnitID)->second;
	}

	PlayerController* GetPlayerController()
	{
		return PlayerController_;
	}

	

protected:
	//FSM으로 구현된 레벨 로딩 단계

	void Load_Start();
	void Load_Update(float _DeltaTime);
	void Load_End();

	void Play_Start();
	void Play_Update(float _DeltaTime);
	void Play_End();

private:
	void CreateActorLevel();

private:
	PlayLevel(const PlayLevel& _Other) = delete;
	PlayLevel(PlayLevel&& _Other) = delete;
	PlayLevel& operator=(const PlayLevel& _Other) = delete;
	PlayLevel& operator=(PlayLevel&& _Other) = delete;
};

template<typename UnitType>
inline UnitType* PlayLevel::CreateUnit(int _UpdateOrder)
{
	if (UserGame::IsServer_ == false)
	{
		GameEngineDebug::MsgBoxError("클라이언트가 Unit을 생성하려 했습니다.");
	}

	++* UnitStaticIDNumbers_; // 1번부터 부여하고, 0번은 죽거나 없는것으로 취급

	if (*UnitStaticIDNumbers_ == UINT32_MAX)
	{
		GameEngineDebug::MsgBoxError("ActorID 최댓값 초과");
	}

	Unit* NewUnit = CreateActor<UnitType>();

	NewUnit->Unit_SetUnitID(*UnitStaticIDNumbers_);

	UnitMap_.insert(std::make_pair(*UnitStaticIDNumbers_, NewUnit));

	return dynamic_cast<UnitType*>(NewUnit);
}