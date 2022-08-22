#include "PreCompile.h"
#include "SJH_Yuki.h"

#include <GameEngine/GameEngineFBXMesh.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "SJH_FloorMap.h"
#include "SJH_NaviCell.h"

SJH_Yuki* SJH_Yuki::MainPlayer = nullptr;

void SJH_Yuki::Initialize(SJH_NaviCell* _CurNaviCell, const float4& _InitPos)
{
	// 현재 플레이어가 위치한 삼각형(셀)을 지정
	CurNaviCell_ = _CurNaviCell;

	// 현재 플레이어가 위치한 삼각형(셀)의 무게중심으로 시작위치 셋팅
	GetTransform()->SetWorldPosition(_InitPos);
}

void SJH_Yuki::Move(SJH_NaviCell* _TargetNaviCell, const float4& _MoveTargetPos)
{
	// 이동중지
	MoveStart_ = false;

	MovePathTarget_ = _TargetNaviCell;

	// 기존 이동경로가 남아있다면 이동중 타겟위치가 변경되었으므로
	// 현재 플레이어의 위치좌표에 해당하는 삼각형을 알아내고
	if (false == MovePath_.empty())
	{
		// 기존 이동경로를 삭제하고
		MovePath_.clear();

		// 플레이어의 현재위치좌표를 이용하여 현재 속한 삼각형(셀)을 알아내어 셋팅
		SJH_NaviCell* CurCell = SJH_FloorMap::FloorMap->SearchCurrentPosToNaviCell(GetTransform()->GetWorldPosition());
		if (nullptr != CurCell)
		{
			CurNaviCell_ = CurCell;
		}
	}

	// 이동목표지점의 삼각형 갱신
	TargetNaviCell_ = _TargetNaviCell;

	// 현재 플레이어의 NaviCell ~ _TargetNaviCell까지의 이동경로 생성
	if (true == SJH_FloorMap::FloorMap->MoveFacePath(GetTransform()->GetWorldPosition(), _MoveTargetPos, CurNaviCell_, TargetNaviCell_, MovePath_))
	{
		// 이동시작위치 지정
		MoveStartPos_ = MovePath_.front();
		MovePath_.pop_front();

		// 경로상의 이동목표위치 지정
		MoveEndPos_ = MovePath_.front();
		MovePath_.pop_front();

		// 이동경로 생성완료 후 Flag On
		MoveStart_ = true;

		// 이동시작
		AnimRenderer_->ChangeFBXAnimation(AnimationNameList_[37]);
	}
}

void SJH_Yuki::Start()
{
	GetLevel()->GetMainCameraActor()->GetTransform()->AttachTransform(GetTransform());
	GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalPosition({ 0.0f, 5.0f, 0.0f });
	GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalRotationDegree({ 80.0f, 0.0f, 0.0f });

#pragma region UserFiles Load
	//// User FBX Files
	//GameEngineDirectory MeshPath;
	//MeshPath.MoveParent("FoxTeam");
	//MeshPath.MoveChild("Resources");
	//MeshPath.MoveChild("FBX");
	//MeshPath.MoveChild("UserMesh");

	//GameEngineDirectory AnimationPath;
	//AnimationPath.MoveParent("FoxTeam");
	//AnimationPath.MoveChild("Resources");
	//AnimationPath.MoveChild("FBX");
	//AnimationPath.MoveChild("UserAni");

	//Mesh_ = GameEngineFBXMeshManager::GetInst().LoadUser(MeshPath.PathToPlusFileName("Yuki_01_LOD1.UserMesh"));
	//GameEngineFBXAnimation* Animation = GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_01_LOD1.UserAnimation"));

	//AnimRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	//AnimRenderer_->SetFBXMesh(Mesh_->GetName(), "TextureDeferredLightAni");
	//AnimRenderer_->GetTransform()->SetLocalRotationDegree({ -90.0f, 0.0f, 0.0f });

	//for (size_t i = 0; i < AnimRenderer_->GetRenderSetCount(); i++)
	//{
	//	AnimRenderer_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Yuki_01_LOD1.png");
	//}

	//// 애니메이션 이름 목록생성
	//AnimationNameList_.clear();
	//for (int i = 0; i < static_cast<int>(Animation->AnimationCount()); i++)
	//{
	//	auto Data = Animation->GetAnimationData(i);
	//
	//	std::string AnimationName = Data.AniName;
	//	AnimationNameList_.push_back(AnimationName);
	//}

	//AnimRenderer_->CreateFBXAnimation(AnimationNameList_[0], Animation->GetName(), 0);
	//AnimRenderer_->CreateFBXAnimation(AnimationNameList_[14], Animation->GetName(), 14);
	//AnimRenderer_->ChangeFBXAnimation(AnimationNameList_[0]);

#pragma endregion

#pragma region FBXFiles Load
	// FBX Files 경로 지정
	GameEngineDirectory Directory;
	Directory.MoveParent("FoxTeam");
	Directory.MoveChild("Resources");
	Directory.MoveChild("FBX");
	Directory.MoveChild("SJH");

	// 메쉬로드
	std::string MeshName = "Yuki_01_LOD1.fbx";

	Mesh_ = GameEngineFBXMeshManager::GetInst().Load(Directory.PathToPlusFileName(MeshName));
	Mesh_->CreateRenderingBuffer();

	// 렌더러 생성
	AnimRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	AnimRenderer_->SetFBXMesh(MeshName, "TextureDeferredLightAni");
	AnimRenderer_->GetTransform()->SetLocalRotationDegree({ -90.0f, 0.0f, 0.0f });

	// Yuki_01_LOD1.png
	for (UINT i = 0; i < AnimRenderer_->GetRenderSetCount(); i++)
	{
		AnimRenderer_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Yuki_01_LOD1.png");
	}

	// 애니메이션 로드
	GameEngineFBXAnimation* Animation = GameEngineFBXAnimationManager::GetInst().Load(Directory.PathToPlusFileName(MeshName));

	// 애니메이션 생성
	AnimationNameList_.clear();
	for (int i = 0; i < static_cast<int>(Animation->AnimationCount()); i++)
	{
		auto Data = Animation->GetAnimationData(i);

		std::string AnimationName = Data.AniName;
		AnimationNameList_.push_back(AnimationName);
		//AnimRenderer_->CreateFBXAnimation(AnimationName, MeshName, i);
	}

#pragma region 기존애니메이션
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

	AnimRenderer_->CreateFBXAnimation(AnimationNameList_[0], MeshName, 0, true);
	AnimRenderer_->CreateFBXAnimation(AnimationNameList_[37], MeshName, 37, true);
	AnimRenderer_->ChangeFBXAnimation(AnimationNameList_[0]);

#pragma endregion

	if (false == GameEngineInput::GetInst().IsKey("SJH_TEST1"))
	{
		GameEngineInput::GetInst().CreateKey("SJH_TEST1", VK_DOWN);
	}

	if (false == GameEngineInput::GetInst().IsKey("SJH_TEST2"))
	{
		GameEngineInput::GetInst().CreateKey("SJH_TEST2", VK_UP);
	}
}

void SJH_Yuki::Update(float _DeltaTime)
{
	if (true == GameEngineInput::GetInst().Down("SJH_TEST1"))
	{
		AnimRenderer_->ChangeFBXAnimation(AnimationNameList_[0], true);
	}

	if (true == GameEngineInput::GetInst().Down("SJH_TEST2"))
	{
		AnimRenderer_->ChangeFBXAnimation(AnimationNameList_[37], true);
	}

	// 이동가능 Flag On & 이동경로가 존재할때 플레이어는 이동
	if (true == MoveStart_)
	{
		// 이동 처리
		LerpMoveTime_ += GameEngineTime::GetInst().GetDeltaTime(MoveSpeed_);
		float4 LerpPos = float4::Lerp(MoveStartPos_, MoveEndPos_, LerpMoveTime_);

		GetTransform()->SetWorldPosition(LerpPos);

		// 이동완료시
		if (1.0f < LerpMoveTime_)
		{
			LerpMoveTime_ = 0.0f;

			// 이동경로가 남아있다면
			if (false == MovePath_.empty())
			{
				// 시작위치 재설정
				MoveStartPos_ = GetTransform()->GetWorldPosition();

				// 목표위치 재설정
				MoveEndPos_ = MovePath_.front();
				MovePath_.pop_front();

				// 여전히 이동중
				AnimRenderer_->ChangeFBXAnimation(AnimationNameList_[37]);
			}
			// 남아있는 경로가 없다면
			else
			{
				// 이동종료 및 이동정보 초기화
				MoveStartPos_ = float4(0.0f, 0.0f, 0.0f, 0.0f);
				MoveEndPos_ = float4(0.0f, 0.0f, 0.0f, 0.0f);

				// 모든경로를 사용하여 기존의 목표위치에 도달했다면
				CurNaviCell_ = TargetNaviCell_;
				TargetNaviCell_ = nullptr;

				// 이동종료
				MoveStart_ = false;
				MovePath_.clear();

				// 완전히 이동종료
				AnimRenderer_->ChangeFBXAnimation(AnimationNameList_[0]);
			}
		}
	}
}

SJH_Yuki::SJH_Yuki()
	: Mesh_(nullptr)
	, AnimRenderer_(nullptr)
	, CurNaviCell_(nullptr)
	, TargetNaviCell_(nullptr)
	, MoveStart_(false)
	, MovePathTarget_(nullptr)
	, MoveStartPos_(float4(0.0f, 0.0f, 0.0f, 0.0f))
	, MoveEndPos_(float4(0.0f, 0.0f, 0.0f, 0.0f))
	, MoveSpeed_(1.0f)
	, LerpMoveTime_(0.0f)
{
}

SJH_Yuki::~SJH_Yuki()
{
}
