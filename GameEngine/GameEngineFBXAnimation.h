#pragma once
#include "GameEngineFBX.h"
#include "EngineVertex.h"
#include "GameEngineFBXMesh.h"


class FbxExBoneFrameData
{
public:
	float4 S; // 크
	float4 Q; // 자
	float4 T; // 이
	double Time;
	float4x4 FrameMat;

	fbxsdk::FbxAMatrix GlobalAnimation;
	fbxsdk::FbxAMatrix LocalAnimation;
};


// 41번 본이 120프레임의 데이터를 가지고 있다.
class FbxExBoneFrame
{
public:
	int BoneIndex;
	int BoneParentIndex;

	// 120프레임이야.
	std::vector<FbxExBoneFrameData> BoneMatData;
};

// Take 001이라는 애니메이션이 있는데.
class FbxExAniData
{
public:
	std::string				AniName;
	fbxsdk::FbxTime			StartTime; // 설정된 애니메이션 시간
	fbxsdk::FbxTime			EndTime; // 
	fbxsdk::FbxLongLong		TimeStartCount;
	fbxsdk::FbxLongLong		TimeEndCount;
	fbxsdk::FbxLongLong		FrameCount;
	fbxsdk::FbxTime::EMode	TimeMode;
	__int64					FbxModeCount;
	double					FbxModeRate;

	// 이 애니메이션에 100개의 본의 애니메이션 데이터가 있다.
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

	// 기본이 되는 매쉬를 알아야 했다.
	// 기본이 되는 매쉬가 로드가 안되어있으면 애니메이션이 로드가 안될것이다.

	bool AnimationLoad(GameEngineFBXMesh* _Mesh, fbxsdk::FbxNode* Node);

	inline size_t GetAnimationCount() 
	{
		return AnimationDatas.size();
	}

	fbxsdk::FbxAMatrix GetGeometryTransformation(fbxsdk::FbxNode* pNode);
	void CalFbxExBoneFrameTransMatrix(GameEngineFBXMesh* _Mesh);

	// 애니메이션 행렬로드
	void ProcessAnimationLoad(GameEngineFBXMesh* _Mesh, fbxsdk::FbxNode* pNode);

	// 애니메이션행렬을 로드했는데 안채워준애가 있으면 기본값을 채워주는거
	void ProcessAnimationCheckState(GameEngineFBXMesh* _Fbx);

protected:
	

private:
	bool CheckAnimation();

	std::vector<FbxExAniData> AnimationDatas;
};

