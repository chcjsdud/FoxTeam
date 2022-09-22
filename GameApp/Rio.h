#pragma once

#include <GameEngine/GameEngineActor.h>
#include "Character.h"
#include <GameEngine/GameEngineFSM.h>

class GameEngineFBXRenderer;
class LumiaMap;
class Rio : public Character
{
public:
	Rio();
	~Rio();
	Rio(const Rio& _other) = delete;
	Rio(Rio&& _other) = delete;
	Rio& operator=(const Rio& _other) = delete;
	Rio& operator=(const Rio&& _other) = delete;

public:


protected:
	void Start() override;
	void Update(float _deltaTime) override;

	// Character을(를) 통해 상속됨
	virtual void initRendererAndAnimation() override;
	virtual void changeAnimationWait() override;
	virtual void changeAnimationRun() override;
	virtual void changeAnimationBasicAttack() override;

private:



};