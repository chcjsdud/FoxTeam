#include "PreCompile.h"
#include "PacketCreateProjectile.h"
#include "ePacketID.h"
#include "RioArrow.h"
#include <GameEngine/GameEngineCore.h>
#include "LumiaLevel.h"

PacketCreateProjectile::PacketCreateProjectile()
    : ownerIndex_(-1)
    , targetIndex_(-1)
    , damage_(0.0f)
    , speed_(0.0f)
    , waitTime_(0.0f)
    , scale_(float4(5.f, 150.f, 50.f))
    , bKnockback_(false)
    , rotationY_(0.0f)
    , lifeTime_(0.0f)
{

}

PacketCreateProjectile::~PacketCreateProjectile()
{

}

void PacketCreateProjectile::MakeTargetArrow(Character& _owner, float _damage, const float4& _position, float _speed, Character& _target)
{
    ownerIndex_ = _owner.GetIndex();
    targetIndex_ = _target.GetIndex();
    damage_ = _damage;
    position_ = _position;
    speed_ = _speed;
}

void PacketCreateProjectile::MakeNonTargetArrow(Character& _owner, float _damage, const float4& _position, float _rotationY, float _speed)
{
    ownerIndex_ = _owner.GetIndex();
    damage_ = _damage;
    position_ = _position;
    speed_ = _speed;
    rotationY_ = _rotationY;
}

void PacketCreateProjectile::initPacketID()
{
    SetPacketID(ePacketID::PacketCreateProjectile);
}

void PacketCreateProjectile::userSerialize()
{
    serializer_ << ownerIndex_;
    serializer_ << targetIndex_;
    serializer_ << damage_;
    serializer_ << position_;
    serializer_ << speed_;
    serializer_ << rotationY_;
    serializer_ << waitTime_;
    serializer_ << lifeTime_;
    serializer_ << scale_;
    serializer_ << bKnockback_;
}

void PacketCreateProjectile::userDeserialize()
{
    serializer_ >> ownerIndex_;
    serializer_ >> targetIndex_;
    serializer_ >> damage_;
    serializer_ >> position_;
    serializer_ >> speed_;
    serializer_ >> rotationY_;
    serializer_ >> waitTime_;
    serializer_ >> lifeTime_;
    serializer_ >> scale_;
    serializer_ >> bKnockback_;
}

GameEnginePacketBase* PacketCreateProjectile::getUserObject()
{
    return new PacketCreateProjectile;
}

void PacketCreateProjectile::execute(SOCKET _socketSender, GameEngineSocketInterface* _network, bool _bServer)
{
    if (ownerIndex_ == -1)
    {
        return;
    }

    LumiaLevel* level = dynamic_cast<LumiaLevel*>(GameEngineCore::CurrentLevel());

    if (nullptr == level)
    {
        return;
    }

    RioArrow* arrow = level->CreateActor<RioArrow>();

    std::vector<Character*> list = level->GetCharacterActorList();

    // 타겟이 있으면 타게팅
    if (targetIndex_ != -1)
    {
        if (_bServer)
        {
            arrow->MakeTargetArrow(*list[ownerIndex_], damage_, position_, speed_, *list[targetIndex_]);
            arrow->SetWaitTime(waitTime_);
            _network->Send(this);
        }
        else
        {
            arrow->MakeTargetArrow(*list[ownerIndex_], 0.0f, position_, speed_, *list[targetIndex_]);
            arrow->SetWaitTime(waitTime_);
        }
    }
    else // 여긴 논 타게팅
    {
        if (_bServer)
        {
            arrow->MakeNonTargetArrow(*list[ownerIndex_], damage_, position_, rotationY_, speed_);
            arrow->SetWaitTime(waitTime_);
            arrow->SetLifeTime(lifeTime_);
            arrow->SetScale(scale_);
            arrow->SetKnockback(static_cast<bool>(bKnockback_));
            _network->Send(this);
        }
        else
        {
            arrow->MakeNonTargetArrow(*list[ownerIndex_], 0.0f, position_, rotationY_, speed_);
            arrow->SetWaitTime(waitTime_);
            arrow->SetLifeTime(lifeTime_);
            arrow->SetScale(scale_);
            arrow->SetKnockback(static_cast<bool>(bKnockback_));
        }
    }


}
