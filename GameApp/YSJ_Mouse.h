#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngineBase/GameEnginePathFind.h>

// 분류 : 마우스
// 용도 : UI 상호 및 플레이어 이동
// 설명 : 
class GameEngineUIRenderer;
class GameEngineCollision;
class YSJ_Ray;
class GameEngineFBXRenderer;
class YSJ_Mouse : public GameEngineActor
{
public:
	static YSJ_Mouse* MainMouse;

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	YSJ_Mouse();
	~YSJ_Mouse();

	void SetPickingRenderer(GameEngineFBXRenderer* _Renderer)
	{
		Picking_ = _Renderer;
	}

	float4 GetClickPoint()
	{
		return ClickPoint_;
	}
	GameEngineCollision* GetPickCollision(const float4& _MousePos, int _Order);
protected:
	YSJ_Mouse(const YSJ_Mouse& _other) = delete;
	YSJ_Mouse(YSJ_Mouse&& _other) noexcept = delete;

private:
	YSJ_Mouse& operator=(const YSJ_Mouse& _other) = delete;
	YSJ_Mouse& operator=(const YSJ_Mouse&& _other) = delete;

	void MeshPicking();
	void PlayerMove(float _DeltaTime);

public:

protected:

private:
	GameEngineUIRenderer* Renderer_;
	GameEngineCollision* Collider_;
	GameEngineFBXRenderer* Picking_;
private:
	YSJ_Ray* Ray_;
	float4 ClickPoint_;

	GameEnginePathFind AStar_;
	std::list<PathIndex> AStarlist_;
};

