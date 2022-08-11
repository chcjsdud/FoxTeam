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

	// 시작위치를 셋팅
	GetTransform()->SetWorldPosition(_InitPos);
}

void SJH_Yuki::Move(SJH_NaviCell* _TargetNaviCell, const float4& _MoveTargetPos)
{
	// 이동중지
	MoveStartFlag_ = false;

	// 기존 이동경로가 남아있다면
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

	// 이동타겟 삼각형이 변경되었다면
	if (CurNaviCell_ != _TargetNaviCell)
	{
		// 이동목표지점의 삼각형 갱신
		TargetNaviCell_ = _TargetNaviCell;

		// 현재 플레이어의 NaviCell ~ _TargetNaviCell까지의 이동경로 생성
		if (true == SJH_FloorMap::FloorMap->MoveFacePath(CurNaviCell_, TargetNaviCell_, MovePath_))
		{
			// 이동경로 생성완료 후 Flag On
			MoveStartFlag_ = true;
		}
	}
}

void SJH_Yuki::Start()
{
	GetLevel()->GetMainCameraActor()->GetTransform()->AttachTransform(GetTransform());
	GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalPosition({ 0.0f, 50.0f, 0.0f });
	GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalRotationDegree({ 80.0f, 0.0f, 0.0f });

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
	AnimRenderer_->GetTransform()->SetLocalScaling(float4(10.f, 10.f, 10.f));
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
	for (size_t i = 0; i < Animation->AnimationCount(); i++)
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

	AnimRenderer_->CreateFBXAnimation(AnimationNameList_[0], MeshName, 0);
	AnimRenderer_->ChangeFBXAnimation(AnimationNameList_[0]);
}

void SJH_Yuki::Update(float _DeltaTime)
{
	// 이동가능 Flag On & 이동경로가 존재할때 플레이어는 이동한다.
	if (true == MoveStartFlag_)
	{
		// 이동 처리





		// 이동경로가 존재한다면 이동을 위한 정보 셋팅
		if (false == MovePath_.empty())
		{
			// 현재 위치 ~ MovePath_의 가장 첫번째 삼각형의 무게중심까지 이동 경로 설정




		}
		// 이동경로가 모두 소진되었다면
		else
		{
			// 현재 플레이어가 위치한 삼각형(셀)을 셋팅하고
			CurNaviCell_ = TargetNaviCell_;

			// 목표 삼각형을 초기화
			TargetNaviCell_ = nullptr;

			// 이동종료
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
