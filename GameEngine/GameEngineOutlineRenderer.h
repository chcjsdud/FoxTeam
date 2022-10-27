#pragma once
#include "GameEngineRenderer.h"

// 분류 : 데이터
// 용도 : 상수버퍼 데이터셋팅
struct OutLineData
{
	float4 LineColor;
	float LineThickness;
};

// 분류 : 렌더러
// 용도 : 외곽선처리
// 설명 : 
class GameEngineOutlineRenderer : public GameEngineRenderer
{
public:
protected:
private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	GameEngineOutlineRenderer();
	~GameEngineOutlineRenderer();

protected:
	GameEngineOutlineRenderer(const GameEngineOutlineRenderer& _Other) = delete;
	GameEngineOutlineRenderer(GameEngineOutlineRenderer&& _Other) noexcept = delete;

private:
	GameEngineOutlineRenderer& operator=(const GameEngineOutlineRenderer& _Other) = delete;
	GameEngineOutlineRenderer& operator=(GameEngineOutlineRenderer&& _Other) noexcept = delete;

public:
protected:
private:
	OutLineData OutLineData_;
};

