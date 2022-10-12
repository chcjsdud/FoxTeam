#pragma once
#include <vector>
#include "Enums.h"

// �з� : ����
// �뵵 : 
// ���� : ������ ��������
struct MonsterStateInfo
{
public: // �⺻����
	int Level_;								// ����(�ʰ� ������ ����(�߻�����)�ϼ��� ������ ���� ����) - ������ ���ÿ� ����(�������� ������ ������ ��� �⺻ 1��������)
	float HPMax_;							// �ִ�ü��
	float HP_;								// ����ü��
	float HPRegenPercent_;					// ü��ȸ����(% ��ġ)
	float SPMax_;							// �ִ븶��
	float SP_;								// ���縶��
	float SPRegenPercent_;					// ����ȸ����(% ��ġ)
	float Defence_;							// ����

public: // �󼼽���
	float4 NestPosition_;					// ������ġ(= ������ġ)
	float HomingInstinctValueMax_;			// �ִ� �ͼҺ��� ��ġ
	float HomingInstinctValue_;				// �ͼҺ��� ��ġ(0.0f���� �۰ų��������� ���ʹ� ������ġ�� �̵��ϸ� ü���� ȸ�� -> Ÿ���� DetectRange_�� ��� �ð��� ���� ����)
	float AttackRange_;						// ���� �þ߰Ÿ�
	float DetectRange_;						// ���� �þ߰Ÿ�
	float AttackSpeed_;						// ���� �ӵ�
	float MoveSpeed_;						// �̵� �ӵ�

public: // ��ų����
	float SkillCoolDown_;					// ��ų����ð�

public: // ������
	float RegenTimeMax_;					// ����Ÿ��(����)
	float RegenTime_;						// ����Ÿ��(����) -> 0.0f�����Ͻ� RegenTime_���� �ʱ�ȭ

public: // �⺻�������������


public: // Ȯ���������������

public:
	MonsterStateInfo()
		: Level_(1)
		, HPMax_(0.0f)
		, HP_(0.0f)
		, HPRegenPercent_(0.0f)
		, SPMax_(0.0f)
		, SP_(0.0f)
		, SPRegenPercent_(0.0f)
		, Defence_(0.0f)
		, NestPosition_(float4::ZERO)
		, HomingInstinctValueMax_(0.0f)
		, HomingInstinctValue_(0.0f)
		, AttackRange_(0.0f)
		, DetectRange_(0.0f)
		, AttackSpeed_(0.0f)
		, MoveSpeed_(0.0f)
		, SkillCoolDown_(0.0f)
		, RegenTimeMax_(0.0f)
		, RegenTime_(0.0f)
	{
	}

	~MonsterStateInfo()
	{
	}
};
