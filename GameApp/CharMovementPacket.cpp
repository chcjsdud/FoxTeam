#include "PreCompile.h"
#include "CharMovementPacket.h"

CharMovementPacket::CharMovementPacket() // default constructer 디폴트 생성자
{

}

CharMovementPacket::~CharMovementPacket() // default destructer 디폴트 소멸자
{

}

CharMovementPacket::CharMovementPacket(CharMovementPacket&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
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

