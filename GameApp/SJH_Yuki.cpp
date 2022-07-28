#include "PreCompile.h"
#include "SJH_Yuki.h"

#include <GameEngine/GameEngineFBXMesh.h>
#include <GameEngine/GameEngineFBXRenderer.h>

void SJH_Yuki::Start()
{
	GetLevel()->GetMainCameraActor()->GetTransform()->AttachTransform(GetTransform());
	GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalPosition({ 0.0f, 10.0f, -20.0f });
	GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalRotationDegree({ 10.0f, 0.0f, 0.0f });

	// FBX Files ��� ����
	GameEngineDirectory Directory;
	Directory.MoveParent("FoxTeam");
	Directory.MoveChild("Resources");
	Directory.MoveChild("FBX");
	Directory.MoveChild("SJH");

	// �޽��ε�
	Mesh_ = GameEngineFBXMeshManager::GetInst().Load(Directory.PathToPlusFileName("Yuki_01_LOD1.fbx"));
	Mesh_->CreateRenderingBuffer();

	// �ִϸ��̼� �ε�
	GameEngineFBXAnimation* Animation = GameEngineFBXAnimationManager::GetInst().Load(Directory.PathToPlusFileName("Yuki_01_LOD1.fbx"));

	// ������ ����
	Renderer_ = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	Renderer_->SetFBXMesh("Yuki_01_LOD1.fbx", "TextureAni");
	Renderer_->GetTransform()->SetLocalScaling(float4(10.f, 10.f, 10.f));
	Renderer_->GetTransform()->SetLocalRotationDegree({ -90.0f, 0.0f, 0.0f });
	
	Renderer_->CreateFBXAnimation("Yuki_Common_Wait", "Yuki_01_LOD1.fbx", 0);
	Renderer_->CreateFBXAnimation("Yuki_Common_rest_start", "Yuki_01_LOD1.fbx", 1);
	Renderer_->CreateFBXAnimation("Yuki_Common_rest_loop", "Yuki_01_LOD1.fbx", 2);
	Renderer_->CreateFBXAnimation("Yuki_Common_operate", "Yuki_01_LOD1.fbx", 3);
	Renderer_->CreateFBXAnimation("Yuki_Common_install_trap", "Yuki_01_LOD1.fbx", 4);
	Renderer_->CreateFBXAnimation("Yuki_Common_fishing", "Yuki_01_LOD1.fbx", 5);
	Renderer_->CreateFBXAnimation("Yuki_Common_craftfood", "Yuki_01_LOD1.fbx", 6);
	Renderer_->CreateFBXAnimation("Yuki_Common_craftmetal", "Yuki_01_LOD1.fbx", 7);
	Renderer_->CreateFBXAnimation("Yuki_Common_arrive", "Yuki_01_LOD1.fbx", 8);
	Renderer_->CreateFBXAnimation("Yuki_Empty_Weaponskill_start", "Yuki_01_LOD1.fbx", 9);
	Renderer_->CreateFBXAnimation("Yuki_Empty_Weaponskill_end", "Yuki_01_LOD1.fbx", 10);
	Renderer_->CreateFBXAnimation("Yuki_Empty_run", "Yuki_01_LOD1.fbx", 11);
	Renderer_->CreateFBXAnimation("Yuki_Common_boxopen", "Yuki_01_LOD1.fbx", 12);
	Renderer_->CreateFBXAnimation("Yuki_Common_collect", "Yuki_01_LOD1.fbx", 13);
	Renderer_->CreateFBXAnimation("Yuki_Common_dance", "Yuki_01_LOD1.fbx", 14);
	Renderer_->CreateFBXAnimation("Yuki_Common_death", "Yuki_01_LOD1.fbx", 15);
	Renderer_->CreateFBXAnimation("Yuki_Common_rest_end", "Yuki_01_LOD1.fbx", 16);
	Renderer_->CreateFBXAnimation("Yuki_Common_collect_Stone", "Yuki_01_LOD1.fbx", 17);
	Renderer_->CreateFBXAnimation("Yuki_Common_collect_Water", "Yuki_01_LOD1.fbx", 18);
	Renderer_->CreateFBXAnimation("Yuki_Common_collect_Wood", "Yuki_01_LOD1.fbx", 19);
	Renderer_->CreateFBXAnimation("Yuki_BareHands_skill04", "Yuki_01_LOD1.fbx", 20);
	Renderer_->CreateFBXAnimation("Yuki_2Hand_skill02_upper_wait", "Yuki_01_LOD1.fbx", 21);
	Renderer_->CreateFBXAnimation("Yuki_BareHands_skill04_end", "Yuki_01_LOD1.fbx", 22);
	Renderer_->CreateFBXAnimation("Yuki_BareHands_skill03_move", "Yuki_01_LOD1.fbx", 23);
	Renderer_->CreateFBXAnimation("Yuki_BareHands_atk01", "Yuki_01_LOD1.fbx", 24);
	Renderer_->CreateFBXAnimation("Yuki_BareHands_atk02", "Yuki_01_LOD1.fbx", 25);
	Renderer_->CreateFBXAnimation("Yuki_BareHands_skill03_attack", "Yuki_01_LOD1.fbx", 26);
	Renderer_->CreateFBXAnimation("Yuki_BareHands_skill01", "Yuki_01_LOD1.fbx", 27);
	Renderer_->CreateFBXAnimation("Yuki_Common_resurrect", "Yuki_01_LOD1.fbx", 28);
	Renderer_->CreateFBXAnimation("Yuki_Common_down_start", "Yuki_01_LOD1.fbx", 29);
	Renderer_->CreateFBXAnimation("Yuki_Common_down_run", "Yuki_01_LOD1.fbx", 30);
	Renderer_->CreateFBXAnimation("Yuki_Common_down_wait", "Yuki_01_LOD1.fbx", 31);
	Renderer_->CreateFBXAnimation("Yuki_Common_down_dead", "Yuki_01_LOD1.fbx", 32);
	Renderer_->CreateFBXAnimation("Yuki_Empty_DualSword_Weaponskill", "Yuki_01_LOD1.fbx", 33);
	Renderer_->CreateFBXAnimation("Yuki_Empty_Dual_weaponskill02", "Yuki_01_LOD1.fbx", 34);
	Renderer_->CreateFBXAnimation("Yuki_2Hand_wait", "Yuki_01_LOD1.fbx", 35);
	Renderer_->CreateFBXAnimation("Yuki_Dual_wait", "Yuki_01_LOD1.fbx", 36);
	Renderer_->CreateFBXAnimation("Yuki_Common_run", "Yuki_01_LOD1.fbx", 37);
	Renderer_->CreateFBXAnimation("Yuki_Common_skill04_end", "Yuki_01_LOD1.fbx", 38);
	Renderer_->CreateFBXAnimation("Yuki_Common_skill04", "Yuki_01_LOD1.fbx", 39);
	Renderer_->CreateFBXAnimation("Yuki_2Hand_skill03_move", "Yuki_01_LOD1.fbx", 40);
	Renderer_->CreateFBXAnimation("Yuki_2Hand_skill01", "Yuki_01_LOD1.fbx", 41);
	Renderer_->CreateFBXAnimation("Yuki_2Hand_Weaponskill_end", "Yuki_01_LOD1.fbx", 42);
	Renderer_->CreateFBXAnimation("Yuki_2Hand_Weaponskill_start", "Yuki_01_LOD1.fbx", 43);
	Renderer_->CreateFBXAnimation("Yuki_2Hand_skill03_attack", "Yuki_01_LOD1.fbx", 44);
	Renderer_->CreateFBXAnimation("Yuki_2Hand_atk01", "Yuki_01_LOD1.fbx", 45);
	Renderer_->CreateFBXAnimation("Yuki_2Hand_atk02", "Yuki_01_LOD1.fbx", 46);
	Renderer_->CreateFBXAnimation("Yuki_2Hand_run", "Yuki_01_LOD1.fbx", 47);
	Renderer_->CreateFBXAnimation("Yuki_Dual_skill02_upper_wait", "Yuki_01_LOD1.fbx", 48);
	Renderer_->CreateFBXAnimation("Yuki_Dual_skill03_move", "Yuki_01_LOD1.fbx", 49);
	Renderer_->CreateFBXAnimation("Yuki_Dual_skill01", "Yuki_01_LOD1.fbx", 50);
	Renderer_->CreateFBXAnimation("Yuki_Dual_skill03_attack", "Yuki_01_LOD1.fbx", 51);
	Renderer_->CreateFBXAnimation("Yuki_Dual_atk01", "Yuki_01_LOD1.fbx", 52);
	Renderer_->CreateFBXAnimation("Yuki_Dual_atk02", "Yuki_01_LOD1.fbx", 53);
	Renderer_->CreateFBXAnimation("Yuki_Empty_Dual_run", "Yuki_01_LOD1.fbx", 54);
	Renderer_->CreateFBXAnimation("Yuki_Dual_weaponskill", "Yuki_01_LOD1.fbx", 55);
	Renderer_->CreateFBXAnimation("Yuki_Dual_weaponskill02", "Yuki_01_LOD1.fbx", 56);
	Renderer_->CreateFBXAnimation("Yuki_Dual_run", "Yuki_01_LOD1.fbx", 57);

	Renderer_->ChangeFBXAnimation("Yuki_Empty_run");

	// ���� ����




}

void SJH_Yuki::Update(float _DeltaTime)
{
}

SJH_Yuki::SJH_Yuki()
	: Mesh_(nullptr)
	, Renderer_(nullptr)
{
}

SJH_Yuki::~SJH_Yuki()
{
}
