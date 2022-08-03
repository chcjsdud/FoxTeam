#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineBase/GameEngineFile.h>


class LGH_Aya : public GameEngineActor
{

public:
	LGH_Aya();
	~LGH_Aya();
	LGH_Aya(const LGH_Aya& _other) = delete;
	LGH_Aya(LGH_Aya&& _other) noexcept;
	LGH_Aya& operator=(const LGH_Aya& _other) = delete;
	LGH_Aya& operator=(const LGH_Aya&& _other) = delete;


protected: 
	virtual void Start();

	virtual void Update(float _DeltaTime);

// FSM
private:
	//GameEngineFSM hyunwooState_;

// States 함수


// FBX 렌더러
private:
	GameEngineFBXRenderer* FBXRenderer_;
	GameEngineDirectory FBXFolder;


// 충돌체



};

