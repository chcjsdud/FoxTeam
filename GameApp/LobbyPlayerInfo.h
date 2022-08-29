#pragma once


class LobbyPlayerInfo 
{
public:
	LobbyPlayerInfo(); 
	~LobbyPlayerInfo();
	LobbyPlayerInfo(const LobbyPlayerInfo& _other) = delete;
	LobbyPlayerInfo(LobbyPlayerInfo&& _other) noexcept; 
	LobbyPlayerInfo& operator=(const LobbyPlayerInfo& _other) = delete;
	LobbyPlayerInfo& operator=(const LobbyPlayerInfo&& _other) = delete;

public:
	void SetPlayerNumber(int _playerNumber);
	void SetCharacter(int _character);
	void SetStartPoint(int _startPoint);

	int GetPlayerNumber()
	{
		return playerNumber_;
	}

	int GetCharacter()
	{
		return character_;
	}

	int GetStartPoint()
	{
		return startPoint_;
	}

protected:

private:
	int playerNumber_;
	int character_;
	int startPoint_;
};

