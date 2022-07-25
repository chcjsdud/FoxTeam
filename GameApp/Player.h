#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

#include "Player_Status.h"
/// <summary>
/// 여우 주인공 액터
/// </summary>


// 플레이어를 static으로 만들고 언제 어디서든 불러올 수 있으면 좋겠다.(편하겠다)

class GameMouse;
class GameEngineLevel;
class GameEngineImageRenderer;
class Player : public GameEngineActor
{
public:
	Player();
	~Player();
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

private:
	//state
	GameEngineFSM PlayerState_;
	GameEngineFSM CameraState_;

#pragma region Player component
	//바닥 콜리전
	GameEngineCollision* PlayerGroundCollision_;
	// RockOn 충돌체
	GameEngineCollision* PlayerLockOnCollision_;
	//피격 히트박스
	GameEngineCollision* PlayerHitBoxCollision_;
	//공격 히트박스
	GameEngineCollision* PlayerAttackHitBoxCollision_;
	//FBX Renderer
	GameEngineFBXRenderer* FBXRenderer_;
#pragma endregion

	// 인식해야 하는 마우스 액터 포인터
	GameMouse* targetMouse_;

	GameEngineActor* Target_;


	//키 방향
	float4 KeyDir_;
	//락온된 대상의 방향
	float4 TargetDir_;
	//바라봐야 할 방향
	float4 FowordDir_;
	//현재 바라보는 방향
	float4 CurFowordDir_;
	//Y축 회전 속도
	float YRotateSpeed_;
	bool IsMove_;
	bool IsRockon_;

	//UI
	class TopUI* TopUI_;
	class LockOnUI* LockOnUI_;
	class Inventory* Inventory_;

#pragma region Player Status
	//공격 텀
	float AttackTurm_;
	//공격 에니메이션 단계
	int AttackLevel_;
	//공격 시간, 이 시간 이내에 공격키를 누르면 다음 단계 공격을 함
	float AttackTime_;
	//Attack Hit Box 활성화 시간
	float AttackHitTime_;
	//구르기 쿨타임
	float RollCoolTime_;
	//구르는 시간(무적타임)
	float RollSecond_;

	//버프 리스트, 
	std::map <std::string, PlayerStatus*> Player_BufferList_;

	// 플레이어 기본 스텟
	PlayerStatus PlayerStatusBase_;
	// 플레이어 추가 스텟 / 덧
	PlayerStatus PlayerStatusAdd_;
	// 플레이어 추가 스텟 / 곱
	PlayerStatus PlayerStatusMult_;
	// 플레이어 최종 스텟
	PlayerStatus PlayerStatusFinal_;

#pragma endregion

private:
	virtual void Start();
	virtual void Update(float _DeltaTime);

#pragma region Init
private:
	//Init

	// 렌더러, 콜리전, 등등 Componenet 초기화
	void ComponenetInit();
	//State 초기화
	void StateInit();
	void KeyInit();
	void UIInit();
#pragma endregion

#pragma region State
private:
	void Idle_Start();
	void Idle_Update(float _DeltaTime);
	void Idle_End();

	void Walk_Start();
	void Walk_Update(float _DeltaTime);
	void Walk_End();

	void Run_Start();
	void Run_Update(float _DeltaTime);
	void Run_End();

	void Attack_Start();
	void Attack_Update(float _DeltaTime);
	void Attack_End();


	void AttackStateInit()
	{
		// 이 시간 안에 추가 공격 하면 연속기가 나감, 공격시 갱신
		AttackTime_ = 0.5f;

		//연속기 단계, 3단계 넘으면 다시 0으로
		AttackLevel_ = 0;

		//0.5초 간격으로 공격한다. 공격시 갱신
		AttackTurm_ = 0.25f;

		//0.1초 동안만 타격 판정이 있다. 공격시 갱신
		AttackHitTime_ = 0.1f;	
	}

#pragma endregion

#pragma region MemberUpdateFunc
private:
	//Member Func
	void DEBUGUpdate(float _DeltaTime);

	// update류 함수에 같이 들어가 카메라 업데이트, 바로 뒤에서 바라보는 함수
	void CameraUpdate_BackPosition(float _DeltaTime);
	void CameraUpdate_UpPosition(float _DeltaTime);

	void KeyDirUpdate(float _DeltaTime);
	// update류 함수에 같이 들어가 이동을 제어함
	void MoveUpdate(float _DeltaTime);

	//캐릭터가 바라보는 방향이 바로 바로 안변하고 천천히 변함(이동과는 무관하게), 
	//07.18 바라보는 방향을 넣어줘야함
	void MoveRotateUpdate(float _DeltaTime);

	//RockOn 중일때 방향 업데이트
	void RockOnDirUpdate(float _DeltaTime);

	void CurDirUpdate(float _DeltaTime);

	void RockOnUpdate(float _DeltaTime);

	void StaminaRecoverUpdate(float _DeltaTime);

	void Run_Stamina_Decrease(float _DeltaTime)
	{
		PlayerStatusFinal_.Stat_Stamina_ -= _DeltaTime;

		if (PlayerStatusFinal_.Stat_Stamina_ < 0.f)
		{
			PlayerStatusFinal_.Stat_Stamina_ = 0.f;
		}
	}


	//void StaminaDecrease(float _DeltaTime);
	//void HpRecover(float _DeltaTime);
	// 
	//void ChangeCamFunc(void (Player::* _Camfunc)(float))
	//{
	//	CamFunc_ = std::bind(_Camfunc, this, std::placeholders::_1);
	//}
#pragma endregion

#pragma region MemberFunc
private:

	void SetParentMouse(GameMouse* _targetMouse)
	{
		targetMouse_ = _targetMouse;
	}

	void SetTarget(GameEngineActor* _Target)
	{
		Target_ = _Target;
	}

	const float4 CalculateTargetDir(float4 TargetPos)
	{
		float4 Dir = TargetPos - GetTransform()->GetWorldPosition();
		Dir.Normalize3D();

		return Dir;
	}

	bool SyncPlayerStat();

	//임시 구현 상태, 인벤토리에서 아이템을 가져오고, 아이템에서 이름과 스테이터스를 가져오게끔
	bool EquipItem(std::string _BuffName, PlayerStatus* _PlayerStatus);

#pragma endregion

private:

#pragma region Player 외부함수
public:

	//void PlayerAddStat(PlayerStatus& _PlayerStatus)
	//{
	//	PlayerStatusAdd_ += _PlayerStatus;
	//}

	//void PlayerSetAddStat(PlayerStatus& _PlayerStatus)
	//{
	//	PlayerStatusAdd_ = _PlayerStatus;
	//}

	std::map < std::string, PlayerStatus*> PlayerGetBuffList()
	{
		return Player_BufferList_;
	}

	const PlayerStatus PlayerGetBaseStat()
	{
		return PlayerStatusBase_;
	}

	const PlayerStatus PlayerGetAddStat()
	{
		return PlayerStatusAdd_;
	}

	const PlayerStatus PlayerGetMultStat()
	{
		return PlayerStatusMult_;
	}

	const PlayerStatus PlayerGetFinalStat()
	{
		return PlayerStatusFinal_;
	}

	const int PlayerGetCurHP()
	{
		return PlayerStatusFinal_.Stat_Hp_;
	}

	//void PlayerSetHP(int _Hp)
	//{
	//	PlayerStatusFinal_.Stat_Hp_ = _Hp;
	//}

	const float PlayerGetCurStamina()
	{
		return PlayerStatusFinal_.Stat_Stamina_;
	}

	//void PlayerSetStamina(float _Stamina)
	//{
	//	PlayerStatusFinal_.Stat_Hp_ = _Stamina;
	//}

	GameEngineActor* PlayerGetTarget()
	{
		return Target_;
	}

	const int PlayerGetAttackPower()
	{
		return PlayerStatusFinal_.Stat_AttackPower_;
	}

	Inventory* PlayerGetInventory()
	{
		return Inventory_;
	}

#pragma endregion
};

