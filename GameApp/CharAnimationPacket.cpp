#include "PreCompile.h"
#include "CharAnimationPacket.h"

#include "ePacketID.h"
#include "PlayerInfoManager.h"

CharAnimationPacket::CharAnimationPacket() // default constructer ����Ʈ ������
{

}

CharAnimationPacket::~CharAnimationPacket() // default destructer ����Ʈ �Ҹ���
{

}

CharAnimationPacket::CharAnimationPacket(CharAnimationPacket&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}

void CharAnimationPacket::SetTargetIndex(int _index)
{
    targetIndex_ = _index;
}

void CharAnimationPacket::SetAnimation(const std::string& _animation)
{
    curAnimation_ = _animation;

}

void CharAnimationPacket::userSerialize()
{
    serializer_ << targetIndex_;
    serializer_ << curAnimation_;
}

void CharAnimationPacket::userDeserialize()
{
    serializer_ >> targetIndex_;
    serializer_ >> curAnimation_;
}

void CharAnimationPacket::initPacketID()
{
    SetPacketID(ePacketID::CharAnimationPacket);
}

GameEnginePacketBase* CharAnimationPacket::getUserObject()
{
    return new CharAnimationPacket;
}

void CharAnimationPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
    pm->GetPlayerList()[targetIndex_].curAnimation_ = curAnimation_;

    if (_bServer)
    {
        _network->Send(this);
    }
}

