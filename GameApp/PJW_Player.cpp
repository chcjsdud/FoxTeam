#include "PreCompile.h"
#include "PJW_Player.h"

PJW_Player::PJW_Player() // default constructer ����Ʈ ������
: target_(nullptr),
status_HP_(0.0f),
status_ATK_(0.0f),
status_MoveSpeed_(0.0f),
deltaTime_(0.0f),
aimDir_(0.0f, 0.0f, 1.0f),
curDir_(0.0f, 0.0f, 1.0f),
isMoving_(false),
curHP_(0.0f),
isAttacking_(false)
{

}

PJW_Player::~PJW_Player() // default destructer ����Ʈ �Ҹ���
{

}

PJW_Player::PJW_Player(PJW_Player&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}

