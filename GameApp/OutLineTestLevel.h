#pragma once
#include <GameEngine/GameEngineLevel.h>

// �з� : ����
// �뵵 : �׽�Ʈ
// ���� : �޽� �ƿ����� �׽�Ʈ�� ����
class OutLineTestWindow;
class OutLineTestLevel : public GameEngineLevel
{
public:
	static bool ThreadLoadingEnd;

public:
protected:

private:
	void CreateBasicActor();

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
	OutLineTestWindow* OutLineWindow_;
};
