#pragma once
#include <GameEngine/GameEnginePacketBase.h>

// �з� : 
// �뵵 : 
// ���� : 
class CharSelectPacket : public GameEnginePacketBase
{
public:
	CharSelectPacket(); 
	~CharSelectPacket();
	CharSelectPacket(const CharSelectPacket& _other) = delete;
	CharSelectPacket(CharSelectPacket&& _other) noexcept;
	CharSelectPacket& operator=(const CharSelectPacket& _other) = delete;
	CharSelectPacket& operator=(const CharSelectPacket&& _other) = delete;

public:
	void SetTargetPlayer(int _index);
	void SetCharacter(int _character);
	void SetStartPoint(int _startPoint);
	void SetIsReady(bool _isReady);

	int GetCharacter()
	{
		return character_;
	}

	int GetStartPoint()
	{
		return startPoint_;
	}

	bool GetIsReady()
	{
		return isReady_;
	}

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
	// ������ ���ҷ� �־�� �ϳ�?
	// 1. � ĳ���͸� �����߳�?
	// 2. � ������ ���� �������� �����߳�?
	int targetIndex_;
	int character_;
	int startPoint_;
	bool isReady_;
	// ������ �س����� ���� enum �̵� ���ǵ� ������ �ٲ� ��.
};

