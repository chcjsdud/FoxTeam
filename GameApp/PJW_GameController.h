#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 
// 용도 : 
// 설명 :
class PJW_Hyunwoo;
class PJW_GameController : public GameEngineActor
{
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
	void Init_Actor();

	virtual void Update(float _DeltaTime) override;
	

private:
	std::vector<GameEngineActor*> playerList_;

	GameEngineActor* p1_;
	GameEngineActor* p2_;

};

