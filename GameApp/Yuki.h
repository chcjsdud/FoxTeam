#pragma once
#include "Characters.h"

// 분류 : 캐릭터
// 용도 : 유키
// 설명 : 고유 스킬 및 패턴을 관리
class Yuki : public Characters
{
private:

public:
	Yuki();
	~Yuki();

protected:		// delete constructer
	Yuki(const Yuki& _other) = delete;
	Yuki(Yuki&& _other) noexcept = delete;

private:		//delete operator
	Yuki& operator=(const Yuki& _other) = delete;
	Yuki& operator=(const Yuki&& _other) = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void BaseInfoSetting();
	void SkillInfoSetting();

public: // 유키 고유정보 및 스킬정보 셋팅
	void Initialize() override;

};

