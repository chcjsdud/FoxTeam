#pragma once

#include <GameEngine/GameEngineLevel.h>

#include <GameApp/MousePointer.h>
#include <GameApp/ItemBoxManager.h>
#include <GameApp/GHRio.h>

#define SERVER

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
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

public:
	void GenerateCharactor();

public:
	MousePointer* GetMousePointer() { return mouse_; }
	LumiaMap* GetMap() { return map_; }
	ItemBoxManager* GetItemBoxManager() { return itemBox_; }
private:
	// On level change start event
	void loadResource();
	void initRenderWindow();
	void createActor();
	void adjustCamera();

	// On level change end event
	void releaseResource();
	void releaseRenderWindow();

private:
	// 0920 ������ ��������
	//Rio* player_;

	// ��̾Ʒ������� ���������� �����ϴ� ĳ���� ���� ����Ʈ�Դϴ�.
	// ���� Available �� ĳ���ʹ� �����ۿ� ��� Rio �� ���������� ��� ĳ���� ������ �θ� ���Ͱ� ��ü�� �����Դϴ�.
	// ��� ĳ������ ������ PlayerInfoManager ���� �����ϰ�,
	// �̰� �׳� "ĳ���� ����" �� ����Ʈ�� ���Դϴ�.
	std::vector<Rio*> characterActorList_;
	

	MousePointer* mouse_;
	LumiaMap* map_;

	ItemBoxManager* itemBox_;
};

