#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// �з� : 
// �뵵 : 
// ���� : 
class GameEngineImageRenderer;
class GameEngineEffectRenderer;
class HyunwooREffect : public GameEngineActor
{
private:	// member Var

public:
	HyunwooREffect(); // default constructer ����Ʈ ������
	~HyunwooREffect(); // default destructer ����Ʈ �Ҹ���
	HyunwooREffect(const HyunwooREffect& _other) = delete; // default Copy constructer ����Ʈ ���������
	HyunwooREffect(HyunwooREffect&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	HyunwooREffect& operator=(const HyunwooREffect& _other) = delete; // default Copy operator ����Ʈ ���� ������
	HyunwooREffect& operator=(const HyunwooREffect&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void PlayAwake();
	void PlayExplode();

	void SetSleepMandatory();

protected:
	void Start() override;
	void Update(float _deltaTime) override;


protected:
	void startAwaken();
	void updateAwaken(float _deltaTime);

	void startExplode();
	void updateExplode(float _deltaTime);

	void startSleeping();
	void updateSleeping(float _deltaTime);

private:

	GameEngineFSM renderState_;
	GameEngineEffectRenderer* hitBoxRenderer_;
	GameEngineEffectRenderer* impactRenderer_;
	GameEngineEffectRenderer* dustRenderer_;

private:
	bool isActivated_;
	bool isTriggered_;
	float timer_;
};

