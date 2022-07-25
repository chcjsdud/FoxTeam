#pragma once
#include "Enums.h"

// �з� : ��Ʈ�ѷ�
// �뵵 : �������� ���� ����
// ���� : 
//        1) �ش� ��Ʈ�ѷ� �ʱ�ȭ������ ���� ������ ��� ������Ʈ(����)���� ������ ���ÿ� ��ġ�ϸ� �Ϸ�� ���ӽ���
//        2) UI�� �ش� ��Ʈ�ѷ��� �����Ͽ� ����ĳ���� ����Ī�� ����
//           -. �����÷��̾� ����Ī ���� ��� ĳ���ʹ� �����·� ���� ��, �����Ͻ��������·� �����ϰ� ����Ī�Ѵ�.
//           -. ����Ī �Ϸ� ������ �����÷��̾��� ������ UI�� �����Ͽ� UI������ �����Ѵ�.
//        3) ���������� ������ ĳ���Ͱ� ������.
//           -. ������ ĳ���ʹ� Characters Ŭ������ ��ӹ޾�ó���ϸ�, ���� ��ų������ ĳ���ͳ����� �����Ͽ� ����Ѵ�.
//           -. ������ ĳ���ʹ� AI�̸� �þ߹����ۿ� �����Ҷ� �������Ĺ������� �����Ѵ�.
//           -. ������ ĳ���ʹ� �⺻ Ŀ�ǵ� ������ ������ Ư����Ȳ�� ���� ���� ������ ��ȯ�Ѵ�.(AI Ÿ���϶�)
//              - ���߰�(�þ߹����� �� ����) : �����ݰ��ɹ������� �������� ����
//              - ������ ����(���� ���� ����) : �� ��ų���� ��Ÿ������
//                -> ��� ��ų ��밡�� : �������� ��ų����(���� ��ų�� ��Ÿ������)
//                -> ��� ��ų ���Ұ� : �Ϲݰ��� ����
//              - ������ ������ ����ü���� �Ǹ� ���� Ÿ������ ���� ü�°� ���Ͽ� �� ���ٸ� ���������� �����Ѵ�.
//        4) ����, ������Ʈ�� ������ �������� �������� ��ġ
//        5) ��������� ��Ʈ�ѷ��� ���� ������ �ʿ� �����ϴ� ��� ������Ʈ�� �����ϸ� UI���� ��ȣ�ۿ��� ó���Ѵ�.
class Characters;
class Monsters;
class Objects;
class GameEngineLevel;
class Controller
{
private:
	static Controller* Inst;

public:
	static Controller& GetInst()
	{
		return *Inst;
	}

	static void Destory()
	{
		if (nullptr != Inst)
		{
			delete Inst;
			Inst = nullptr;
		}
	}

private:
	GameEngineLevel* CurrentLevel_;

private: // ���ӳ��� �����ϴ� ��� ĳ����(AI, ����)�� ���͸� ����
	std::list<Objects*> Objects_;
	std::list<Monsters*> Monsters_;
	std::list<Characters*> Characters_;

private: // ��� ���� ��ġ�� ���� ��ũ�� ����
	float4 MapSize_;

private:


public:
	Controller();
	~Controller();

protected:
	Controller(const Controller& _other) = delete;
	Controller(Controller&& _other) noexcept = delete;

private:
	Controller& operator=(const Controller& _other) = delete;
	Controller& operator=(const Controller&& _other) = delete;

#pragma region ���ӽ��� �غ�
public: // ���ӽ��� �� �ʱ�ȭ �� ����
	void Initialize();

private: // �ʷε�
	void LoadMap();

private: // ������Ʈ & ���� & ĳ���� ����
	void CreateObject();
	void CreateMonster();
	void CreateCharacter();

private: // ��� ���� ��ġ
	void AllActorArrange();

private: // �����Ϸ�� ��� ������Ʈ �� ����, AI ��ġ
	void ArrangeObjects();
	void ArrangeCharacters();
	void ArrangeMonsters();

#pragma endregion

#pragma region ����Ī����
public: // UI�κ��� ����
	void MainPlayerSwitching(Characters* _SelCharacter);	// ���� ���õ� ĳ���ͷ� ������� �ο� �� ī�޶� �̵�

#pragma endregion

#pragma region ...



#pragma endregion
};

