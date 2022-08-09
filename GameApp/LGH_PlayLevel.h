#pragma once
#include <GameEngine/GameEngineLevel.h>


#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineBase/GameEngineFile.h>

// 분류 : 
// 용도 : 
// 설명 : 
class LGH_Aya;
class LGH_PlayLevel : public GameEngineLevel
{
public:
	LGH_PlayLevel();

public:
	GameEngineDirectory FBXFolder;

	LGH_Aya* player_;

	class UI_Skill* SkillUI_;
	class UI_Equip* EquipUI_;
	class UI_Inventory* InventoryUI_;
	class UI_Status* StatusUI_;

protected:
	LGH_PlayLevel(const LGH_PlayLevel& _other) = delete;
	LGH_PlayLevel(LGH_PlayLevel&& _other) noexcept = delete;

private:
	void LevelStart() override;
	void LevelUpdate(float _DeltaTime) override;
	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

	~LGH_PlayLevel();

	LGH_PlayLevel& operator=(const LGH_PlayLevel& _other) = delete;
	LGH_PlayLevel& operator=(const LGH_PlayLevel&& _other) = delete;
};

