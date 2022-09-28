#pragma once
#include <GameEngine/GameEngineLevel.h>

#include "MousePointer.h"
#include "ItemBoxManager.h"

#define SERVER

// �з� : ����(��)
// �뵵 : 
// ���� : �÷��� ����
class LumiaMap;
class NavMesh;
class Character;
class Monsters;
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
	void OtherLevelCommand(LoadingType _LoadingType);						// �ε�Ÿ�� ����
	void ClientMapCreation();												// Guest ����
	void ClientMonsterCreation();											// Guest ����
	void ClientCharacterCreation();											// Guest ����

private: // Packet
	void MapCreationPacketSend();											// �ʻ��� ��Ŷ ����
	void MonsterCreationPacketSend();										// ���ͻ��� ��Ŷ ����
	void CharacterCreationPacketSend();										// ĳ���ͻ��� ��Ŷ ����

private: // Command
	bool MapCreationCommand();												// Host & Guest ����
	bool MonsterCreationCommand();											// Host ����
	bool CharacterCreationCommand();										// Host & Guest ����

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
	std::vector<Character*> CharacterActorList_;							// ĳ���� ���(������ �����ʹ� PlayerInfoManager���� ����)
	std::vector<Monsters*> MonsterActorList_;								// ���� ���(������ �����ʹ� MonsterInfoManager���� ����)

private:
	LumiaMap* CurMap_;														// ��
	ItemBoxManager* ItemBoxManager_;										// �����۹ڽ� �Ŵ���

private:
	GameEngineLevelControlWindow* DebugAndControlWindow_;					// ����� & ��������ü���� ������
};
