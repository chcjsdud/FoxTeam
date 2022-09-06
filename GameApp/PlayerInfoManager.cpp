#include "PreCompile.h"
#include "PlayerInfoManager.h"

PlayerInfoManager::PlayerInfoManager()
{
}

PlayerInfoManager::~PlayerInfoManager()
{

}

void PlayerInfoManager::AddNewPlayer(PlayerInfo _playerInfo)
{
	serverPlayerList_.push_back(_playerInfo);
}
