#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResHelper.h"

struct RendererData
{
	int IsBump;
	int Shadow;
	int IsAni;
	int Temp2;
	int Temp3;
	int Temp4;
	int Temp5;
	int Temp6;

public:
	RendererData()
		: IsBump(0)
		, Shadow(0)
		, IsAni(0)
		, Temp2(0)
		, Temp3(0)
		, Temp4(0)
		, Temp5(0)
		, Temp6(0)
	{

	}
};

// Ό³Έν :
class CameraComponent;
class GameEngineRendererBase : public GameEngineTransformComponent
{
	friend CameraComponent;

public:
	inline bool IsPreprocessing() { return IsPreprocessing_; }
	inline void PreprocessingOn() { IsPreprocessing_ = true; }
	inline void PreprocessingOff() { IsPreprocessing_ = false; }

protected:
	void Start() override;
	virtual void SetRenderGroup(int _Order);
	virtual void Render(float _DeltaTime, bool _IsDeferred);

private:

public:
	GameEngineRendererBase();
	~GameEngineRendererBase();

protected:
	GameEngineRendererBase(const GameEngineRendererBase& _Other) = delete;
	GameEngineRendererBase(GameEngineRendererBase&& _Other) noexcept = delete;

private:
	GameEngineRendererBase& operator=(const GameEngineRendererBase& _Other) = delete;
	GameEngineRendererBase& operator=(GameEngineRendererBase&& _Other) noexcept = delete;

public:
protected:
	RendererData RendererDataInst;

private:
	bool IsPreprocessing_;
};