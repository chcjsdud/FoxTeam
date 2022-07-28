#pragma once
#include <GameEngine/GameEngineActor.h>

// �з� : 
// �뵵 : 
// ���� : 
class GameEngineFBXMesh;
class GameEngineFBXRenderer;
class SJH_Yuki : public GameEngineActor
{
public:

protected:

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	SJH_Yuki();
	~SJH_Yuki();

protected:
	SJH_Yuki(const SJH_Yuki& _other) = delete;
	SJH_Yuki(SJH_Yuki&& _other) noexcept = delete;

private:
	SJH_Yuki& operator=(const SJH_Yuki& _other) = delete;
	SJH_Yuki& operator=(const SJH_Yuki&& _other) = delete;

public:

protected:

private:
	GameEngineFBXMesh* Mesh_;
	GameEngineFBXRenderer* Renderer_;

};
