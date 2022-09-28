#pragma once
#include "Enums.h"

struct LoadingSyncInfo
{
public:
	LoadingSyncInfo()
		: MapLoadEnd_(0)
		, MonsterLoadEnd_(0)
		, CharacterLoadEnd_(0)
	{
	}

	~LoadingSyncInfo()
	{
	}

public: // �ε��Ϸ� Flag ����
	int MapLoadEnd_;					// ���� ���ӿ� ������ �� �ε�Ϸ�(1: �ε��Ϸ�)
	int MonsterLoadEnd_;				// ���� �ʿ� ��ġ�Ǵ� ��� ���� �ε�Ϸ�(1: �ε��Ϸ�)
	int CharacterLoadEnd_;				// ���� ���ӿ� ������ �÷��̾� �ε�Ϸ�(1: �ε��Ϸ�)
};
