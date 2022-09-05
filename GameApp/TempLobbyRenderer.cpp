#include "PreCompile.h"
#include "TempLobbyRenderer.h"
#include "GameEngine/GameEngineUIRenderer.h"

TempLobbyRenderer::TempLobbyRenderer() // default constructer ����Ʈ ������
	:portraitRenderer_(nullptr)
{

}

TempLobbyRenderer::~TempLobbyRenderer() // default destructer ����Ʈ �Ҹ���
{

}

TempLobbyRenderer::TempLobbyRenderer(const TempLobbyRenderer& _other)
{
}

void TempLobbyRenderer::SetPos(const float4& _pos)
{
	GetTransform()->SetLocalPosition(_pos);
}

void TempLobbyRenderer::SetScale(const float4& _pos)
{
	GetTransform()->SetLocalScaling(_pos);
}

void TempLobbyRenderer::SetImage(std::string _imageName)
{
	portraitRenderer_->SetImage(_imageName);
}

void TempLobbyRenderer::SetRender(bool _bool)
{
	if (true == _bool)
	{
		portraitRenderer_->On();
	}
	else
	{
		portraitRenderer_->Off();
	}
}

void TempLobbyRenderer::Start()
{
	portraitRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	portraitRenderer_->SetImage("tempLobbyAnon.png", "PointSmp");
}

void TempLobbyRenderer::Update(float _DeltaTime)
{
}

