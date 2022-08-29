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
	void SetCharacter(int _character);
	void SetStartPoint(int _startPoint);

	int GetCharacter()
	{
		return character_;
	}

	int GetStartPoint()
	{
		return startPoint_;
	}

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(bool _bServer, GameEngineSocketInterface* _network) override;

private:
	// ������ ���ҷ� �־�� �ϳ�?
	// 1. � ĳ���͸� �����߳�?
	// 2. � ������ ���� �������� �����߳�?
	int character_;
	int startPoint_;
	// ������ �س����� ���� enum �̵� ���ǵ� ������ �ٲ� ��.
};

