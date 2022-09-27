#include "PreCompile.h"
#include "CharMovementPacket.h"

#include "ePacketID.h"
#include "PlayerInfoManager.h"

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
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	pm->GetPlayerList()[targetIndex_].curPos_ = pos_;
	pm->GetPlayerList()[targetIndex_].curDir_ = dir_;
	

	if (_bServer)
	{
		_network->Send(this);
	}
}
