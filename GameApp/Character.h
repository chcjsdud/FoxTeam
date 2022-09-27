#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

#include "NavMesh.h"
#include "CharacterStat.h"

// 이터널 리턴에 나오는 실험체의 베이스가 되는 클래스
class ItemBoxManager;
class MousePointer;
class LumiaMap;
class Character : public GameEngineActor
{
public:
	Character();
	~Character();
	Character(const Character& _other) = delete;
	Character(Character&& _other) noexcept = delete;
	Character& operator=(const Character& _other) = delete;
	Character& operator=(const Character&& _other) = delete;

	void Start() override;
	void Update(float _DeltaTime) override;

public:
	void InitSpawnPoint(const float4& _position);

	void Move(const float4& _position);
	void MoveWithPathFind(const float4& _position);

public:
	float4 GetDirection() { return direction_; }
	std::string GetCurAnimation() { return curAnimation_; }
	void SetDirection(float4 _dir) { direction_ = _dir; }
	// 0922 박종원
	bool IsFocused() { return bFocused_; }
	void ChangeAnimation(std::string _animationName);

public:
	NavFace* GetCurrentNavFace() { return currentNavFace_; }

	void Focus() { bFocused_ = true; }
	void UnFocus() { bFocused_ = false; }
	void SetCurrentNavMesh(NavMesh* _NaviMesh);
	void SetCurrentNavFace(NavFace* _Navi) { currentNavFace_ = _Navi; }

protected:
	// override 해서 애니메이션을 바꿀 수 있다.
	virtual void initRendererAndAnimation() = 0;

protected:
	virtual void changeAnimationRun() = 0;
	virtual void changeAnimationWait() = 0;
	virtual void changeAnimationBasicAttack() = 0;

	virtual void onStartQSkill() = 0;
	virtual void onUpdateQSkill(float _deltaTime) = 0;

	virtual void onStartWSkill() = 0;
	virtual void onUpdateWSkill(float _deltaTime) = 0;

	virtual void onStartESkill() = 0;
	virtual void onUpdateESkill(float _deltaTime) = 0;

	virtual void onStartRSkill() = 0;
	virtual void onUpdateRSkill(float _deltaTime) = 0;

	virtual void onStartDSkill() = 0;
	virtual void onUpdateDSkill(float _deltaTime) = 0;

private:
	void initInput();
	void initState();

	void inputProcess(float _deltaTime);
	void moveProcess(float _deltaTime);
	void moveTick(float _deltaTime, const float4& _startPosition);

	void checkCurrentNavFace();

	void getItem();
	void checkItemBox();




	//------------------------------------------------------------------------------------------------------------------
	// State
	//------------------------------------------------------------------------------------------------------------------

	// Main FSM 이 다른 FSM의 Update 를 책임진다.
	//
	//   계층도
	//                    [Main]
	//     ----------------  |  ----------------
	//     |                 |                 |
	//  [Normal]            [CC]            [Attack]
	// 

#pragma region MainState
	void startNormalState();
	void updateNormalState(float _deltaTime);

	void startCrowdControlState();
	void updateCrowdControlState(float _deltaTime);

	void startAttackState();
	void updateAttackState(float _deltaTime);
#pragma endregion



#pragma region NormalState
	// Watch 는 Stop 과 다르다. 공격 범위에 실험체가 있다면 자동으로 공격한다.
	void startWatch();
	void updateWatch(float _deltaTime);

	// Stop 은 공격당하거나, 공격 범위에 실험체가 있더라도 자동으로 공격하지 않는다.
	void startStop();
	void updateStop(float _deltaTime);

	void startRun();
	void updateRun(float _deltaTime);


#pragma endregion



#pragma region CrowdControlState
	void startStun();
	void updateStun(float _deltaTime);
#pragma endregion



#pragma region AttackState
	void startNoAttack();
	void updateNoAttack(float _deltaTime);
	
	void startBasicAttack();
	void updateBasicAttack(float _deltaTime);

	void startQSkill();
	void updateQSkill(float _deltaTime);

	void startWSkill();
	void updateWSkill(float _deltaTime);

	void startESkill();
	void updateESkill(float _deltaTime);

	void startRSkill();
	void updateRSkill(float _deltaTime);

	void startDSkill();
	void updateDSkill(float _deltaTime);
#pragma endregion



	//------------------------------------------------------------------------------------------------------------------

protected:
	// 컴포넌트
	GameEngineFBXRenderer* renderer_;
	GameEngineCollision* collision_;


	// 맵, 이동
	LumiaMap* currentMap_;
	NavFace* currentNavFace_;
	NavMesh* currentNavMesh_;
	float4 destination_;
	float4 direction_;
	std::vector<float4> destinations_;


	// 아이템
	ItemBoxManager* itemBoxmanager_;


	// 캐릭터 상태
	CharacterStat stat_;
	std::string curAnimation_;
	GameEngineFSM mainState_;
	GameEngineFSM normalState_;
	GameEngineFSM crowdControlState_;
	GameEngineFSM attackState_;


	// 그 외
	MousePointer* mouse_;
	bool bFocused_;
};