#pragma once
#include <GameEngine/GameEngineActor.h>

// �з� : 
// �뵵 : 
// ���� : 
class Objects : public GameEngineActor
{
private:	// member Var

public:
	Objects();
	~Objects();

protected:		// delete constructer
	Objects(const Objects& _other) = delete;
	Objects(Objects&& _other) noexcept = delete;

private:		//delete operator
	Objects& operator=(const Objects& _other) = delete;
	Objects& operator=(const Objects&& _other) = delete;

public:
	void Start() override;
	void Update(float _DeltaTime) override;
};

