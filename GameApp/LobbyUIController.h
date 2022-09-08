#pragma once
#include <GameEngine/GameEngineActor.h>
// �з� : 
// �뵵 : �κ񷹺��� �� UI���͸� ��Ƶδ� ��
// ���� : 



class Lobby_ButtonOne;
class Lobby_ButtonTwo;
class Lobby_PortraitBg;
class LobbyBackground;
class Lobby_CharFull;
class LobbyUIController : public GameEngineActor
{
public:
	LobbyUIController(); // default constructer ����Ʈ ������
	~LobbyUIController(); // default destructer ����Ʈ �Ҹ���
	LobbyUIController(const LobbyUIController& _other) = delete; // default Copy constructer ����Ʈ ���������
	LobbyUIController(LobbyUIController&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	LobbyUIController& operator=(const LobbyUIController& _other) = delete; // default Copy operator ����Ʈ ���� ������
	LobbyUIController& operator=(const LobbyUIController&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void InitUI();
	Lobby_PortraitBg* GetPortraitVector(int _index);
	Lobby_ButtonOne* GetHostButton() { return hostButtonUI_; }
	Lobby_ButtonTwo* GetClientButton() { return clientButtonUI_; }

public:
	//���� �� ĳ���Ͱ� ���� ������ ���� static ����
	static JobType SelectedChar;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	LobbyBackground* backGroundUI_;
	Lobby_ButtonOne* hostButtonUI_;
	Lobby_ButtonTwo* clientButtonUI_;
	Lobby_CharFull* charFullUI_;
	std::vector<Lobby_PortraitBg*> PortraitBg;


};

