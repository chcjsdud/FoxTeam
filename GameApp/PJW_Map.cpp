#include "PreCompile.h"
#include "PJW_Map.h"
#include "PJW_Enum.h"

#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineImageRenderer.h>

PJW_Map::PJW_Map() // default constructer ����Ʈ ������
{

}

PJW_Map::~PJW_Map() // default destructer ����Ʈ �Ҹ���
{

}

PJW_Map::PJW_Map(PJW_Map&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}


void PJW_Map::Start()
{
	GetTransform()->SetWorldPosition({ 0.0f, -50.0f, 0.0f });


	renderer_->SetRenderingPipeLine("Color");
	renderer_->GetTransform()->SetLocalScaling({ 1000.0f, 10.0f, 1000.0f });
	renderer_->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	renderer_->ShaderHelper.SettingConstantBufferSet("ResultColor", float4(0.5f, 0.5f, 0.5f));

	{
		////Ÿ�� ��Ʈ �ڽ�
		mapCollision_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
		mapCollision_->GetTransform()->SetLocalPosition(renderer_->GetTransform()->GetLocalPosition());
		mapCollision_->GetTransform()->SetLocalScaling(renderer_->GetTransform()->GetLocalScaling());
		mapCollision_->SetCollisionType(CollisionType::AABBBox3D);
		mapCollision_->SetCollisionGroup(InGameCollisionType::Map);
	}
}