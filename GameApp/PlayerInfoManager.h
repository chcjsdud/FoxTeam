#pragma once

#include "PlayerInfo.h"
#include "Rio.h"

class PlayerInfoManager
{
public:
	PlayerInfoManager(); 
	~PlayerInfoManager();

	PlayerInfoManager(const PlayerInfoManager& _other) = delete; 
	PlayerInfoManager(PlayerInfoManager&& _other) = delete; 

	PlayerInfoManager& operator=(const PlayerInfoManager& _other) = delete;
	PlayerInfoManager& operator=(const PlayerInfoManager&& _other) = delete;

public:
	static PlayerInfoManager* GetInstance() { static PlayerInfoManager instance; return &instance; }

public:
	void						AddNewPlayer(PlayerInfo _playerInfo);


	std::vector<PlayerInfo>&	GetPlayerList()								{ return serverPlayerList_; }
	int							GetMyNumber()								{ return playerNumber_; }
	std::string					GetNickname()						     	{ return playerName_; }
	Rio*						GetMainCharacter()							{ return myCharacter_; }

	void						SetPlayerNumber(int _playerNumber)			{ playerNumber_ = _playerNumber; }
	void						SetPlayerNickname(std::string _nickname)	{ playerName_ = _nickname; }
	void						SetMainCharacter(Rio* _character)			{ myCharacter_ = _character; }
private:
	std::vector<PlayerInfo> serverPlayerList_;
	int playerNumber_;
	Rio* myCharacter_;

	// 0914 ������
	std::string playerName_;
};

