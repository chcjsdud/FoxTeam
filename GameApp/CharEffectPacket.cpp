#include "PreCompile.h"
#include "CharEffectPacket.h"
#include "Character.h"
#include "LumiaLevel.h"
#include <GameEngine/GameEngineCore.h>

#include "ePacketID.h"
#include "PlayerInfoManager.h"
CharEffectPacket::CharEffectPacket() // default constructer 디폴트 생성자
	: targetIndex_(-1)
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

void CharEffectPacket::SetAnimationName(const std::string& _animation)
{
    effectAnimationName_ = _animation;
}

void CharEffectPacket::userSerialize()
{
	serializer_ << targetIndex_;
	serializer_ << effectAnimationName_;
}

void CharEffectPacket::userDeserialize()
{
	serializer_ >> targetIndex_;
	serializer_ >> effectAnimationName_;
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

	if (nullptr == level)
	{
		GameEngineDebug::MsgBoxError("이팩트 패킷 execute 도중 레벨을 가져오는 데 실패했습니다.");
		return;
	}

	Character* targetChar = level->GetCharacterActorList()[targetIndex_];

	if ("StunEffect" == effectAnimationName_)
	{
		int a = 0;
	}

	targetChar->PlayEffect(effectAnimationName_);

	if (_bServer)
	{
		_network->Send(this);
	}

	// 이 패킷은 한번에 한 렌더러의 이펙트만을 접근할 수 있는가?
	// 차라리 그렇게 만드는 게 편할려나?
	// 이것도 실시간으로 process 하는 게 편할 것.

	// 이펙트는 캐릭터마다 어떻게, 몇 개의 렌더러를 사용해서, 몇 초동안 표현할지가 각각 다를 것.
	// 

}
