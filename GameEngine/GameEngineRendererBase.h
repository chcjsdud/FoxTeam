#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResHelper.h"

struct RendererData
{
	int IsBump;
	int IsShadow;
	int IsAni;
	int Temp2;
	int Temp3;
	int Temp4;
	int Temp5;
	int Temp6;

public:
	RendererData()
		: IsBump(0)
		, IsShadow(0)
		, IsAni(0)
		, Temp2(0)
		, Temp3(0)
		, Temp4(0)
		, Temp5(0)
		, Temp6(0)
	{

	}
};

// 분류 : 베이스(Base)
// 용도 : 
// 설명 : 모든 렌더러의 기본이 되는 데이터를 관리
class CameraComponent;
class GameEngineRendererBase : public GameEngineTransformComponent
{
	friend CameraComponent;

public: // Public Inline Get Function
	inline bool IsPreprocessing() 
	{ 
		return IsPreprocessing_; 
	}

public: // Public Inline Set Function
	inline void PreprocessingOn() 
	{
		IsPreprocessing_ = true;
	}

	inline void PreprocessingOff() 
	{ 
		IsPreprocessing_ = false; 
	}

public: // Public Shadow Processing Related Function
	void ShadowOff();
	void ShadowOn();

protected:
	void Start() override;

protected:
	virtual void SetRenderGroup(int _Order);
	virtual void Render(float _DeltaTime, bool _IsDeferred);

protected: // Protected Shadow Processing Related Function
	virtual void ShadowInit(class GameEngineRenderingPipeLine* _ShadowPipe);
	virtual void ShadowRender(float _DeltaTime);

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

protected: // Preprocessing Related Value
	bool IsPreprocessing_;

protected: // Shadow Related Value
	GameEngineRenderingPipeLine* DefaultShadowPipeLine_;
	GameEngineShaderResHelper ShadowHelper_;
};