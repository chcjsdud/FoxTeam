#pragma once
#include "PJW_Mouse.h"
#include <GameEngine/GameEngineLevel.h>
#include <GameEngine/GameEngineFSM.h>

// 분류 : 
// 용도 : 
// 설명 : 
class PJW_Mouse;
class PJW_GameController;
class LightActor;
class PJW_Hyunwoo;
class PJW_Map;
class PJW_Level : public GameEngineLevel
{
public:
	PJW_Level(); 
	~PJW_Level(); 
	PJW_Level(const PJW_Level& _other) = delete;
	PJW_Level(PJW_Level&& _other) noexcept = delete;
	PJW_Level& operator=(const PJW_Level& _other) = delete;
	PJW_Level& operator=(const PJW_Level&& _other) noexcept = delete;

public:
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

protected:
	class GameEngineFBXWindow* FBXWindow;

private:
	void Init_Resources();
	void Init_Actors();
	// 캐릭터 맵 빛 등의 액터 생성하는 함수


private:
	PJW_GameController* gameController_;
	PJW_Hyunwoo* player_;
	PJW_Map* map_;
	LightActor* light_A;
	LightActor* light_B;

	PJW_Mouse* mouse_;

public:
	PJW_Mouse* GetMousePointer() { return mouse_; }
	
};

