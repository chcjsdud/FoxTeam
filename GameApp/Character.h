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
	// 0922 박종원
	float4 GetDirection() { return direction_; }
	std::string GetCurAnimation() { return curAnimation_; }
	void SetDirection(float4 _dir) { direction_ = _dir; }
	bool isFocused() { return bFocused_; }

public:
	NavFace* GetCurrentNavFace() { return currentNavFace_; }

	void Focus() { bFocused_ = true; }
	void UnFocus() { bFocused_ = false; }
	void SetCurrentNavMesh(NavMesh* _NaviMesh);
	void SetCurrentNavFace(NavFace* _Navi) { currentNavFace_ = _Navi; }

protected:
	// override 해서 애니메이션을 바꿀 수 있다.
	virtual void initRendererAndAnimation() = 0;

	// 0922 박종원
public:
	virtual void changeAnimation(std::string _animationName);
protected:
	virtual void changeAnimationRun() = 0;
	virtual void changeAnimationWait() = 0;
	virtual void changeAnimationBasicAttack() = 0;

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
	void updateAttackState();
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

#pragma endregion



	//------------------------------------------------------------------------------------------------------------------

protected:
	GameEngineFBXRenderer* renderer_;
	GameEngineCollision* collision_;

	LumiaMap* currentMap_;
	NavFace* currentNavFace_;
	NavMesh* currentNavMesh_;

	ItemBoxManager* itemBoxmanager_;

	MousePointer* mouse_;

	CharacterStat stat_;
	std::string curAnimation_;

private:
	GameEngineFSM mainState_;
	GameEngineFSM normalState_;
	GameEngineFSM crowdControlState_;
	GameEngineFSM attackState_;

	float4 destination_;
	float4 direction_;

	std::vector<float4> destinations_;

	bool bFocused_;

};