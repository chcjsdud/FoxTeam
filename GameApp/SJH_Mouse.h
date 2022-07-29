#pragma once
#include <GameEngine/GameEngineActor.h>

// �з� : ���콺
// �뵵 : UI ��ȣ �� �÷��̾� �̵�
// ���� : 
class GameEngineUIRenderer;
class GameEngineCollision;
class SJH_Ray;
class SJH_Mouse : public GameEngineActor
{
public:
protected:
private:

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	SJH_Mouse();
	~SJH_Mouse();

protected:
	SJH_Mouse(const SJH_Mouse& _other) = delete;
	SJH_Mouse(SJH_Mouse&& _other) noexcept = delete;

private:
	SJH_Mouse& operator=(const SJH_Mouse& _other) = delete;
	SJH_Mouse& operator=(const SJH_Mouse&& _other) = delete;

public:

protected:

private:
	GameEngineUIRenderer* Renderer_;
	GameEngineCollision* Collider_;

private:
	SJH_Ray* Ray_;
};

