#pragma once
#include <GameEngine/GameEngineActor.h>
// �з� : 
// �뵵 : �κ񷹺��� �� UI���͸� ��Ƶδ� ��
// ���� : 




class LumiaUIController : public GameEngineActor
{
public:
	LumiaUIController(); // default constructer ����Ʈ ������
	~LumiaUIController(); // default destructer ����Ʈ �Ҹ���
	LumiaUIController(const LumiaUIController& _other) = delete; // default Copy constructer ����Ʈ ���������
	LumiaUIController(LumiaUIController&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	LumiaUIController& operator=(const LumiaUIController& _other) = delete; // default Copy operator ����Ʈ ���� ������
	LumiaUIController& operator=(const LumiaUIController&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void InitUI();


public:


protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:

};

