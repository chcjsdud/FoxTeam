#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineShaderResHelper.h"
#include "Enums.h"

// �̳༮�鸸�� �׷��� ����ڽ��ϴ�.


// ���� :
class GameEngineCore;
class GameEngineLevel;
class GameEngineCollision : public GameEngineTransformComponent
{
	friend GameEngineCore;
	friend GameEngineLevel;

	static std::function<bool(GameEngineTransform*, GameEngineTransform*)> 
		CollisionCheckFunction[static_cast<int>(CollisionType::MAX)][static_cast<int>(CollisionType::MAX)];

	static void Init();

public:
	static bool CirCleToCirCle(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool Sphere3DToSphere3D(GameEngineTransform* _Left, GameEngineTransform* _Right);

	static bool RectToRect(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool AABBToAABB(GameEngineTransform* _Left, GameEngineTransform* _Right);

	static bool OBBToOBB(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool CirCleToAABB(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool AABBToCirCle(GameEngineTransform* _Left, GameEngineTransform* _Right);

public:
	// constrcuter destructer
	GameEngineCollision();
	~GameEngineCollision();

	// delete Function
	GameEngineCollision(const GameEngineCollision& _Other) = delete;
	GameEngineCollision(GameEngineCollision&& _Other) noexcept = delete;
	GameEngineCollision& operator=(const GameEngineCollision& _Other) = delete;
	GameEngineCollision& operator=(GameEngineCollision&& _Other) noexcept = delete;

	template<typename UserType>
	void SetCollisionGroup(UserType _Type)
	{
		SetCollisionGroup(static_cast<int>(_Type));
	}

	void SetCollisionGroup(int _Type);

	void Collision(
		CollisionType _ThisType, 
		CollisionType _OtherType, 
		int _OtherGroup, 
		std::function<void(GameEngineCollision*)> _CallBack
	);


	void SphereToSphereCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack);

	/// ���� ///
	void SetCollisionType(CollisionType _ColType)
	{
		ColType_ = _ColType;
	}

	void SetCollisionInfo(int _Type, CollisionType _ColType)
	{
		SetCollisionGroup(_Type);
		SetCollisionType(_ColType);
	}

	//�浹 ���θ� Bool ������ ����
	bool Collision(int _OtherGroup);
	//�浹�� ���� ����� ptr ����
	GameEngineCollision* CollisionPtr(int _OtherGroup);
	//�浹�� ������ �ϴ� ����Ʈ�� ����
	std::list<GameEngineCollision*> CollisionPtrGroup(int _OtherGroup);

protected:
	CollisionType ColType_;
	///  ���� ///

private:
	void Start() override;
	void Update(float _DeltaTime) override;
};

