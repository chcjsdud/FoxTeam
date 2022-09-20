#pragma once
#include <GameEngine/GameEngineActor.h>
// 분류 : 
// 용도 : 로비레벨에 들어갈 UI액터를 모아두는 곳
// 설명 : 




class LumiaUIController : public GameEngineActor
{
public:
	LumiaUIController(); // default constructer 디폴트 생성자
	~LumiaUIController(); // default destructer 디폴트 소멸자
	LumiaUIController(const LumiaUIController& _other) = delete; // default Copy constructer 디폴트 복사생성자
	LumiaUIController(LumiaUIController&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	LumiaUIController& operator=(const LumiaUIController& _other) = delete; // default Copy operator 디폴트 대입 연산자
	LumiaUIController& operator=(const LumiaUIController&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void InitUI();


public:


protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:

};

