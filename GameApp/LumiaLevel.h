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
	// 0920 박종원 삭제예정
	//Rio* player_;

	// 루미아레벨에서 고정적으로 관리하는 캐릭터 액터 리스트입니다.
	// 현재 Available 한 캐릭터는 리오밖에 없어서 Rio 로 고정했지만 모든 캐릭터 액터의 부모 액터가 대체할 예정입니다.
	// 모든 캐릭터의 정보는 PlayerInfoManager 에서 관리하고,
	// 이건 그냥 "캐릭터 액터" 의 리스트일 뿐입니다.
	std::vector<Rio*> characterActorList_;
	

	MousePointer* mouse_;
	LumiaMap* map_;

	ItemBoxManager* itemBox_;
};

