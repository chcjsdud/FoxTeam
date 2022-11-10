#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// �з� : 
// �뵵 : 
// ���� : 


class GameEngineFBXRenderer;
class RioDSkillArrow : public GameEngineActor
{
public:
	RioDSkillArrow(); // default constructer ����Ʈ ������
	~RioDSkillArrow(); // default destructer ����Ʈ �Ҹ���
	RioDSkillArrow(const RioDSkillArrow& _other) = delete; // default Copy constructer ����Ʈ ���������
	RioDSkillArrow(RioDSkillArrow&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	RioDSkillArrow& operator=(const RioDSkillArrow& _other) = delete; // default Copy operator ����Ʈ ���� ������
	RioDSkillArrow& operator=(const RioDSkillArrow&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime);

protected:
	void startSleep();
	void updateSleep(float _deltaTime);

	void startAwake();
	void updateAwake(float _deltaTime);

	void startFade();
	void updateFade(float _deltaTime);

private:
	GameEngineFSM renderState_;
	GameEngineFBXRenderer* arrowRenderer_;

	float timer_;


};

