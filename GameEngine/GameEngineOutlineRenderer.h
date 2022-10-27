#pragma once
#include "GameEngineRenderer.h"

// �з� : ������
// �뵵 : ������� �����ͼ���
struct OutLineData
{
	float4 LineColor;
	float LineThickness;
};

// �з� : ������
// �뵵 : �ܰ���ó��
// ���� : 
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

