#pragma once
#include <GameEngine/GameEngineLevel.h>

#include "MousePointer.h"
#include "ItemBoxManager.h"

#define SERVER

// 분류 : 레벨(씬)
// 용도 : 
// 설명 : 플레이 레벨
class LumiaMap;
class NavMesh;
class Character;
class Monsters;
class GameEngineLevelControlWindow;
class LumiaLevel : public GameEngineLevel
{
public: // Inline Get Function
	inline MousePointer* GetMousePointer() const
	{
		return MousePointer::InGameMouse;
	}

	inline LumiaMap* GetMap() const
	{
		return CurMap_;
	}

	inline ItemBoxManager* GetItemBoxManager() const
	{
		return ItemBoxManager_;
	}

	inline std::vector<Character*> GetCharacterActorList()
	{
		return CharacterActorList_;
	}

public: // Inline Set Function

public: // Public Function
	void HostCreateCommand();												// Host 전용
	void GuestCreateCommand();												// Guest 전용

public: // Create MonsterInfo
	void CreateMonsterInfo();												// Host 전용(현재 맵에 배치하려는 몬스터들을 미리 셋팅)
	// 몬스터가 스폰되는 지역과 위치가 해당 함수에서 모두 결정난다.

private: // Command
	void HostAllCreationCommand();											// Host 전용
	void GuestAllCreationCommand();											// Guest 전용

private: // Command
	void MapCreationCommand();												// Host & Guest 공통
	void MonsterCreationCommand();											// Host & Guest 공통
	void CharacterCreationCommand();										// Host & Guest 공통

private: // Resource Load
	void MapResourceLoad();													// Current Map Resource Load
	void CharacterResourceLoad();											// InGame Play Character Resource Load

private: // Basic Initalize Function
	void CreateBasicActor();												// LumiaLevel Basic Actore Creation Function
	void CreateLevelInput();												// LumiaLevel Input Creation Function
	void AddSocketHandle();													// LumiaLevel Network Handle Add Function
	void InitIMGUIWindow();													// LumiaLevel IMGUI Window Setting Function
	void CameraAdjustment();												// LumiaLevel Main Camera Adjustment Function

private: // State Update Function
	void CharacterStateUpdatePacketSend();									// Character State Update Packet Send Function
	void CharactersTransformUpdate();										// Character ManagerList Transform Update Function
	void MonsterStateUpdatePacketSend();									// Monster State Update Packet Send Function
	void MonstersTransformUpdate();											// Monster ManagerList Transform Update Function

private: // Debug Function
	void DebugWindowUpdate();												// Debug Window Update Function

#pragma region 테스트용함수(추후삭제예정)
	// LevelControlWindow에의해 강제 레벨이동한경우 사용하는 함수
	// 서버 생성이 없이 해당 레벨로 체인지한경우
private:
	void Test_loadResource();
	void Test_initRenderWindow();
	void Test_createActor();
	void Test_adjustCamera();
	void Test_serverCheck();
	void Test_releaseRenderWindow();
	void Test_releaseResource();

private:
	void Test_GenerateCharactor();
#pragma endregion

public:
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;

public:
	LumiaLevel();
	~LumiaLevel();

protected:
	LumiaLevel(const LumiaLevel& _other) = delete;
	LumiaLevel(LumiaLevel&& _other) = delete;

private:
	LumiaLevel& operator=(const LumiaLevel& _other) = delete;
	LumiaLevel& operator=(const LumiaLevel&& _other) = delete;

private:
	std::vector<Character*> CharacterActorList_;							// 캐릭터 목록(실질적 데이터는 PlayerInfoManager에서 관리)
	std::vector<Monsters*> MonsterActorList_;								// 몬스터 목록(실직적 데이터는 MonsterInfoManager에서 관리)

	// Area, Position
	std::map<std::string, std::vector<float4>> characterSpawnPoints_;

private:
	LumiaMap* CurMap_;														// 맵
	ItemBoxManager* ItemBoxManager_;										// 아이템박스 매니저

private:
	GameEngineLevelControlWindow* DebugAndControlWindow_;					// 디버그 & 강제레벨체인지 윈도우
};
