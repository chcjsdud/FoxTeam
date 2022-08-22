#pragma once
#include <GameEngine/GameEngineActor.h>

// �з� : 
// �뵵 : 
// ���� : 
class GameEngineFBXMesh;
class GameEngineFBXRenderer;
class SJH_NaviCell;
class SJH_Yuki : public GameEngineActor
{
public:
	void Initialize(SJH_NaviCell* _CurNaviCell, const float4& _InitPos);

public:
	void Move(SJH_NaviCell* _TargetNaviCell, const float4& _MoveTargetPos);

protected:

private:

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
	static SJH_Yuki* MainPlayer;

protected:

private:
	GameEngineFBXMesh* Mesh_;
	GameEngineFBXRenderer* AnimRenderer_;

private:
	std::vector<std::string> AnimationNameList_;

private:
	SJH_NaviCell* CurNaviCell_;				// ���� �÷��̾ ��ġ�ϰ� �ִ� �ﰢ��
	SJH_NaviCell* TargetNaviCell_;			// ���� �÷��̾ �̵��Ϸ��� ������ǥ������ ��ġ�� �ﰢ��
	bool MoveStart_;						// �̵���� ���� �Ϸ�� On

private:
	std::list<float4> MovePath_;			// �̵����
	SJH_NaviCell* MovePathTarget_;			// �̵��Ϸ� üũ��

private:
	float4 MoveStartPos_;					// �̵������ ���� ��ġ
	float4 MoveEndPos_;						// �̵������ ��ǥ ��ġ
	float MoveSpeed_;						// �̵��ӵ�
};

