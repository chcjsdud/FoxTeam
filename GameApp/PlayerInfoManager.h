#pragma once

#include "PlayerInfo.h"

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

	void						SetPlayerNumber(int _playerNumber)			{ playerNumber_ = _playerNumber; }
private:
	std::vector<PlayerInfo> serverPlayerList_;
	int playerNumber_;
};

