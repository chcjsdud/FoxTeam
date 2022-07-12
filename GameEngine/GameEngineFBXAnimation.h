#pragma once
#include "GameEngineFBX.h"
#include "EngineVertex.h"
#include "GameEngineFBXMesh.h"


class FbxExBoneFrameData
{
public:
	float4 S; // ũ
	float4 Q; // ��
	float4 T; // ��
	double Time;
	float4x4 FrameMat;

	fbxsdk::FbxAMatrix GlobalAnimation;
	fbxsdk::FbxAMatrix LocalAnimation;
};


// 41�� ���� 120�������� �����͸� ������ �ִ�.
class FbxExBoneFrame
{
public:
	int BoneIndex;
	int BoneParentIndex;

	// 120�������̾�.
	std::vector<FbxExBoneFrameData> BoneMatData;
};

// Take 001�̶�� �ִϸ��̼��� �ִµ�.
class FbxExAniData
{
public:
	std::string				AniName;
	fbxsdk::FbxTime			StartTime; // ������ �ִϸ��̼� �ð�
	fbxsdk::FbxTime			EndTime; // 
	fbxsdk::FbxLongLong		TimeStartCount;
	fbxsdk::FbxLongLong		TimeEndCount;
	fbxsdk::FbxLongLong		FrameCount;
	fbxsdk::FbxTime::EMode	TimeMode;
	__int64					FbxModeCount;
	double					FbxModeRate;

	// �� �ִϸ��̼ǿ� 100���� ���� �ִϸ��̼� �����Ͱ� �ִ�.
	std::vector<FbxExBoneFrame> AniFrameData;

public:
	float FrameTime(int _Frame)
	{
		fbxsdk::FbxTime tTime = {};
		tTime.SetFrame(_Frame, TimeMode);

		return (float)tTime.GetSecondDouble();
	}

public:
	__int64 FrameModeRateInt()
	{
		return static_cast<__int64>(fbxsdk::FbxGetFrameRate(TimeMode));
	}
	double FrameModeRateDouble()
	{
		return fbxsdk::FbxGetFrameRate(TimeMode);
	}

	float FrameModeRateFloat()
	{
		return static_cast<float>(fbxsdk::FbxGetFrameRate(TimeMode));
	}


public:
	FbxExAniData() 
		: AniName("")
		, StartTime(0)
		, EndTime(0)
		, TimeStartCount(0)
		, TimeEndCount(0)
		, FrameCount(0)
		, FbxModeCount(0)
		, FbxModeRate(0.0)
		, TimeMode(fbxsdk::FbxTime::EMode::eDefaultMode)
	{}
	~FbxExAniData() {}
};

class FBXAnimation;
class GameEngineFBXAnimation : public GameEngineFBX
{
	friend FBXAnimation;
public:
	// constrcuter destructer
	GameEngineFBXAnimation();
	~GameEngineFBXAnimation();

	// delete Function
	GameEngineFBXAnimation(const GameEngineFBXAnimation& _Other) = delete;
	GameEngineFBXAnimation(GameEngineFBXAnimation&& _Other) noexcept = delete;
	GameEngineFBXAnimation& operator=(const GameEngineFBXAnimation& _Other) = delete;
	GameEngineFBXAnimation& operator=(GameEngineFBXAnimation&& _Other) noexcept = delete;

	void Load(const std::string& _Path);

	// �⺻�� �Ǵ� �Ž��� �˾ƾ� �ߴ�.
	// �⺻�� �Ǵ� �Ž��� �ε尡 �ȵǾ������� �ִϸ��̼��� �ε尡 �ȵɰ��̴�.

	bool AnimationLoad(GameEngineFBXMesh* _Mesh, fbxsdk::FbxNode* Node);

	inline size_t GetAnimationCount() 
	{
		return AnimationDatas.size();
	}

	fbxsdk::FbxAMatrix GetGeometryTransformation(fbxsdk::FbxNode* pNode);
	void CalFbxExBoneFrameTransMatrix(GameEngineFBXMesh* _Mesh);

	// �ִϸ��̼� ��ķε�
	void ProcessAnimationLoad(GameEngineFBXMesh* _Mesh, fbxsdk::FbxNode* pNode);

	// �ִϸ��̼������ �ε��ߴµ� ��ä���ؾְ� ������ �⺻���� ä���ִ°�
	void ProcessAnimationCheckState(GameEngineFBXMesh* _Fbx);

protected:
	

private:
	bool CheckAnimation();

	std::vector<FbxExAniData> AnimationDatas;
};

