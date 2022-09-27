#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

#include "NavMesh.h"
#include "CharacterStat.h"

// ���ͳ� ���Ͽ� ������ ����ü�� ���̽��� �Ǵ� Ŭ����
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
	// 0922 ������
	bool IsFocused() { return bFocused_; }
	void ChangeAnimation(std::string _animationName);

public:
	NavFace* GetCurrentNavFace() { return currentNavFace_; }

	void Focus() { bFocused_ = true; }
	void UnFocus() { bFocused_ = false; }
	void SetCurrentNavMesh(NavMesh* _NaviMesh);
	void SetCurrentNavFace(NavFace* _Navi) { currentNavFace_ = _Navi; }

protected:
	// override �ؼ� �ִϸ��̼��� �ٲ� �� �ִ�.
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

	// Main FSM �� �ٸ� FSM�� Update �� å������.
	//
	//   ������
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
	// Watch �� Stop �� �ٸ���. ���� ������ ����ü�� �ִٸ� �ڵ����� �����Ѵ�.
	void startWatch();
	void updateWatch(float _deltaTime);

	// Stop �� ���ݴ��ϰų�, ���� ������ ����ü�� �ִ��� �ڵ����� �������� �ʴ´�.
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
	// ������Ʈ
	GameEngineFBXRenderer* renderer_;
	GameEngineCollision* collision_;


	// ��, �̵�
	LumiaMap* currentMap_;
	NavFace* currentNavFace_;
	NavMesh* currentNavMesh_;
	float4 destination_;
	float4 direction_;
	std::vector<float4> destinations_;


	// ������
	ItemBoxManager* itemBoxmanager_;


	// ĳ���� ����
	CharacterStat stat_;
	std::string curAnimation_;
	GameEngineFSM mainState_;
	GameEngineFSM normalState_;
	GameEngineFSM crowdControlState_;
	GameEngineFSM attackState_;


	// �� ��
	MousePointer* mouse_;
	bool bFocused_;
};