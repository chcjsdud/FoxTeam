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

// �з� : ������
// �뵵 : �׸���ó��
// ���� : 
class GameEngineFBXRenderer;
class GameEngineShadowRenderer : public GameEngineRenderer
{
public:	// ���̽������� ���� �� ������ ����
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
private: // ������ ���ð���
	std::string PipeLineName_;
	GameEngineFBXMesh* FBXMesh_;
	std::vector<RenderSet> RenderSets_;
	float4 ShadowColor_;

private: // ���̽�������
	GameEngineFBXRenderer* BaseRenderer_;

private: // �׸��� ǥ�� ����(���̽�����)
	ShadowDir ShadowDir_;

private:
};

