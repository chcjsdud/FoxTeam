#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

#include "NavMesh.h"
#include "CharacterStat.h"

struct CombineItem;
struct QueueItem;

// ���ͳ� ���Ͽ� ������ ����ü�� ���̽��� �Ǵ� Ŭ����
class ItemBoxManager;
class MousePointer;
class LumiaMap;
class ItemBase;
class EquipmentItem;
class PlayerUIController;
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

	void Focus() { bFocused_ = true; }
	void UnFocus() { bFocused_ = false; }
	bool IsFocused() { return bFocused_; }

	void ChangeAnimation(const std::string& _animationName, bool _bForce = false);
	void ChangeOverrideAnimation(const std::string& _animationName, const std::string& _boneNameToAffect);

	void Damage(float _amount);
	void Stun(float _stunTime);
	void Knockback(float _knockbackTime, float4 _knockbackSpeed);
	void WallSlam(float _knockbackTime, float4 _knockbackSpeed, float _stunTime);



#pragma region GetterSetter
public:
	CharacterStat* GetStat() { return &stat_; }
	float4 GetDirection() { return direction_; }
	std::string GetCurAnimation() { return curAnimationName_; }
	std::string GetOverrideAnimationName() { return overrideAnimationName_; }
	std::string GetOverrideAnimationBoneName() { return overrideAnimationBoneName_; }
	NavFace* GetCurrentNavFace() { return currentNavFace_; }
	int GetIndex() { return myIndex_; }
	std::vector<ItemBase*> GetInventory() { return inventory_; }

	void SetDirection(float4 _dir) { direction_ = _dir; }
	void SetCurrentNavFace(NavFace* _Navi) { currentNavFace_ = _Navi; }
	void SetCurrentNavMesh(NavMesh* _NaviMesh);
	void SetStat(CharacterStat _status) { stat_ = _status; }
	void SetIndex(int _index) { myIndex_ = _index; }

	float GetCoolTimeQSkill() { return coolTimeQ_; }
	float GetCoolTimeWSkill() { return coolTimeW_; }
	float GetCoolTimeESkill() { return coolTimeE_; }
	float GetCoolTimeRSkill() { return coolTimeR_; }
	float GetCoolTimeDSkill() { return coolTimeD_; }

	void SetCoolTimeQSkill(float _coolTime) { coolTimeQ_ = _coolTime; }
	void SetCoolTimeWSkill(float _coolTime) { coolTimeW_ = _coolTime; }
	void SetCoolTimeESkill(float _coolTime) { coolTimeE_ = _coolTime; }
	void SetCoolTimeRSkill(float _coolTime) { coolTimeR_ = _coolTime; }
	void SetCoolTimeDSkill(float _coolTime) { coolTimeD_ = _coolTime; }

#pragma endregion

#pragma region VirtualFunc
public:
	virtual JobType GetJobType() = 0;

protected:
	virtual void initRendererAndAnimation() = 0;

	virtual void changeAnimationRun() = 0;
	virtual void changeAnimationWait() = 0;
	virtual void changeAnimationBasicAttack() = 0;

	// Main(AttackState)
	virtual void onStartBasicAttacking(Character* _target) = 0;
	virtual void onUpdateBasicAttacking(Character* _target, float _deltaTime) = 0;

	virtual void onStartQSkill() = 0;
	virtual void onUpdateQSkill(float _deltaTime) = 0;

	virtual void onStartWSkill() = 0;
	virtual void onUpdateWSkill(float _deltaTime) = 0;

	virtual void onStartESkill() = 0;
	virtual void onUpdateESkill(float _deltaTime) = 0;

	virtual void onStartRSkill() = 0;
	virtual void onUpdateRSkill(float _d1eltaTime) = 0;

	virtual void onStartDSkill() = 0;
	virtual void onUpdateDSkill(float _deltaTime) = 0;

	// ĳ���� ���� ������ FSM ������Ʈ�� update �ϰ� ���� �� ���
	virtual void onUpdateCustomState(float _deltaTime) = 0;

	// Main(DeathState)
	virtual void onStartDeath() = 0;
	virtual void onUpdateDeath(float _deltaTime) = 0;

#pragma endregion

protected:
	void inputProcess(float _deltaTime);
	void moveProcess(float _deltaTime);

private:
	void initInput();
	void initState();

private:
	void moveTick(float _deltaTime, const float4& _startPosition);
	void setRotationTo(const float4& _destination, const float4 _startPosition);

	void checkCurrentNavFace();

	void getItem(int _index);
	void getItem(const std::string& _itemName);
	void checkItemBox();
	void checkItemRecipes();	// �κ��丮 ���� ������ ���� ���ɿ��θ� �Ǻ�
	void mixingItem();
	void SetEquipBuildItem(const std::string& _itemName, EquipmentType _type);

	void checkBuildItems();
	void checkBuildItemsRecursive(ItemBase* _item);

protected:
	Character* getMousePickedCharacter();


	//------------------------------------------------------------------------------------------------------------------
	// State
	//------------------------------------------------------------------------------------------------------------------

	// Main FSM �� �ٸ� FSM�� Update �� å������.
	//
	//   ������
	//                    [Main]
	//     ----------------  |  ---------------- ----------------------------
	//     |                 |                 |				|           |
	//  [Normal]            [CC]            [Attack]		 [Death]     [Custom]
	// 
#pragma region MainState
	void startNormalState();
	void updateNormalState(float _deltaTime);
	void endNormalState();

	void startCrowdControlState();
	void updateCrowdControlState(float _deltaTime);

	void startAttackState();
	void updateAttackState(float _deltaTime);

	// ĳ������ ������ FSM ������Ʈ�� update �� �ش�.
	void startCustomState();
	void updateCustomState(float _deltaTime);

	void startDeathState();
	void updateDeathState(float _deltaTime);
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

	void startChase();
	void updateChase(float _deltaTime);
#pragma endregion



#pragma region CrowdControlState
	void startStun();
	void updateStun(float _deltaTime);

	void startKnockback();
	void updateKnockback(float _deltaTime);

	void startWallSlam();
	void updateWallSlam(float _deltaTime);

	//void startHyunwooE();
	//void updateHyunwooE(float _deltaTime);

#pragma endregion



#pragma region AttackState
	void startBasicAttack();
	void updateBasicAttack(float _deltaTime);

	void startBasicAttacking();
	void updateBasicAttacking(float _deltaTime);

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

#pragma region DeathState
	void startPlayerDeath();
	void updatePlayerDeath(float _deltaTime);
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
	std::vector<ItemBase*> inventory_;	// 10ĭ
	std::list<QueueItem> queueItemMixing_;

	std::vector<EquipmentItem*> equipedItem_;
	std::vector<EquipmentItem*>	equipBuildItem_;	// ������ ���� ��� ������
	std::list<ItemBase*>		allMyBuildItems_;	// �������� �ϼ��ϱ� ���� ��� ���

	// ĳ���� ����, �ɷ�ġ
	CharacterStat stat_;
	std::string curAnimationName_;
	std::string overrideAnimationName_;
	std::string overrideAnimationBoneName_;

	float coolTimeQ_;
	float coolTimeW_;
	float coolTimeE_;
	float coolTimeR_;
	float coolTimeD_;

	// Omni State
	GameEngineFSM mainState_;

	// In Main State
	GameEngineFSM normalState_;
	GameEngineFSM crowdControlState_;
	GameEngineFSM attackState_;

	// ����
	Character* target_;

	GameEngineFSM deathState_;


	// ���� ����
	float timerStun_;
	float timerKnockback_;
	float4 knockbackSpeed_;

	//float timerHyunwooE_;
	//float4 dirHyunwooE_;

	// �� ��
	MousePointer* mouse_;
	bool bFocused_;
	float attackCooldown_;
	float attackTime_;

	// 0927������
	int myIndex_;
	bool isPlayerDead_;

	// 1004 �̰�ȣ : UI��Ʈ�ѷ� �������ȭ
	PlayerUIController* uiController_;

private:
	enum class eCurrentAnimation
	{
		None,
		Wait,
		Run,
		BasicAttack
	};
	eCurrentAnimation currentAnimation_;
};