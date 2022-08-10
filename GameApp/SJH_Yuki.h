#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngineBase/GameEnginePathFind.h>

// �з� : 
// �뵵 : 
// ���� : 
class GameEngineFBXMesh;
class GameEngineFBXRenderer;
class SJH_NaviCell;
class SJH_Yuki : public GameEngineActor
{
public:
	void Move(SJH_NaviCell* _TargetNaviCell, const float4& _MoveTargetPos);

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
	static SJH_Yuki* MainPlayer;

protected:

private:
	GameEngineFBXMesh* Mesh_;
	GameEngineFBXRenderer* AnimRenderer_;

private:
	std::vector<std::string> AnimationNameList_;

private:
	SJH_NaviCell* CurNaviCell_;				// ���� �÷��̾ ��ġ�ϰ� �ִ� �ﰢ��
	SJH_NaviCell* TargetNaviCell_;			// ���� �÷��̾ �̵��Ϸ��� ��ǥ������ ��ġ�� �ﰢ��
	bool MoveFlag_;							// �̵���� ���� �Ϸ�� On
	std::list<PathIndex> MovePath_;			// �̵� ���

private: // �׽�Ʈ��
	float4 MoveStartPos_;					// �̵�������ġ
	float4 MoveTargetPos_;					// �̵���ǥ��ġ
	float4 MoveDir_;						// �̵�����
	float MoveSpeed_;						// �̵��ӵ�

};

