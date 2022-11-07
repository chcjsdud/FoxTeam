#pragma once
#include "GameEngineRenderer.h"

enum class ShadowDir
{
	NONE = -1,
	TOP,
	RIGHTTOP,
	RIGHT,
	RIGHTBOTTOM,
	BOTTOM,
	LEFTBOTTOM,
	LEFT,
	LEFTTOP,
	MAX
};

// 분류 : 렌더러
// 용도 : 그림자처리
// 설명 : FBXRenderer가 가지고있으며 RendererShadowOn() 호출시 해당 렌더러가 생성되는 방식???
class GameEngineFBXRenderer;
class GameEngineShadowRenderer : public GameEngineRenderer
{
public:
protected:
private:

private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime, bool _IsDeferred);

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

