#pragma once
#include "Enums.h"

// �з� : ������
// �뵵 : 
// ���� : �� ���� Ÿ�Ժ� ������ ������ ����
struct LevelData
{
	std::string TypeName_;						// �� ������ ������(Ex. Yuki, Rio, ...)
	float AttackPower_;							// ���ݷ�
	float HP_;									// ü��
	float HPRegeneration_;						// ü�� ���
	float SP_;									// ���¹̳�
	float SPRegeneration_;						// ���¹̳� ���
	float Defence_;								// ����
	float AttackSpeed_;							// ���ݼӵ�
	float CriticalChance_;						// ġ��Ÿ
	float MovementSpeed_;						// �̵��ӵ�
	float VisionRange_;							// �þ�
};

// �з� : �ý���
// �뵵 : 
// ���� : ���ӽð��� ���� ������ ó����� ����
class LevelUpSystem
{
public: // Static Value & Function
	static LevelUpSystem* GetInstance();

public: // ���������� ���� �ε�
	void LoadLevelInfomationFile();

public: // �ϰ�ó��
	void AllUnitLevelUP();

protected:
private:

public:
	LevelUpSystem();
	~LevelUpSystem();

protected:
	LevelUpSystem(const LevelUpSystem& _other) = delete;
	LevelUpSystem(LevelUpSystem&& _other) = delete;

private:
	LevelUpSystem& operator=(const LevelUpSystem& _other) = delete;
	LevelUpSystem& operator=(const LevelUpSystem&& _other) = delete;

public:
protected:
private:
	std::vector<GameEngineFile> AllFiles_;													// ������ ����� ��� ���ϸ��

private:
	std::map<JobType, LevelData> CharacterLevelData_;										// ĳ���͵��� ������ �������
	std::map<MonsterType, LevelData> MonsterLevelData_;										// ���͵��� ������ �������
};

