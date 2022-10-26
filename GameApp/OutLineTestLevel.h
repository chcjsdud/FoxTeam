#pragma once
#include <GameEngine/GameEngineLevel.h>

// 분류 : 레벨
// 용도 : 테스트
// 설명 : 메쉬 아웃라인 테스트용 레벨
class MouseActor;
class OutLineTestLevel : public GameEngineLevel
{
public:
protected:
private:
	void LevelStart() override;
	void LevelUpdate(float _DeltaTime) override;
	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

public:
	OutLineTestLevel();
	~OutLineTestLevel();

protected:
	OutLineTestLevel(const OutLineTestLevel& _other) = delete;
	OutLineTestLevel(OutLineTestLevel&& _other) noexcept = delete;

private:
	OutLineTestLevel& operator=(const OutLineTestLevel& _other) = delete;
	OutLineTestLevel& operator=(const OutLineTestLevel&& _other) = delete;

public:
protected:
private:
};

