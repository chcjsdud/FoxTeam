#include "PreCompile.h"
#include "NoticeBroadcastPacket.h"

NoticeBroadcastPacket::NoticeBroadcastPacket() // default constructer ����Ʈ ������
{

}

NoticeBroadcastPacket::~NoticeBroadcastPacket() // default destructer ����Ʈ �Ҹ���
{

}

NoticeBroadcastPacket::NoticeBroadcastPacket(NoticeBroadcastPacket&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}

void NoticeBroadcastPacket::SetString(const std::string& _string)
{
}

void NoticeBroadcastPacket::SetTimer(float _timer)
{
}

void NoticeBroadcastPacket::initPacketID()
{
}

void NoticeBroadcastPacket::userSerialize()
{
}

void NoticeBroadcastPacket::userDeserialize()
{
}

GameEnginePacketBase* NoticeBroadcastPacket::getUserObject()
{
    return nullptr;
}

void NoticeBroadcastPacket::execute(SOCKET _socketSender, GameEngineSocketInterface* _network, bool _bServer)
{

    // �ڱ� �÷��̾��� uicontroller �� ����
    // noticeUI ������ ��ε�ĳ��Ʈ ��ü �ϰ������� ��Ű�� ���
}

