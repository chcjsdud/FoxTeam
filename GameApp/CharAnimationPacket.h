#pragma once
#include <GameEngine/GameEnginePacketBase.h>

class CharAnimationPacket : public GameEnginePacketBase
{
public:
	CharAnimationPacket(); // default constructer ����Ʈ ������
	~CharAnimationPacket(); // default destructer ����Ʈ �Ҹ���
	CharAnimationPacket(const CharAnimationPacket& _other) = delete; // default Copy constructer ����Ʈ ���������
	CharAnimationPacket(CharAnimationPacket&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	CharAnimationPacket& operator=(const CharAnimationPacket& _other) = delete; // default Copy operator ����Ʈ ���� ������
	CharAnimationPacket& operator=(const CharAnimationPacket&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void SetTargetIndex(int _index);
	void SetAnimation(const std::string& _animation);

	std::string GetAnimation()
	{
		return curAnimation_;
	}


protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
	int targetIndex_;
	std::string curAnimation_;
};

