#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

class GameEngineEffectRenderer;
class BasicAttackEffect : public GameEngineActor
{
public:
	BasicAttackEffect(); // default constructer ����Ʈ ������
	~BasicAttackEffect(); // default destructer ����Ʈ �Ҹ���
	BasicAttackEffect(const BasicAttackEffect& _other) = delete; // default Copy constructer ����Ʈ ���������
	BasicAttackEffect(BasicAttackEffect&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	BasicAttackEffect& operator=(const BasicAttackEffect& _other) = delete; // default Copy operator ����Ʈ ���� ������
	BasicAttackEffect& operator=(const BasicAttackEffect&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void PlayAwake(const std::string& _animationName);
	GameEngineEffectRenderer* GetAttackRenderer() { return atkRenderer_; }

protected:
	void Start() override;
	void Update(float _deltaTime) override;

protected:
	void startSleep();
	void updateSleep(float _deltaTime);

	void startAwake();
	void updateAwake(float _deltaTime);

private:
	GameEngineFSM renderState_;
	GameEngineEffectRenderer* atkRenderer_;
	std::string animationName_;

};
