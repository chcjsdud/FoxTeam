#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResHelper.h"

// Ό³Έν :
class CameraComponent;
class GameEngineRendererBase : public GameEngineTransformComponent
{
	friend CameraComponent;

public:
	// constrcuter destructer
	GameEngineRendererBase();
	~GameEngineRendererBase();

	// delete Function
	GameEngineRendererBase(const GameEngineRendererBase& _Other) = delete;
	GameEngineRendererBase(GameEngineRendererBase&& _Other) noexcept = delete;
	GameEngineRendererBase& operator=(const GameEngineRendererBase& _Other) = delete;
	GameEngineRendererBase& operator=(GameEngineRendererBase&& _Other) noexcept = delete;

protected:
	void Start() override;

	virtual void SetRenderGroup(int _Order);

	virtual void Render(float _DeltaTime, bool _IsDeferred);

private:

};