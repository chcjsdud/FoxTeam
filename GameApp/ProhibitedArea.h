#pragma once
#include "Enums.h"
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineCollision.h>

// �з� : 
// �뵵 : 
// ���� : 
class ProhibitedArea : public GameEngineActor
{
public:
	ProhibitedArea(); // default constructer ����Ʈ ������
	~ProhibitedArea(); // default destructer ����Ʈ �Ҹ���
	ProhibitedArea(const ProhibitedArea& _other) = delete; // default Copy constructer ����Ʈ ���������
	ProhibitedArea(ProhibitedArea&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	ProhibitedArea& operator=(const ProhibitedArea& _other) = delete; // default Copy operator ����Ʈ ���� ������
	ProhibitedArea& operator=(const ProhibitedArea&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void Init(Location _location, const std::string& _fbxName, float4 _pos);
	void AddTriggerCollision(float4 _pos, float4 _scale);
	void SetProhibited(bool _tf);

	void Start() override;
	void Update(float _DeltaTime) override;

private:
	Location location_;
	bool isProhibited_;

	GameEngineFBXRenderer* renderer_;
	std::vector<GameEngineCollision*> triggerList_;
};

