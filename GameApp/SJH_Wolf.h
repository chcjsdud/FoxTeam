#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 몬스터
// 용도 : 
// 설명 : 
class GameEngineFBXMesh;
class GameEngineFBXRenderer;
class GameEngineCollision;
class SJH_NaviCell;
class SJH_Wolf : public GameEngineActor
{
public:
	void Initialize(SJH_NaviCell* _CurNaviCell, const float4& _InitPos);

protected:

private:

private:
	void InputKeyStateCheck(float _DeltaTime);

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	SJH_Wolf();
	~SJH_Wolf();

protected:		// delete constructer
	SJH_Wolf(const SJH_Wolf& _other) = delete;
	SJH_Wolf(SJH_Wolf&& _other) noexcept = delete;

private:		//delete operator
	SJH_Wolf& operator=(const SJH_Wolf& _other) = delete;
	SJH_Wolf& operator=(const SJH_Wolf&& _other) = delete;

public:
	static SJH_Wolf* MainWolf;

protected:

private:
	GameEngineFBXMesh* BaseMesh_;
	GameEngineFBXRenderer* AnimRenderer_;

private:
	GameEngineCollision* BodyCollider_;
	GameEngineCollision* AttackColiider_;

private:
	std::vector<std::string> AnimNameList_;
	int CurAnimationIndex_;

private:
	SJH_NaviCell* CurNaviCell_;
	SJH_NaviCell* TargetNaviCell_;
	bool MoveStart_;
};

