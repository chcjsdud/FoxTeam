#pragma once
#include "Enums.h"

struct MonsterInfo
{
public: // ��������
	int Index_;										// ���� ���� �ε���(Ž����)
	Location AreaType_;								// ���� ���� ���� Ÿ��(Ž����)
	MonsterType MonsterType_;						// ���� Ÿ��

public: // �ʱ�ȭ����
	float4 SpawnPosition_;							// ���� ��ġ
};