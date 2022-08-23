#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 몬스터
// 용도 : 
// 설명 : 
class SJH_Wolf : public GameEngineActor
{
public:
protected:
private:

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	SJH_Wolf();
	~SJH_Wolf();

protected:		// delete constructer
	SJH_Wolf(const SJH_Wolf& _other) = delete;
	SJH_Wolf(SJH_Wolf&& _other) noexcept = delete;

private:		//delete operator
	SJH_Wolf& operator=(const SJH_Wolf& _other) = delete;
	SJH_Wolf& operator=(const SJH_Wolf&& _other) = delete;

public:
protected:
private:
};

