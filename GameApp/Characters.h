#pragma once
#include <GameEngine/GameEngineActor.h>

#include "Enums.h"

struct SkillInfo
{
	// skilltype(�нú�, ����, ...)
	// �Ҹ��ڿ�(sp)
	// ��Ÿ��
	// ��Ÿ�
	// ȿ��
};

// �з� : 
// �뵵 : ĳ���� �ֻ��� Ŭ����
// ���� : ��� ĳ������ Ŀ�ǵ� ���� �� �⺻������ ����
class Characters : public GameEngineActor
{
private: // ���� �����÷��̾� ����
	static Characters* MainPlayer;

public:
	static Characters* GetMainPlayer();
	static void SetMainPlayer(Characters* _Player);

private: // ��������
	CharacterType CharacterType_;		// (0: AI, 1: MAIN) : default(AI)

private: // �⺻����
	JobType JobType_;					// ����Ÿ��
	float OffensePower_;				// ���ݷ�
	int Health_;						// ü��
	float HealthRecovery_;				// ü�����
	int Stamina_;						// ���¹̳�
	float StaminaRecovery_;				// ���¹̳� ���
	float Defense_;						// ����
	float AttackSpeed_;					// ���ݼӵ�
	float CriticalHit_;					// ġ��Ÿ
	float MoveSpeed_;					// �̵��ӵ�
	float Eyesight_;					// �þ�

private: // �������� �������
	float Lv_OffensePower_;				// ���ݷ�
	int Lv_Health_;						// ü��
	float Lv_HealthRecovery_;			// ü�����
	int Lv_Stamina_;					// ���¹̳�
	float Lv_StaminaRecovery_;			// ���¹̳� ���
	float Lv_Defense_;					// ����
	float Lv_AttackSpeed_;				// ���ݼӵ�
	float Lv_CriticalHit_;				// ġ��Ÿ
	float Lv_MoveSpeed_;				// �̵��ӵ�
	float Lv_Eyesight_;					// �þ�

private: // ��ų����
	SkillInfo SkillInfo_[6];			// ��ų(�нú�, Q, W, E, R, D)
										// �нú�, Q~R ���� ��ų
										// D ��ų�� ������ ��ų

private: // Ŀ�ǵ� ����
// 1. ����ĳ���� �þ� �ۿ����� ����(�̵�-�Ĺ� ����)
// 2. ����ĳ���� �þ� ������������ ����(�̵�-���� or ����-ȸ��)


public:
	Characters();
	~Characters();

protected:		// delete constructer
	Characters(const Characters& _other) = delete;
	Characters(Characters&& _other) noexcept = delete;

private:		//delete operator
	Characters& operator=(const Characters& _other) = delete;
	Characters& operator=(const Characters&& _other) = delete;

public:
	inline void SetCharacterType(CharacterType _Type)
	{
		// �߸��� Ÿ�� ���Ž� ����
		if (_Type == CharacterType::NONE || _Type == CharacterType::MAX)
		{
			return;
		}

		CharacterType_ = _Type;
	}

protected: // �� ĳ���� ���� ���� �� ��ų����
	virtual void Initialize() = 0;

protected: // �� ĳ���� Ŀ�ǵ� �ο�


};

