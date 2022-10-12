#include "PreCompile.h"
#include "CharEffectPacket.h"
#include "Character.h"
#include "LumiaLevel.h"
#include <GameEngine/GameEngineCore.h>
CharEffectPacket::CharEffectPacket() // default constructer ����Ʈ ������
{

}

CharEffectPacket::~CharEffectPacket() // default destructer ����Ʈ �Ҹ���
{

}

CharEffectPacket::CharEffectPacket(CharEffectPacket&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
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
}

void CharEffectPacket::userDeserialize()
{
}

void CharEffectPacket::initPacketID()
{
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
		return;
	}

	if (_bServer)
	{
		_network->Send(this);
	}

	const std::vector<Character*>& list = level->GetCharacterActorList();

	if (list.size() <= targetIndex_ || targetIndex_ < 0)
	{
		return;
	}

	Character* character = list[targetIndex_];
	if (character == nullptr)
	{
		return;
	}

	//character->PlayEffect(effectAnimationName_);

}
