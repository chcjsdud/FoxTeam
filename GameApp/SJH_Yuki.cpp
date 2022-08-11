#include "PreCompile.h"
#include "SJH_Yuki.h"

#include <GameEngine/GameEngineFBXMesh.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "SJH_FloorMap.h"
#include "SJH_NaviCell.h"

SJH_Yuki* SJH_Yuki::MainPlayer = nullptr;

void SJH_Yuki::Initialize(SJH_NaviCell* _CurNaviCell, const float4& _InitPos)
{
	// ���� �÷��̾ ��ġ�� �ﰢ��(��)�� ����
	CurNaviCell_ = _CurNaviCell;

	// ������ġ�� ����
	GetTransform()->SetWorldPosition(_InitPos);
}

void SJH_Yuki::Move(SJH_NaviCell* _TargetNaviCell, const float4& _MoveTargetPos)
{
	// �̵�����
	MoveStartFlag_ = false;

	// ���� �̵���ΰ� �����ִٸ�
	if (false == MovePath_.empty())
	{
		// ���� �̵���θ� �����ϰ�
		MovePath_.clear();

		// �÷��̾��� ������ġ��ǥ�� �̿��Ͽ� ���� ���� �ﰢ��(��)�� �˾Ƴ��� ����
		SJH_NaviCell* CurCell = SJH_FloorMap::FloorMap->SearchCurrentPosToNaviCell(GetTransform()->GetWorldPosition());
		if (nullptr != CurCell)
		{
			CurNaviCell_ = CurCell;
		}
	}

	// �̵�Ÿ�� �ﰢ���� ����Ǿ��ٸ�
	if (CurNaviCell_ != _TargetNaviCell)
	{
		// �̵���ǥ������ �ﰢ�� ����
		TargetNaviCell_ = _TargetNaviCell;

		// ���� �÷��̾��� NaviCell ~ _TargetNaviCell������ �̵���� ����
		if (true == SJH_FloorMap::FloorMap->MoveFacePath(CurNaviCell_, TargetNaviCell_, MovePath_))
		{
			// �̵���� �����Ϸ� �� Flag On
			MoveStartFlag_ = true;
		}
	}
}

void SJH_Yuki::Start()
{
	GetLevel()->GetMainCameraActor()->GetTransform()->AttachTransform(GetTransform());
	GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalPosition({ 0.0f, 50.0f, 0.0f });
	GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalRotationDegree({ 80.0f, 0.0f, 0.0f });

	// FBX Files ��� ����
	GameEngineDirectory Directory;
	Directory.MoveParent("FoxTeam");
	Directory.MoveChild("Resources");
	Directory.MoveChild("FBX");
	Directory.MoveChild("SJH");

	// �޽��ε�
	std::string MeshName = "Yuki_01_LOD1.fbx";

	Mesh_ = GameEngineFBXMeshManager::GetInst().Load(Directory.PathToPlusFileName(MeshName));
	Mesh_->CreateRenderingBuffer();

	// ������ ����
	AnimRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	AnimRenderer_->SetFBXMesh(MeshName, "TextureDeferredLightAni");
	AnimRenderer_->GetTransform()->SetLocalScaling(float4(10.f, 10.f, 10.f));
	AnimRenderer_->GetTransform()->SetLocalRotationDegree({ -90.0f, 0.0f, 0.0f });

	// Yuki_01_LOD1.png
	for (UINT i = 0; i < AnimRenderer_->GetRenderSetCount(); i++)
	{
		AnimRenderer_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Yuki_01_LOD1.png");
	}

	// �ִϸ��̼� �ε�
	GameEngineFBXAnimation* Animation = GameEngineFBXAnimationManager::GetInst().Load(Directory.PathToPlusFileName(MeshName));

	// �ִϸ��̼� ����
	AnimationNameList_.clear();
	for (size_t i = 0; i < Animation->AnimationCount(); i++)
	{
		auto Data = Animation->GetAnimationData(i);

		std::string AnimationName = Data.AniName;
		AnimationNameList_.push_back(AnimationName);
		//AnimRenderer_->CreateFBXAnimation(AnimationName, MeshName, i);
	}

#pragma region �����ִϸ��̼�
	//Renderer_->CreateFBXAnimation("Yuki_Common_wait", "Yuki_01_LOD1.fbx", 0);
	//Renderer_->CreateFBXAnimation("Yuki_Common_rest_start", "Yuki_01_LOD1.fbx", 1);
	//Renderer_->CreateFBXAnimation("Yuki_Common_rest_loop", "Yuki_01_LOD1.fbx", 2);
	//Renderer_->CreateFBXAnimation("Yuki_Common_operate", "Yuki_01_LOD1.fbx", 3);
	//Renderer_->CreateFBXAnimation("Yuki_Common_install_trap", "Yuki_01_LOD1.fbx", 4);
	//Renderer_->CreateFBXAnimation("Yuki_Common_fishing", "Yuki_01_LOD1.fbx", 5);
	//Renderer_->CreateFBXAnimation("Yuki_Common_craftfood", "Yuki_01_LOD1.fbx", 6);
	//Renderer_->CreateFBXAnimation("Yuki_Common_craftmetal", "Yuki_01_LOD1.fbx", 7);
	//Renderer_->CreateFBXAnimation("Yuki_Common_arrive", "Yuki_01_LOD1.fbx", 8);
	//Renderer_->CreateFBXAnimation("Yuki_Empty_Weaponskill_start", "Yuki_01_LOD1.fbx", 9);
	//Renderer_->CreateFBXAnimation("Yuki_Empty_Weaponskill_end", "Yuki_01_LOD1.fbx", 10);
	//Renderer_->CreateFBXAnimation("Yuki_Empty_run", "Yuki_01_LOD1.fbx", 11);
	//Renderer_->CreateFBXAnimation("Yuki_Common_boxopen", "Yuki_01_LOD1.fbx", 12);
	//Renderer_->CreateFBXAnimation("Yuki_Common_collect", "Yuki_01_LOD1.fbx", 13);
	//Renderer_->CreateFBXAnimation("Yuki_Common_dance", "Yuki_01_LOD1.fbx", 14);
	//Renderer_->CreateFBXAnimation("Yuki_Common_death", "Yuki_01_LOD1.fbx", 15);
	//Renderer_->CreateFBXAnimation("Yuki_Common_rest_end", "Yuki_01_LOD1.fbx", 16);
	//Renderer_->CreateFBXAnimation("Yuki_Common_collect_Stone", "Yuki_01_LOD1.fbx", 17);
	//Renderer_->CreateFBXAnimation("Yuki_Common_collect_Water", "Yuki_01_LOD1.fbx", 18);
	//Renderer_->CreateFBXAnimation("Yuki_Common_collect_Wood", "Yuki_01_LOD1.fbx", 19);
	//Renderer_->CreateFBXAnimation("Yuki_BareHands_skill04", "Yuki_01_LOD1.fbx", 20);
	//Renderer_->CreateFBXAnimation("Yuki_2Hand_skill02_upper_wait", "Yuki_01_LOD1.fbx", 21);
	//Renderer_->CreateFBXAnimation("Yuki_BareHands_skill04_end", "Yuki_01_LOD1.fbx", 22);
	//Renderer_->CreateFBXAnimation("Yuki_BareHands_skill03_move", "Yuki_01_LOD1.fbx", 23);
	//Renderer_->CreateFBXAnimation("Yuki_BareHands_atk01", "Yuki_01_LOD1.fbx", 24);
	//Renderer_->CreateFBXAnimation("Yuki_BareHands_atk02", "Yuki_01_LOD1.fbx", 25);
	//Renderer_->CreateFBXAnimation("Yuki_BareHands_skill03_attack", "Yuki_01_LOD1.fbx", 26);
	//Renderer_->CreateFBXAnimation("Yuki_BareHands_skill01", "Yuki_01_LOD1.fbx", 27);
	//Renderer_->CreateFBXAnimation("Yuki_Common_resurrect", "Yuki_01_LOD1.fbx", 28);
	//Renderer_->CreateFBXAnimation("Yuki_Common_down_start", "Yuki_01_LOD1.fbx", 29);
	//Renderer_->CreateFBXAnimation("Yuki_Common_down_run", "Yuki_01_LOD1.fbx", 30);
	//Renderer_->CreateFBXAnimation("Yuki_Common_down_wait", "Yuki_01_LOD1.fbx", 31);
	//Renderer_->CreateFBXAnimation("Yuki_Common_down_dead", "Yuki_01_LOD1.fbx", 32);
	//Renderer_->CreateFBXAnimation("Yuki_Empty_DualSword_Weaponskill", "Yuki_01_LOD1.fbx", 33);
	//Renderer_->CreateFBXAnimation("Yuki_Empty_Dual_weaponskill02", "Yuki_01_LOD1.fbx", 34);
	//Renderer_->CreateFBXAnimation("Yuki_2Hand_wait", "Yuki_01_LOD1.fbx", 35);
	//Renderer_->CreateFBXAnimation("Yuki_Dual_wait", "Yuki_01_LOD1.fbx", 36);
	//Renderer_->CreateFBXAnimation("Yuki_Common_run", "Yuki_01_LOD1.fbx", 37);
	//Renderer_->CreateFBXAnimation("Yuki_Common_skill04_end", "Yuki_01_LOD1.fbx", 38);
	//Renderer_->CreateFBXAnimation("Yuki_Common_skill04", "Yuki_01_LOD1.fbx", 39);
	//Renderer_->CreateFBXAnimation("Yuki_2Hand_skill03_move", "Yuki_01_LOD1.fbx", 40);
	//Renderer_->CreateFBXAnimation("Yuki_2Hand_skill01", "Yuki_01_LOD1.fbx", 41);
	//Renderer_->CreateFBXAnimation("Yuki_2Hand_Weaponskill_end", "Yuki_01_LOD1.fbx", 42);
	//Renderer_->CreateFBXAnimation("Yuki_2Hand_Weaponskill_start", "Yuki_01_LOD1.fbx", 43);
	//Renderer_->CreateFBXAnimation("Yuki_2Hand_skill03_attack", "Yuki_01_LOD1.fbx", 44);
	//Renderer_->CreateFBXAnimation("Yuki_2Hand_atk01", "Yuki_01_LOD1.fbx", 45);
	//Renderer_->CreateFBXAnimation("Yuki_2Hand_atk02", "Yuki_01_LOD1.fbx", 46);
	//Renderer_->CreateFBXAnimation("Yuki_2Hand_run", "Yuki_01_LOD1.fbx", 47);
	//Renderer_->CreateFBXAnimation("Yuki_Dual_skill02_upper_wait", "Yuki_01_LOD1.fbx", 48);
	//Renderer_->CreateFBXAnimation("Yuki_Dual_skill03_move", "Yuki_01_LOD1.fbx", 49);
	//Renderer_->CreateFBXAnimation("Yuki_Dual_skill01", "Yuki_01_LOD1.fbx", 50);
	//Renderer_->CreateFBXAnimation("Yuki_Dual_skill03_attack", "Yuki_01_LOD1.fbx", 51);
	//Renderer_->CreateFBXAnimation("Yuki_Dual_atk01", "Yuki_01_LOD1.fbx", 52);
	//Renderer_->CreateFBXAnimation("Yuki_Dual_atk02", "Yuki_01_LOD1.fbx", 53);
	//Renderer_->CreateFBXAnimation("Yuki_Empty_Dual_run", "Yuki_01_LOD1.fbx", 54);
	//Renderer_->CreateFBXAnimation("Yuki_Dual_weaponskill", "Yuki_01_LOD1.fbx", 55);
	//Renderer_->CreateFBXAnimation("Yuki_Dual_weaponskill02", "Yuki_01_LOD1.fbx", 56);
	//Renderer_->CreateFBXAnimation("Yuki_Dual_run", "Yuki_01_LOD1.fbx", 57);

#pragma endregion

	AnimRenderer_->CreateFBXAnimation(AnimationNameList_[0], MeshName, 0);
	AnimRenderer_->ChangeFBXAnimation(AnimationNameList_[0]);
}

void SJH_Yuki::Update(float _DeltaTime)
{
	// �̵����� Flag On & �̵���ΰ� �����Ҷ� �÷��̾�� �̵��Ѵ�.
	if (true == MoveStartFlag_)
	{
		// �̵� ó��





		// �̵���ΰ� �����Ѵٸ� �̵��� ���� ���� ����
		if (false == MovePath_.empty())
		{
			// ���� ��ġ ~ MovePath_�� ���� ù��° �ﰢ���� �����߽ɱ��� �̵� ��� ����




		}
		// �̵���ΰ� ��� �����Ǿ��ٸ�
		else
		{
			// ���� �÷��̾ ��ġ�� �ﰢ��(��)�� �����ϰ�
			CurNaviCell_ = TargetNaviCell_;

			// ��ǥ �ﰢ���� �ʱ�ȭ
			TargetNaviCell_ = nullptr;

			// �̵�����
			MoveStartFlag_ = false;
		}
	}
}

SJH_Yuki::SJH_Yuki()
	: Mesh_(nullptr)
	, AnimRenderer_(nullptr)
	, CurNaviCell_(nullptr)
	, TargetNaviCell_(nullptr)
	, MoveStartFlag_(false)
	, MoveStartPos_(float4::ZERO)
	, MoveEndPos_(float4::ZERO)
	, MoveDir_(float4::ZERO)
{
}

SJH_Yuki::~SJH_Yuki()
{
}
