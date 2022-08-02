#pragma once

#include "LH_Unit.h"
/// <summary>
/// 여우 주인공 액터
/// </summary>


// 플레이어블 캐릭터 부모 클레스

class GameMouse;
class GameEngineLevel;
class GameEngineImageRenderer;
class Player : public Unit
{
public:
	Player();
	~Player();

	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

private:
	// 인식해야 하는 마우스 액터 포인터
	GameMouse* targetMouse_;
	bool IsRockon_;
	// 이터널 리턴 추가 변수
	float4 targetPos_;
	// 하달받은 마우스 커서 우클릭 좌표
	float4 arrivalPos_;
	// 최종 도착 좌표

protected:
	//UI
	bool IsPlayer_;

	class UI_Skill* SkillUI_;
	class LockOnUI* LockOnUI_;
	//class Inventory* Inventory_;
#pragma region Player Status

	// 플레이어 레벨업 스텟
	Status Status_LevelUp_;
	// 직업 정보
	JobType JobType_;

#pragma endregion

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

#pragma region State
protected:
	// 캐릭터 State, 가상함수를 통해 캐릭터 클레스가 기본적으로 가지고 있어야 할 state를 지정
	virtual void Idle_Start() = 0;
	virtual void Idle_Update(float _DeltaTime) = 0;
	virtual void Idle_End() = 0;

	virtual void Walk_Start() = 0;
	virtual void Walk_Update(float _DeltaTime) = 0;
	virtual void Walk_End() = 0;

	virtual void Run_Start() = 0;
	virtual void Run_Update(float _DeltaTime) = 0;
	virtual void Run_End() = 0;

	virtual void Attack_Start() = 0;
	virtual void Attack_Update(float _DeltaTime) = 0;
	virtual void Attack_End() = 0;

	//void Stand_Start();
	//void Stand_Update(float _DeltaTime);
	//void Stand_End();

	//void Move_Start();
	//void Move_Update(float _DeltaTime);
	//void Move_End();

#pragma endregion

#pragma region MemberUpdateFunc
protected:

	// update류 함수에 같이 들어가 카메라 업데이트, 바로 뒤에서 바라보는 함수
	void CameraUpdate_BackPosition(float _DeltaTime);
	void CameraUpdate_UpPosition(float _DeltaTime);

	// 이터널 리턴용 추가 : 쿼터뷰 시점
	void CameraUpdate_EternalReturn(float _DeltaTime);


	//캐릭터가 바라보는 방향이 바로 바로 안변하고 천천히 변함(이동과는 무관하게), 
	//07.18 바라보는 방향을 넣어줘야함
	void MoveRotateUpdate(float _DeltaTime);

	//RockOn 중일때 방향 업데이트
	void RockOnDirUpdate(float _DeltaTime);


	void KeyDirUpdate(float _DeltaTime);
	// update류 함수에 같이 들어가 이동을 제어함
	void MoveUpdate(float _DeltaTime);
	void CurDirUpdate(float _DeltaTime);
	void RockOnUpdate(float _DeltaTime);

	void StaminaRecoverUpdate(float _DeltaTime);

#pragma endregion

#pragma region MemberFunc
private:
	const float4 CalculateTargetDir(float4 TargetPos)
	{
		float4 Dir = TargetPos - GetTransform()->GetWorldPosition();
		Dir.Normalize3D();

		return Dir;
	}

	//임시 구현 상태, 인벤토리에서 아이템을 가져오고, 아이템에서 이름과 스테이터스를 가져오게끔
	//bool EquipItem(std::string _BuffName, PlayerStatus* _PlayerStatus);

#pragma endregion

private:

#pragma region Player 외부함수
public:

	void SetParentMouse(GameMouse* _targetMouse)
	{
		targetMouse_ = _targetMouse;
	}

	void SetTargetPos(const float4 _targetPos)
	{
		targetPos_ = _targetPos;
		arrivalPos_ = { targetPos_.x + GetTransform()->GetWorldPosition().x, 0.0f,  targetPos_.y + GetTransform()->GetWorldPosition().y };
		IsMove_ = true;
	}
#pragma endregion
};

