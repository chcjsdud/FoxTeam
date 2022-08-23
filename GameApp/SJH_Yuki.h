#pragma once
#include <GameEngine/GameEngineActor.h>

enum class Yuki_State
{
	COM_ARRIVE,
	COM_BOXOPEN,
	COM_COLLECT,
	COM_COLLECT_STONE,
	COM_COLLECT_WATER,
	COM_COLLECT_WOOD,
	COM_CRAFTFOOD,
	COM_CRAFTMETAL,
	COM_DANCE,
	COM_DEATH,
	COM_DOWN_DEAD,
	COM_DOWN_RUN,
	COM_DOWN_START,
	COM_DOWN_WAIT,
	COM_DOWNDEAD,
	COM_FISHING,
	COM_INSTALL_TRAP,
	COM_OPERATE,
	COM_REST_END,
	COM_REST_LOOP,
	COM_REST_START,
	COM_RESURRECT,
	COM_RUN,
	COM_SKILL04,
	COM_SKILL04_END,
	COM_WAIT,
	MAX
};

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineFBXMesh;
class GameEngineFBXRenderer;
class SJH_NaviCell;
class SJH_Yuki : public GameEngineActor
{
public: // 스폰위치 셋팅
	void Initialize(SJH_NaviCell* _CurNaviCell, const float4& _InitPos);

public: // 이동관련
	void MoveStart(SJH_NaviCell* _TargetNaviCell, const float4& _MoveTargetPos);

public: // 기타액션
	void Detect(const GameEngineActor* _DetectTarget);
	void Attack(const GameEngineActor* _AttackTarget);
	void SkillAttack(const GameEngineActor* _AttackTarget);
	void Block(const GameEngineActor* _BlockTarget);
	void GetHit(const GameEngineActor* _GetHitTarget);

protected:

private:
	void InputKeyStateCheck(float _DeltaTime);

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
	GameEngineFBXMesh* BaseMesh_;
	GameEngineFBXRenderer* AnimRenderer_;

#pragma region 애니메이션 관리관련
private:
	std::vector<std::string> AnimNameList_;	// 현재 캐릭터가 사용하는 애니메이션 이름 목록
	int CurAnimationIndex_;					// 현재 캐릭터 애니메이션 인덱스

private:
	Yuki_State CurState_;					// 현재 캐릭터 상태

private:
#pragma endregion

#pragma region 이동관련
private:
	SJH_NaviCell* CurNaviCell_;				// 현재 플레이어가 위치하고 있는 삼각형
	SJH_NaviCell* TargetNaviCell_;			// 현재 플레이어가 이동하려는 최종목표지점의 위치한 삼각형
	bool MoveStart_;						// 이동경로 생성 완료시 On

private:
	std::list<float4> MovePath_;			// 이동경로

private:
	float4 MoveStartPos_;					// 이동경로의 시작 위치
	float4 MoveEndPos_;						// 이동경로의 목표 위치
	float MoveSpeed_;						// 이동속도
#pragma endregion
};

