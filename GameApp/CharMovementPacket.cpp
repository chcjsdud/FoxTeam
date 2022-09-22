#include "PreCompile.h"
#include "CharMovementPacket.h"

#include "ePacketID.h"
#include "PlayerInfoManager.h"

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
	targetIndex_ = _index;
}

void CharMovementPacket::SetPos(float4 _pos)
{
	pos_ = _pos;
}

void CharMovementPacket::SetDirection(float4 _dir)
{
	dir_ = _dir;
}

void CharMovementPacket::userSerialize()
{
	serializer_ << targetIndex_;
	serializer_ << pos_;
	serializer_ << dir_;
}

void CharMovementPacket::userDeserialize()
{
	serializer_ >> targetIndex_;
	serializer_ >> pos_;
	serializer_ >> dir_;
}

void CharMovementPacket::initPacketID()
{
	SetPacketID(ePacketID::CharMovementPacket);
}

GameEnginePacketBase* CharMovementPacket::getUserObject()
{
	return new CharMovementPacket;
}

void CharMovementPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
	PlayerInfoManager::GetInstance()->GetPlayerList()[targetIndex_].curPos_ = pos_;
	PlayerInfoManager::GetInstance()->GetPlayerList()[targetIndex_].curDir_ = dir_;

	if (_bServer)
	{
		_network->Send(this);
	}
}

