#pragma once

#include <GameEngine/GameEngineLevel.h>
#include <GameEngine/GameEngineFSM.h>

// �з� : 
// �뵵 : 
// ���� : 
class LightActor;
class PJW_Hyunwoo;
class PJW_Map;
class PJW_Level : public GameEngineLevel
{
public:
	PJW_Level(); 
	~PJW_Level(); 
	PJW_Level(const PJW_Level& _other) = delete;
	PJW_Level(PJW_Level&& _other) noexcept;
	PJW_Level& operator=(const PJW_Level& _other) = delete;
	PJW_Level& operator=(const PJW_Level&& _other) = delete;



public:
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

protected:


private:
	void Init_Actors();
	// ĳ���� �� �� ���� ���� �����ϴ� �Լ�
	void Init_Keys();

private:
	PJW_Hyunwoo* player_;
	PJW_Map* map_;
	LightActor* light_A;
	LightActor* light_B;

};

