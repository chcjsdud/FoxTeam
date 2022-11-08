#include "PreCompile.h"
#include "NoticeBroadcastPacket.h"

NoticeBroadcastPacket::NoticeBroadcastPacket() // default constructer 디폴트 생성자
{

}

NoticeBroadcastPacket::~NoticeBroadcastPacket() // default destructer 디폴트 소멸자
{

}

NoticeBroadcastPacket::NoticeBroadcastPacket(NoticeBroadcastPacket&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
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

    // 자기 플레이어의 uicontroller 에 접근
    // noticeUI 접근해 브로드캐스트 전체 일괄적으로 시키는 기능
}

