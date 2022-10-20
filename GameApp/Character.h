#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

#include "NavMesh.h"
#include "CharacterStat.h"
#include "IUnit.h"

struct CombineItem;
struct QueueItem;

// 이터널 리턴에 나오는 실험체의 베이스가 되는 클래스
class ItemBoxManager;
class MousePointer;
class LumiaMap;
class ItemBase;
class EquipmentItem;
class PlayerUIController;
class Character : public IUnit
{
public:
	Character();
	~Character();
	Character(const Character& _other) = delete;
	Character(Character&& _other) noexcept = delete;
	Character& operator=(const Character& _other) = delete;
	Character& operator=(const Character&& _other) = delete;

public:
	void Start() override;
	void Update(float _DeltaTime) override;

	int GetIndex() override;
	//void Damage(float _amount) override;
	void Damage(float _Amount, GameEngineActor* _Target) override;

public:
	void InitSpawnPoint(const float4& _position);

	void Move(const float4& _position);
	void MoveWithPathFind(const float4& _position);

	void Focus() { bFocused_ = true; }
	void UnFocus() { bFocused_ = false; }
	bool IsFocused() { return bFocused_; }

	void ChangeAnimation(const std::string& _animationName, bool _bForce = false);
	void ChangeOverrideAnimation(const std::string& _animationName, const std::string& _boneNameToAffect);

	void Stun(float _stunTime);
	void Knockback(float _knockbackTime, float4 _knockbackSpeed);
	void WallSlam(float _knockbackTime, float4 _knockbackSpeed, float _stunTime);


	bool IsDead() { return isPlayerDead_; }



#pragma region GetterSetter
public:
	CharacterStat* GetStat() { return &stat_; }
	float4 GetDirection() { return direction_; }
	std::string GetCurAnimation() { return curAnimationName_; }
	std::string GetOverrideAnimationName() { return overrideAnimationName_; }
	std::string GetOverrideAnimationBoneName() { return overrideAnimationBoneName_; }
	NavFace* GetCurrentNavFace() { return currentNavFace_; }
	std::vector<ItemBase*> GetInventory() { return inventory_; }

	void SetDirection(float4 _dir) { direction_ = _dir; }
	void SetCurrentNavFace(NavFace* _Navi) { currentNavFace_ = _Navi; }
	void SetCurrentNavMesh(NavMesh* _NaviMesh);
	void SetStat(CharacterStat _status) { stat_ = _status; }
	void SetIndex(int _index) { myIndex_ = _index; }


	void SetCharacterDeath();

	float GetCooltimeQSkill() { return coolTimeQ_; }
	float GetCooltimeWSkill() { return coolTimeW_; }
	float GetCooltimeESkill() { return coolTimeE_; }
	float GetCooltimeRSkill() { return coolTimeR_; }
	float GetCooltimeDSkill() { return coolTimeD_; }

	void SetCooltimeQSkill(float _coolTime) { coolTimeQ_ = _coolTime; }
	void SetCooltimeWSkill(float _coolTime) { coolTimeW_ = _coolTime; }
	void SetCooltimeESkill(float _coolTime) { coolTimeE_ = _coolTime; }
	void SetCooltimeRSkill(float _coolTime) { coolTimeR_ = _coolTime; }
	void SetCooltimeDSkill(float _coolTime) { coolTimeD_ = _coolTime; }

#pragma endregion

#pragma region VirtualFunc
public:
	virtual JobType GetJobType() = 0;

protected:
	virtual void initRendererAndAnimation() = 0;

	virtual void changeAnimationRun() = 0;
	virtual void changeAnimationWait() = 0;
	virtual void changeAnimationBasicAttack() = 0;
	virtual void changeAnimationCook() {}
	virtual void changeAnimationCraft() {}
	virtual void changeDeathAnimation() = 0;

	// Main(AttackState)
	virtual void onStartBasicAttacking(IUnit* _target) = 0;
	virtual void onUpdateBasicAttacking(IUnit* _target, float _deltaTime) = 0;

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

	// 캐릭터 마다 고유한 FSM 스테이트를 update 하고 싶을 때 사용
	virtual void onUpdateCustomState(float _deltaTime) = 0;

	// Main(DeathState)
	virtual void onStartDeath() = 0;
	virtual void onUpdateDeath(float _deltaTime) = 0;

	virtual void onPlayEffect(const std::string& _effectName) = 0;

#pragma endregion



protected:
	void inputProcess(float _deltaTime);
	void moveProcess(float _deltaTime);

	void setRotationTo(const float4& _destination, const float4 _startPosition);
	IUnit* getMousePickedCharacter();

private:
	void initInput();
	void initState();

private:
	void moveTick(float _deltaTime, const float4& _startPosition);

	void checkCurrentNavFace();

	void getItem(int _index);
	void getItem(const std::string& _itemName);
	void checkItemBox();
	void checkItemRecipes();	// 인벤토리 내의 아이템 조합 가능여부를 판별
	void mixingItem();
	void SetEquipBuildItem(const std::string& _itemName, EquipmentType _type);

	void checkBuildItems();
	void checkBuildItemsRecursive(ItemBase* _item);



	//------------------------------------------------------------------------------------------------------------------
	// State
	//------------------------------------------------------------------------------------------------------------------

	// Main FSM 이 다른 FSM의 Update 를 책임진다.
	//
	//   계층도
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

	// 캐릭터의 고유한 FSM 스테이트를 update 해 준다.
	void startCustomState();
	void updateCustomState(float _deltaTime);

	void startDeathState();
	void updateDeathState(float _deltaTime);
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

	void startChase();
	void updateChase(float _deltaTime);

	void startMixItem();
	void updateMixItem(float _deltaTime);

	void startCraft();
	void updateCraft(float _deltaTime);

	void startCook();
	void updateCook(float _deltaTime);
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


public:
#pragma region EffectPlay
	void PlayEffect(const std::string& _effectName);
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
	std::vector<ItemBase*> inventory_;	// 10칸
	std::list<QueueItem> queueItemMixing_;

	std::vector<EquipmentItem*> equipedItem_;
	std::vector<EquipmentItem*>	equipBuildItem_;	// 부위별 최종 장비 아이템
	std::list<ItemBase*>		allMyBuildItems_;	// 아이템을 완성하기 위한 모든 재료

	// 캐릭터 상태, 능력치
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

	// 공격
	IUnit* target_;

	GameEngineFSM deathState_;


	// 군중 제어
	float timerStun_;
	float timerKnockback_;
	float4 knockbackSpeed_;

	//float timerHyunwooE_;
	//float4 dirHyunwooE_;

	// 그 외
	MousePointer* mouse_;
	bool bFocused_;
	float attackCooldown_;
	float attackTime_;

	// characterList && pm 의 player 인덱스값과 동일한 변수값
	int myIndex_;
	//

	// 플레이어 죽음 판정
	bool isPlayerDead_;


	// 1004 이건호 : UI컨트롤러 멤버변수화
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