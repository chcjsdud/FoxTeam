#include "PreCompile.h"
#include "CharEffectPacket.h"
#include "Character.h"
#include "LumiaLevel.h"
#include <GameEngine/GameEngineCore.h>

#include "ePacketID.h"
#include "PlayerInfoManager.h"
CharEffectPacket::CharEffectPacket() // default constructer 디폴트 생성자
	: targetIndex_(-1), victimIndex_(-1)
{

}

CharEffectPacket::~CharEffectPacket() // default destructer 디폴트 소멸자
{

}

CharEffectPacket::CharEffectPacket(CharEffectPacket&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void CharEffectPacket::SetTargetIndex(int _index)
{
    targetIndex_ = _index;
}

void CharEffectPacket::SetVictimIndex(int _index)
{
	victimIndex_ = _index;
}

void CharEffectPacket::SetAnimationName(const std::string& _animation)
{
    effectAnimationName_ = _animation;
}

void CharEffectPacket::userSerialize()
{
	serializer_ << targetIndex_;
	serializer_ << effectAnimationName_;
	serializer_ << victimIndex_;
}

void CharEffectPacket::userDeserialize()
{
	serializer_ >> targetIndex_;
	serializer_ >> effectAnimationName_;
	serializer_ >> victimIndex_;
}

void CharEffectPacket::initPacketID()
{
	SetPacketID(ePacketID::CharEffectPacket);
}

GameEnginePacketBase* CharEffectPacket::getUserObject()
{
    return new CharEffectPacket;
}

void CharEffectPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{

	LumiaLevel* level = dynamic_cast<LumiaLevel*>(GameEngineCore::CurrentLevel());

	// 똑같이 나와야하는데
	// 클라이언트에서 생성한 이 패킷은 

	if (nullptr == level)
	{
		GameEngineDebug::MsgBoxError("이팩트 패킷 execute 도중 레벨을 가져오는 데 실패했습니다.");
		return;
	}

	Character* targetChar = level->GetCharacterActorList()[targetIndex_];


	if (-1 != victimIndex_)
	{
		Character* victimChar = level->GetCharacterActorList()[victimIndex_];
		targetChar->PlayEffect(effectAnimationName_, targetIndex_ , victimChar);
	}
	else 
	{
		targetChar->PlayEffect(effectAnimationName_, targetIndex_);
	}


	if (_bServer)
	{
		_network->Send(this);
	}
}
