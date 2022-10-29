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
class GameEngineFBXRenderer;
class GameEngineOutlineRenderer : public GameEngineRenderer
{
public:
	void SetBaseRenderer(GameEngineFBXRenderer* _BaseRenderer, bool _IsCharacter = false);

protected:
private:
	void SetOutLineData(const float4& _LineColor, float _LineThickness);

private:
	void SetMesh(bool _IsCharacter);
	void SetFBXMeshRenderSet(int _MeshIndex);
	void SetFBXMeshRenderSetCharacter(int _MeshIndex);

private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime, bool _IsDeferred) override;

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

private:
	GameEngineFBXRenderer* BaseRenderer_;
	GameEngineFBXMesh* FBXMesh_;
	std::vector<RenderSet> RenderSets_;
};

