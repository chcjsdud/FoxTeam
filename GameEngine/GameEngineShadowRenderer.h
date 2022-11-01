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
	GameEngineFBXMesh* FBXMesh_;
	std::vector<RenderSet> RenderSets_;
	float4 ShadowColor_;

private:
	GameEngineFBXRenderer* BaseRenderer_;
};

