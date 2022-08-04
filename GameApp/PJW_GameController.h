#pragma once
#include <GameEngine/GameEngineActor.h>

// �з� : 
// �뵵 : 

class PJW_Map;
class PJW_Player;
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
	void Init_Player();
	void Init_Keys();
	
	
	virtual void Update(float _DeltaTime) override;
	void Check_Input(float _DeltaTime);


private:
	std::vector<PJW_Player*> playerList_;
	// �÷��̾� ���

	PJW_Player* curPlayer_;
	// ���� ��Ʈ�ѱ��� ���� �÷��̾�

	PJW_Map* map_;
	// �׺�޽ð� ����Ǵ� ��

};

