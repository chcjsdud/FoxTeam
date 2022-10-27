#pragma once
#include <GameEngine/GameEngineActor.h>

// �з� : ����
// �뵵 : �׽�Ʈ
// ���� : 
class GameEngineOutlineRenderer;
class OutLineTestActor : public GameEngineActor
{
public:
protected:
private:
	void Start() override;
	void Update(float _deltaTime) override;

public:
	OutLineTestActor();
	~OutLineTestActor();

protected:
	OutLineTestActor(const OutLineTestActor& _other) = delete;
	OutLineTestActor(OutLineTestActor&& _other) = delete;

private:
	OutLineTestActor& operator=(const OutLineTestActor& _other) = delete;
	OutLineTestActor& operator=(const OutLineTestActor&& _other) = delete;

public:
protected:
private:
	GameEngineOutlineRenderer* TestRenderer_;
};

