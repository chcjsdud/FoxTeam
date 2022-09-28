#pragma once

#include <GameEngine/GameEngineLevel.h>

#include <GameApp/MousePointer.h>
#include <GameApp/ItemBoxManager.h>
#include <GameApp/Character.h>

#define SERVER

class Character;
class LumiaMap;
class NavMesh;
class LumiaLevel : public GameEngineLevel
{
public:
	LumiaLevel();
	~LumiaLevel();
	LumiaLevel(const LumiaLevel& _other) = delete;
	LumiaLevel(LumiaLevel&& _other) = delete;
	LumiaLevel& operator=(const LumiaLevel& _other) = delete;
	LumiaLevel& operator=(const LumiaLevel&& _other) = delete;

public:
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;

public:
	void GenerateCharactor();

public:
	MousePointer* GetMousePointer() { return MousePointer::InGameMouse; }
	LumiaMap* GetMap() { return map_; }
	ItemBoxManager* GetItemBoxManager() { return itemBox_; }
	std::vector<Character*> GetCharacterActorList() { return characterActorList_; }
private:
	// On level change start event
	void loadResource();
	void initRenderWindow();
	void createActor();
	void adjustCamera();
	// 0922 ������
	void serverCheck();

	// On level change end event
	void releaseResource();
	void releaseRenderWindow();

private:
	// ��̾Ʒ������� ���������� �����ϴ� ĳ���� ���� ����Ʈ�Դϴ�.
	// ���� Available �� ĳ���ʹ� �����ۿ� ��� Rio �� ���������� ��� ĳ���� ������ �θ� ���Ͱ� ��ü�� �����Դϴ�.
	// ��� ĳ������ ������ PlayerInfoManager ���� �����ϰ�,
	// �̰� �׳� "ĳ���� ����" �� ����Ʈ�� ���Դϴ�.
	std::vector<Character*> characterActorList_;
	
	LumiaMap* map_;

	ItemBoxManager* itemBox_;
};

