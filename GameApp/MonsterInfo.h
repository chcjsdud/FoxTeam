#pragma once
#include "Enums.h"

struct MonsterInfo
{
public: // ��������
	int Index_;										// ���� ���� �ε���(Ž����)
	Location RegionType_;							// ���� ���� ���� Ÿ��(Ž����)
	MonsterType MonsterType_;						// ���� Ÿ��
	int IsGroup_;									// �׷��������(1: �׷����λ���, 0: �ܵ�����)
	int GroupCount_;								// �׷�����϶� ������ġ�� �����ؾ��ϴ� �߻�������

public: // �ʱ�ȭ����
	float4 SpawnPosition_;							// ���� ��ġ
};
