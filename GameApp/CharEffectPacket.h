#pragma once
#include <GameEngine/GameEnginePacketBase.h>
// �з� : 
// �뵵 : 
// ���� : 
class CharEffectPacket : public GameEnginePacketBase
{
public:
	CharEffectPacket(); // default constructer ����Ʈ ������
	~CharEffectPacket(); // default destructer ����Ʈ �Ҹ���
	CharEffectPacket(const CharEffectPacket& _other) = delete; // default Copy constructer ����Ʈ ���������
	CharEffectPacket(CharEffectPacket&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	CharEffectPacket& operator=(const CharEffectPacket& _other) = delete; // default Copy operator ����Ʈ ���� ������
	CharEffectPacket& operator=(const CharEffectPacket&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void SetTargetIndex(int _index);
	void SetAnimationName(const std::string& _animation);
	void SetOverrideAnimation(const std::string& _overrideAnimationName, const std::string& _boneNameToAffect);

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
	int targetIndex_;
	std::string effectAnimationName_;
};

