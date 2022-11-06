#pragma once
#include <GameEngine/GameEngineActor.h>
// �з� : 
// �뵵 : 
// ���� : 
class GameEngineCollision;
class AreaChangeTrigger : public GameEngineActor
{
public:
	AreaChangeTrigger(); // default constructer ����Ʈ ������
	~AreaChangeTrigger(); // default destructer ����Ʈ �Ҹ���
	AreaChangeTrigger(const AreaChangeTrigger& _other) = delete; // default Copy constructer ����Ʈ ���������
	AreaChangeTrigger(AreaChangeTrigger&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	AreaChangeTrigger& operator=(const AreaChangeTrigger& _other) = delete; // default Copy operator ����Ʈ ���� ������
	AreaChangeTrigger& operator=(const AreaChangeTrigger&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void SetLocations(Location _loca0, const std::string& _loca0Name, Location _loca1, const std::string& _loca1Name);
	void SetCollision(float4 _pos, float4 _scale);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	Location entryPoint0_;
	Location entryPoint1_;

	std::string entryPointName0_;
	std::string entryPointName1_;

	GameEngineCollision* entryCollision_;

	bool bIsIn_;
};

