#pragma once
#include "MonsterInfo.h"

// ������ �߻�������������
struct RefInfoByRegion
{
public: // �⺻��������
	Location Region_;																		// ����Ÿ��
	int TotalMonsterCnt_;																	// ������ �ִ���� ������

public: // ��ġ��������(������ġ������) - �ش� ������ �ش��ϴ� ���ؽ��� ��ξ˰��ִ´�.
	GameEngineFBXMesh* SpawnPoints_;														// ��ǥ��ϸ޽�(�׺�޽�Ȱ��: ������ġ������)

public: // ��ü������(�����Ϸ��¸��Ͱ�ü�� ������)
	int MonsterCount_[static_cast<int>(MonsterType::MAX)];									// ����(�߻�����) Ÿ�Ժ� ��ü��
};

// �з� : Manager
// �뵵 : 
// ���� : ���� �ʿ� ��ġ�� ��� ������ ������ �����ϴ� ������
class MonsterInfoManager
{
public:
	static MonsterInfoManager* GetInstance();

public: // Inline Get Function
	inline int GetCurMonsterListSize()
	{
		return static_cast<int>(AllMonsters_.size());
	}

	inline std::vector<MonsterInfo>& GetAllMonsterListRef()
	{
		return AllMonsters_;
	}

	inline std::vector<MonsterInfo> GetAllMonsterListValue()
	{
		return AllMonsters_;
	}

public: // Inline Set Function

public: // Public MonsterBasicInfo Add & Set Function
	void AddMonsterInfo(MonsterInfo _MonsterInfo);
	void SetMonsterInfos(std::vector<MonsterInfo> _MonsterInfos);

public: // 
	bool CreatMonsterInfomation();															// ����ʿ� ��ġ�Ϸ��� ������ �⺻������ ����

protected:

private: // 
	void LoadSpawnPointMeshByRegion();														// ������ �׺�޽� �ε�
	void CreateReferenceInfomation(Location _Location);											// ������ ���Ͱ�ü��, ... ������ ����
	void SaveCreationCountByRegion(RefInfoByRegion& _ResultInfo);							// ������ �ִ�������Ͱ��� ���� ���� - �����������...
	void CreateBasicMonsterInfos();															// �ռ� ������ ������ ���� �����Ϸ��� ���� �⺻���� ����

private: // Logging & Conversion Function
	std::string LoggingTypeToString(Location _Type);										// Type To String Conversion(Logging) - Korean(English)
	std::string ConversionTypeToString(Location _Type);										// Type To String Conversion - English

public:
	MonsterInfoManager();
	~MonsterInfoManager();

protected:
	MonsterInfoManager(const MonsterInfoManager& _other) = delete;
	MonsterInfoManager(MonsterInfoManager&& _other) noexcept = delete;

private:
	MonsterInfoManager& operator=(const MonsterInfoManager& _other) = delete;
	MonsterInfoManager& operator=(const MonsterInfoManager&& _other) = delete;

public:
protected:
private:
	std::vector<MonsterInfo> AllMonsters_;													// ���� �ʿ� ��ġ�� ��� ������ ����

private:
	int MaxCreationCount_;																	// �����Ϸ��� ���� �ִ��(1~MaxCreationCount_���� �������� ��������������)
	RefInfoByRegion RefInfoByRegion_[static_cast<int>(Location::MAX)];						// ������ �����Ϸ��� ���͵��� �⺻����(�����)

};

