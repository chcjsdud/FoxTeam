#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 
// 용도 : 

class PJW_Mouse;
class PJW_Map;
class PJW_Player;
class PJW_Hyunwoo;
class PJW_GameController : public GameEngineActor
{
	friend GameEngineWindow;
	friend PJW_Hyunwoo;

public:
	PJW_GameController();
	~PJW_GameController(); 
	PJW_GameController(const PJW_GameController& _other) = delete; 
	PJW_GameController(PJW_GameController&& _other) noexcept; 
	PJW_GameController& operator=(const PJW_GameController& _other) = delete; 
	PJW_GameController& operator=(const PJW_GameController&& _other) = delete; 

public:
	virtual void Start() override;
	void Init_Player();
	void Init_Keys();
	
	
	virtual void Update(float _DeltaTime) override;
	void Check_MousePicking(float _DeltaTime);


private:
	std::vector<PJW_Player*> playerList_;
	// 플레이어 목록

	PJW_Player* curPlayer_;
	// 현재 컨트롤권을 가진 플레이어

	PJW_Map* map_;
	// 네브메시가 적용되는 맵

	PJW_Mouse* mouse_;

};

