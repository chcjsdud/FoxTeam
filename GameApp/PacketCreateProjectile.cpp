#include "PreCompile.h"
#include "PacketCreateProjectile.h"
#include "ePacketID.h"
#include "RioArrow.h"
#include <GameEngine/GameEngineCore.h>
#include "LumiaLevel.h"
#include "Character.h"
#include "RioDSkill.h"

PacketCreateProjectile::PacketCreateProjectile()
    : ownerIndex_(-1)
    , targetIndex_(-1)
    , damage_(0.0f)
    , speed_(0.0f)
    , waitTime_(0.0f)
    , scale_(float4(5.f, 150.f, 50.f))
    , projectileType_(0)
    , rotationY_(0.0f)
    , lifeTime_(0.0f)
{

}

PacketCreateProjectile::~PacketCreateProjectile()
{

}

void PacketCreateProjectile::MakeTargetProjectile(IUnit& _owner, float _damage, const float4& _position, float _speed, IUnit& _target)
{
    ownerIndex_ = _owner.GetIndex();
    targetIndex_ = _target.GetIndex();
    damage_ = _damage;
    position_ = _position;
    speed_ = _speed;
}

void PacketCreateProjectile::MakeNonTargetProjectile(IUnit& _owner, float _damage, const float4& _position, float _rotationY, float _speed)
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
    serializer_ << projectileType_;
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
    serializer_ >> projectileType_;
}

GameEnginePacketBase* PacketCreateProjectile::getUserObject()
{
    return new PacketCreateProjectile;
}

void PacketCreateProjectile::execute(SOCKET _socketSender, GameEngineSocketInterface* _network, bool _bServer)
{
    // 오너가 없으면 처리하지 않는다.
    if (ownerIndex_ == -1)
    {
        return;
    }

    // 루미아레벨 아니면 처리 안 함.
    LumiaLevel* level = dynamic_cast<LumiaLevel*>(GameEngineCore::CurrentLevel());
    if (nullptr == level)
    {
        return;
    }


    eProjectileType type = static_cast<eProjectileType>(projectileType_);
    switch (type)
    {
    case eProjectileType::RioTargetArrow:
        rioTargetArrow(level, _network, _bServer);
        break;
    case eProjectileType::RioWSkillShort:
        rioTargetArrow(level, _network, _bServer);
        break;
    case eProjectileType::RioWSkillLong:
        rioWSkillLong(level, _network, _bServer);
        break;
    case eProjectileType::RioRSkillShort:
        rioRSkillShort(level, _network, _bServer);
        break;
    case eProjectileType::RioRSkillShortImpact:
        rioRSkillShortImpact(level, _network, _bServer);
        break;
    case eProjectileType::RioRSkillLong:
        rioRSkillLong(level, _network, _bServer);
        break;
    case eProjectileType::RioDSkill:
        rioDSkill(level, _network, _bServer);
        break;
    default:
        break;
    }
}

void PacketCreateProjectile::rioTargetArrow(LumiaLevel* _level, GameEngineSocketInterface* _network, bool _bServer)
{
    std::vector<Character*> list = _level->GetCharacterActorList();
    RioArrow* arrow = _level->CreateActor<RioArrow>();

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
}

void PacketCreateProjectile::rioWSkillShort(LumiaLevel* _level, GameEngineSocketInterface* _network, bool _bServer)
{
    std::vector<Character*> list = _level->GetCharacterActorList();
    RioArrow* arrow = _level->CreateActor<RioArrow>();

    if (_bServer)
    {
        arrow->MakeNonTargetArrow(*list[ownerIndex_], damage_, position_, rotationY_, speed_);
        arrow->SetWaitTime(waitTime_);
        arrow->SetLifeTime(lifeTime_);
        arrow->SetScale(scale_);
        _network->Send(this);
    }
    else
    {
        arrow->MakeNonTargetArrow(*list[ownerIndex_], 0.0f, position_, rotationY_, speed_);
        arrow->SetWaitTime(waitTime_);
        arrow->SetLifeTime(lifeTime_);
        arrow->SetScale(scale_);
    }
}

void PacketCreateProjectile::rioWSkillLong(LumiaLevel* _level, GameEngineSocketInterface* _network, bool _bServer)
{
    std::vector<Character*> list = _level->GetCharacterActorList();
    RioArrow* arrow = _level->CreateActor<RioArrow>();

    if (_bServer)
    {
        arrow->MakeNonTargetArrow(*list[ownerIndex_], damage_, position_, rotationY_, speed_);
        arrow->SetWaitTime(waitTime_);
        arrow->SetLifeTime(lifeTime_);
        arrow->SetScale(scale_);
        _network->Send(this);
    }
    else
    {
        arrow->MakeNonTargetArrow(*list[ownerIndex_], 0.0f, position_, rotationY_, speed_);
        arrow->SetWaitTime(waitTime_);
        arrow->SetLifeTime(lifeTime_);
        arrow->SetScale(scale_);
    }
}

void PacketCreateProjectile::rioRSkillShort(LumiaLevel* _level, GameEngineSocketInterface* _network, bool _bServer)
{
    std::vector<Character*> list = _level->GetCharacterActorList();
    RioArrow* arrow = _level->CreateActor<RioArrow>();

    if (_bServer)
    {
        arrow->MakeNonTargetArrow(*list[ownerIndex_], damage_, position_, rotationY_, speed_);
        arrow->SetWaitTime(waitTime_);
        arrow->SetLifeTime(lifeTime_);
        arrow->SetScale(scale_);
        _network->Send(this);
    }
    else
    {
        arrow->MakeNonTargetArrow(*list[ownerIndex_], 0.0f, position_, rotationY_, speed_);
        arrow->SetWaitTime(waitTime_);
        arrow->SetLifeTime(lifeTime_);
        arrow->SetScale(scale_);
    }
}

void PacketCreateProjectile::rioRSkillShortImpact(LumiaLevel* _level, GameEngineSocketInterface* _network, bool _bServer)
{
    std::vector<Character*> list = _level->GetCharacterActorList();
    RioArrow* arrow = _level->CreateActor<RioArrow>();

    if (_bServer)
    {
        arrow->MakeNonTargetArrow(*list[ownerIndex_], damage_, position_, rotationY_, speed_);
        arrow->SetWaitTime(waitTime_);
        arrow->SetLifeTime(lifeTime_);
        arrow->SetScale(scale_);
        arrow->SetKnockback(true);
        _network->Send(this);
    }
    else
    {
        arrow->MakeNonTargetArrow(*list[ownerIndex_], 0.0f, position_, rotationY_, speed_);
        arrow->SetWaitTime(waitTime_);
        arrow->SetLifeTime(lifeTime_);
        arrow->SetScale(scale_);
        arrow->SetKnockback(true);
    }
}

void PacketCreateProjectile::rioRSkillLong(LumiaLevel* _level, GameEngineSocketInterface* _network, bool _bServer)
{
    std::vector<Character*> list = _level->GetCharacterActorList();
    RioArrow* arrow = _level->CreateActor<RioArrow>();

    if (_bServer)
    {
        arrow->MakeNonTargetArrow(*list[ownerIndex_], damage_, position_, rotationY_, speed_);
        arrow->SetWaitTime(waitTime_);
        arrow->SetLifeTime(lifeTime_);
        arrow->SetScale(scale_);
        arrow->SetKnockback(true);
        _network->Send(this);
    }
    else
    {
        arrow->MakeNonTargetArrow(*list[ownerIndex_], 0.0f, position_, rotationY_, speed_);
        arrow->SetWaitTime(waitTime_);
        arrow->SetLifeTime(lifeTime_);
        arrow->SetScale(scale_);
        arrow->SetKnockback(true);
    }
}

void PacketCreateProjectile::rioDSkill(LumiaLevel* _level, GameEngineSocketInterface* _network, bool _bServer)
{
    std::vector<Character*> list = _level->GetCharacterActorList();
    RioDSkill* arrow = _level->CreateActor<RioDSkill>();

    if (_bServer)
    {
        arrow->SetOwner(list[ownerIndex_]);
        arrow->SetWaitTime(waitTime_);
        arrow->SetDamage(damage_);
        _network->Send(this);
    }
    else
    {
        arrow->SetOwner(list[ownerIndex_]);
        arrow->SetWaitTime(waitTime_);
        arrow->SetDamage(0.0f);
    }
}
