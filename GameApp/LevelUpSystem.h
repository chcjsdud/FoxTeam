#pragma once
#include "Enums.h"

// �������ý����� Ÿ��
enum class LevelUPSystemType
{
	POWER,
	HP,
	HPRegen,
	SP,
	SPRegen,
	Defence,
	AtkSpeed,
	Critical,
	MoveSpeed,
	EyeSight,
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
	std::map<JobType, std::map<LevelUPSystemType, float>> CharacterLevelUpInfo_;			// ĳ���������� ����������
	std::map<MonsterType, std::map<LevelUPSystemType, float>> MonsterLevelUpInfo_;			// ����Ÿ�Ժ� ����������
};

