#pragma once
#include "GameEngineRendererBase.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResHelper.h"

class RenderSet 
{
public:
	GameEngineShaderResHelper* ShaderHelper;
	GameEngineRenderingPipeLine* PipeLine_;
};

class FbxExAniData;
class GameEngineFBXRenderer;
class GameEngineFBXMesh;
class GameEngineFBXAnimation;
class FBXAnimation
{
public:
	GameEngineFBXMesh* Mesh;
	GameEngineFBXAnimation* Animation;
	FbxExAniData* PixAniData;
	GameEngineFBXRenderer* ParentRenderer;

	float CurFrameTime;
	float FrameTime;
	UINT CurFrame;
	UINT End;
	UINT Start;

	void Init();
	void Update(float _DeltaTime);
};

// ���� : �ϳ��� ���� ������ ǥ���մϴ�.
class CameraComponent;
class GameEngineLevel;
class GameEngineIndexBuffer;
class GameEngineVertexBuffer;
class GameEngineRenderingPipeLine;
class GameEngineFBXRenderer : public GameEngineRendererBase
{
	friend FBXAnimation;

private:
	friend GameEngineLevel;
	friend CameraComponent;

public:
	// constrcuter destructer
	GameEngineFBXRenderer();
	~GameEngineFBXRenderer();

	// delete Function
	GameEngineFBXRenderer(const GameEngineFBXRenderer& _Other) = delete;
	GameEngineFBXRenderer(GameEngineFBXRenderer&& _Other) noexcept = delete;
	GameEngineFBXRenderer& operator=(const GameEngineFBXRenderer& _Other) = delete;
	GameEngineFBXRenderer& operator=(GameEngineFBXRenderer&& _Other) noexcept = delete;

	// virtual void SetRenderGroup(int _Order);

	// �̳༮�� ���⿡ ����ִ� Mesh�� fbx�Դϴ�.
	void SetFBXMesh(const std::string& _Value, std::string _PipeLine);


	size_t GetRenderSetCount()
	{
		return RenderSets.size();
	}

	RenderSet& GetRenderSet(unsigned int _Index)
	{
		return RenderSets[_Index];
	}


protected:
	void Start() override;

	void Render(float _DeltaTime) override;

private:
	std::vector<RenderSet> RenderSets;

	GameEngineFBXMesh* FBXMesh;

	void Update(float _DeltaTime) override;


/////////////////////////////// �ִϸ��̼�
	// FBXAnimation

public:
	void CreateFBXAnimation(const std::string& _AnimationName, const std::string& _AnimationFBXName);
	void ChangeFBXAnimation(const std::string& _AnimationName);

private:
	std::vector<float4x4> BoneData;

	std::map<std::string, FBXAnimation*> Animations;
	FBXAnimation* CurAnimation;


};

