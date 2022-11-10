#include "PreCompile.h"
#include "RioDSkillArrow.h"

RioDSkillArrow::RioDSkillArrow() // default constructer ����Ʈ ������
	: arrowRenderer_(nullptr)
{

}

RioDSkillArrow::~RioDSkillArrow() // default destructer ����Ʈ �Ҹ���
{

}

RioDSkillArrow::RioDSkillArrow(RioDSkillArrow&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}

void RioDSkillArrow::Start()
{
	arrowRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>(static_cast<int>(ObjectRenderOrder::CHARACTER));
	arrowRenderer_->SetFBXMesh("Rio_000_Arrow.fbx", "TextureDeferredLight", true);

	arrowRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 1000.0f, 0.0f });
	//arrowRenderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	arrowRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });


	renderState_.CreateState(MakeState(RioDSkillArrow, Sleep));
	renderState_.CreateState(MakeState(RioDSkillArrow, Awake));
	renderState_.CreateState(MakeState(RioDSkillArrow, Fade));
	renderState_ << "Sleep";
}

void RioDSkillArrow::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void RioDSkillArrow::startSleep()
{
	timer_ = 0.0f;
	arrowRenderer_->Off();
}

void RioDSkillArrow::updateSleep(float _deltaTime)
{

}

void RioDSkillArrow::startAwake()
{

	// ��ġ ����
}

void RioDSkillArrow::updateAwake(float _deltaTime)
{

}

void RioDSkillArrow::startFade()
{

}

void RioDSkillArrow::updateFade(float _deltaTime)
{
}

