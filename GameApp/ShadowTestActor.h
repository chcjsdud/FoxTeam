#pragma once
#include <GameEngine/GameEngineActor.h>

// �з� : 
// �뵵 : 
// ���� : 
class GameEngineFBXRenderer;
class GameEnginePreprocessingRenderer;
class ShadowTestActor : public GameEngineActor
{
public:
	static void TestResourceLoad();

public:
protected:
private:
	void Start() override;
	void Update(float _deltaTime) override;

public:
	ShadowTestActor();
	~ShadowTestActor();

protected:
	ShadowTestActor(const ShadowTestActor& _other) = delete;
	ShadowTestActor(ShadowTestActor&& _other) noexcept = delete;

private:
	ShadowTestActor& operator=(const ShadowTestActor& _other) = delete;
	ShadowTestActor& operator=(const ShadowTestActor&& _other) = delete;

public:
protected:
private:
	bool IsChange_;

private:
	GameEngineFBXRenderer* TestBaseRenderer_;
};

