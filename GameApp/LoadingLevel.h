#pragma once
#include <GameEngine/GameEngineLevel.h>

#include "Enums.h"

// 분류 : 레벨(씬)
// 용도 : 
// 설명 : 플레이하려는 게임의 모든 맵, 오브젝트, 몬스터, 플레이어 액터 생성기능 제공
class SynchronizationInfoManager;
class LoadingLevel_BackDrop;
class LoadingLevel_LoadPercent;
class LoadingLevel : public GameEngineLevel
{
public:
	static bool ResourceLoadEndCheck;

public:
	static bool ReadyMapCreationCommand;
	static bool ReadyMonsterCreationCommand;
	static bool ReadyCharacterCreationCommnd;

public:
protected:
private:
	void LoadingLevelInitalize();
	void CreateBasicActor();

private:
	void CreateGameInfomation();

private:
	void MapCreationCommand();
	void MonsterCreationCommand();
	void CharacterCreationCommand();

private:
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

public:
	LoadingLevel();
	~LoadingLevel();

protected:
	LoadingLevel(const LoadingLevel& _other) = delete;
	LoadingLevel(LoadingLevel&& _other) noexcept = delete;

private:
	LoadingLevel& operator=(const LoadingLevel& _other) = delete;
	LoadingLevel& operator=(const LoadingLevel&& _other) = delete;

public:
protected:
private:
	LoadingLevel_BackDrop* BackDrop_;
	LoadingLevel_LoadPercent* LoadPercent_;
};

