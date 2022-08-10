#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngineBase/GameEnginePathFind.h>

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineFBXMesh;
class GameEngineFBXRenderer;
class SJH_NaviCell;
class SJH_Yuki : public GameEngineActor
{
public:
	void Move(SJH_NaviCell* _TargetNaviCell, const float4& _MoveTargetPos);

protected:

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	SJH_Yuki();
	~SJH_Yuki();

protected:
	SJH_Yuki(const SJH_Yuki& _other) = delete;
	SJH_Yuki(SJH_Yuki&& _other) noexcept = delete;

private:
	SJH_Yuki& operator=(const SJH_Yuki& _other) = delete;
	SJH_Yuki& operator=(const SJH_Yuki&& _other) = delete;

public:
	static SJH_Yuki* MainPlayer;

protected:

private:
	GameEngineFBXMesh* Mesh_;
	GameEngineFBXRenderer* AnimRenderer_;

private:
	std::vector<std::string> AnimationNameList_;

private:
	SJH_NaviCell* CurNaviCell_;				// 현재 플레이어가 위치하고 있는 삼각형
	SJH_NaviCell* TargetNaviCell_;			// 현재 플레이어가 이동하려는 목표지점의 위치한 삼각형
	bool MoveFlag_;							// 이동경로 생성 완료시 On
	std::list<PathIndex> MovePath_;			// 이동 경로

};

