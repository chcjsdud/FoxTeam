#pragma once
#include "GameEngineRenderer.h"

// �з� : ������
// �뵵 : �׸���ó��
// ���� : 
class GameEngineFBXRenderer;
class GameEngineShadowRenderer : public GameEngineRenderer
{
public:
protected:
private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime, bool _IsDeferred) override;

public:
	GameEngineShadowRenderer();
	~GameEngineShadowRenderer();

protected:
	GameEngineShadowRenderer(const GameEngineShadowRenderer& _Other) = delete;
	GameEngineShadowRenderer(GameEngineShadowRenderer&& _Other) noexcept = delete;

private:
	GameEngineShadowRenderer& operator=(const GameEngineShadowRenderer& _Other) = delete;
	GameEngineShadowRenderer& operator=(GameEngineShadowRenderer&& _Other) noexcept = delete;

public:
protected:
private:
};

