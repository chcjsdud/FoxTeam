#pragma once
#include "Enums.h"

// 분류 : 컨트롤러
// 용도 : 전반적인 게임 관리
// 설명 : 
//        1) 해당 컨트롤러 초기화시점에 맵을 포함한 모든 오브젝트(액터)들을 생성과 동시에 배치하며 완료시 게임시작
//        2) UI는 해당 컨트롤러에 접근하여 메인캐릭터 스위칭을 수행
//           -. 메인플레이어 스위칭 시점 모든 캐릭터는 대기상태로 돌입 즉, 게임일시정지상태로 유지하고 스위칭한다.
//           -. 스위칭 완료 시점의 메인플레이어의 정보를 UI에 전달하여 UI정보를 갱신한다.
//        3) 패턴정보는 각각의 캐릭터가 가진다.
//           -. 각각의 캐릭터는 Characters 클래스를 상속받아처리하며, 고유 스킬정보는 캐릭터내에서 셋팅하여 사용한다.
//           -. 각각의 캐릭터는 AI이며 시야범위밖에 존재할때 아이템파밍패턴을 실행한다.
//           -. 각각의 캐릭터는 기본 커맨드 패턴을 가지며 특정상황에 따라 각각 패턴을 변환한다.(AI 타입일때)
//              - 적발견(시야범위내 적 진입) : 적공격가능범위까지 접근패턴 실행
//              - 적과의 전투(공격 패턴 실행) : 각 스킬에는 쿨타임존재
//                -> 모든 스킬 사용가능 : 랜덤으로 스킬실행(사용된 스킬은 쿨타임진행)
//                -> 모든 스킬 사용불가 : 일반공격 실행
//              - 적과의 전투중 일정체력이 되면 현재 타겟팅인 적의 체력과 비교하여 더 적다면 도망패턴을 실행한다.
//        4) 몬스터, 오브젝트가 가지는 아이템은 랜덤으로 배치
//        5) 결론적으로 컨트롤러는 맵을 포함한 맵에 존재하는 모든 오브젝트를 관리하며 UI와의 상호작용을 처리한다.
class Characters;
class Monsters;
class Objects;
class GameEngineLevel;
class Controller
{
private:
	static Controller* Inst;

public:
	static Controller& GetInst()
	{
		return *Inst;
	}

	static void Destory()
	{
		if (nullptr != Inst)
		{
			delete Inst;
			Inst = nullptr;
		}
	}

private:
	GameEngineLevel* CurrentLevel_;

private: // 게임내에 존재하는 모든 캐릭터(AI, 메인)과 몬스터를 관리
	std::list<Objects*> Objects_;
	std::list<Monsters*> Monsters_;
	std::list<Characters*> Characters_;

private: // 모든 액터 배치를 위해 맵크기 저장
	float4 MapSize_;

private:


public:
	Controller();
	~Controller();

protected:
	Controller(const Controller& _other) = delete;
	Controller(Controller&& _other) noexcept = delete;

private:
	Controller& operator=(const Controller& _other) = delete;
	Controller& operator=(const Controller&& _other) = delete;

#pragma region 게임시작 준비
public: // 게임시작 전 초기화 및 생성
	void Initialize();

private: // 맵로드
	void LoadMap();

private: // 오브젝트 & 몬스터 & 캐릭터 생성
	void CreateObject();
	void CreateMonster();
	void CreateCharacter();

private: // 모든 액터 배치
	void AllActorArrange();

private: // 생성완료된 모든 오브젝트 및 몬스터, AI 배치
	void ArrangeObjects();
	void ArrangeCharacters();
	void ArrangeMonsters();

#pragma endregion

#pragma region 스위칭관련
public: // UI로부터 수신
	void MainPlayerSwitching(Characters* _SelCharacter);	// 현재 선택된 캐릭터로 제어권한 부여 및 카메라 이동

#pragma endregion

#pragma region ...



#pragma endregion
};

