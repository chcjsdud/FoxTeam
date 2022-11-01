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
// 설명 : 
class GameEngineFBXRenderer;
class GameEngineShadowRenderer : public GameEngineRenderer
{
public:	// 베이스렌더러 지정 및 렌더셋 셋팅
	void SetBaseRenderer(GameEngineFBXRenderer* _BaseRenderer);

protected:
private:
	void SetRenderSet(int _MeshIndex);
	void ParallelogramAlgorithm();

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
private: // 랜더셋 셋팅관련
	std::string PipeLineName_;
	GameEngineFBXMesh* FBXMesh_;
	std::vector<RenderSet> RenderSets_;
	float4 ShadowColor_;

private: // 베이스렌더러
	GameEngineFBXRenderer* BaseRenderer_;

private: // 그림자 표시 방향(베이스기준)
	ShadowDir ShadowDir_;

private:
};

