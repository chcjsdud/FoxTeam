#include "PreCompile.h"
#include "CharMovementPacket.h"

CharMovementPacket::CharMovementPacket() // default constructer ����Ʈ ������
{

}

CharMovementPacket::~CharMovementPacket() // default destructer ����Ʈ �Ҹ���
{

}

CharMovementPacket::CharMovementPacket(CharMovementPacket&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}

void CharMovementPacket::SetTargetIndex(int _index)
{
}

void CharMovementPacket::SetPos(float4 _character)
{
}

void CharMovementPacket::SetDirection(float4 _startPoint)
{
}

void CharMovementPacket::userSerialize()
{
}

void CharMovementPacket::userDeserialize()
{
}

void CharMovementPacket::initPacketID()
{
}

GameEnginePacketBase* CharMovementPacket::getUserObject()
{
	return nullptr;
}

void CharMovementPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
}

