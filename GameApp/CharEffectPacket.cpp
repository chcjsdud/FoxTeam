#include "PreCompile.h"
#include "CharEffectPacket.h"
#include "Character.h"
#include "LumiaLevel.h"
#include <GameEngine/GameEngineCore.h>

#include "ePacketID.h"
#include "PlayerInfoManager.h"
CharEffectPacket::CharEffectPacket() // default constructer ����Ʈ ������
	: targetIndex_(-1), victimIndex_(-1)
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

	// �Ȱ��� ���;��ϴµ�
	// Ŭ���̾�Ʈ���� ������ �� ��Ŷ�� 

	if (nullptr == level)
	{
		GameEngineDebug::MsgBoxError("����Ʈ ��Ŷ execute ���� ������ �������� �� �����߽��ϴ�.");
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
