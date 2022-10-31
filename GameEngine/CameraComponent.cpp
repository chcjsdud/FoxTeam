#include "PreCompile.h"
#include "CameraComponent.h"
#include "GameEngineTransform.h"
#include "GameEngineRendererBase.h"
#include "GameEngineRenderingPipeLineManager.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShader.h"
#include "GameEnginePixelShader.h"
#include "GameEngineVertexShader.h"
#include "GameEngineRenderTarget.h"

#include "GameEngineWindow.h"

CameraComponent::CameraComponent() :
	ProjectionMode_(ProjectionMode::Perspective),
	FovAngleY_(90.0f),
	CamSize_(GameEngineWindow::GetInst().GetSize()),
	NearZ_(0.1f),
	FarZ_(10000.0f),
	DebugRenderCount_(0),
	CameraBufferTarget_(nullptr)
{
}

CameraComponent::~CameraComponent()
{
	if (nullptr != CameraBufferTarget_)
	{
		delete CameraBufferTarget_;
		CameraBufferTarget_ = nullptr;
	}

	if (nullptr != CameraForwardTarget_)
	{
		delete CameraForwardTarget_;
		CameraForwardTarget_ = nullptr;
	}

	if (nullptr != CameraDeferredGBufferTarget)
	{
		delete CameraDeferredGBufferTarget;
		CameraDeferredGBufferTarget = nullptr;
	}

	if (nullptr != CameraDeferredLightTarget)
	{
		delete CameraDeferredLightTarget;
		CameraDeferredLightTarget = nullptr;
	}

	if (nullptr != CameraDeferredTarget_)
	{
		delete CameraDeferredTarget_;
		CameraDeferredTarget_ = nullptr;
	}

	if (nullptr != CameraOutLineTarget_)
	{
		delete CameraOutLineTarget_;
		CameraOutLineTarget_ = nullptr;
	}
}

void CameraComponent::ClearCameraTarget()
{
	CameraBufferTarget_->Clear();
}

void CameraComponent::CameraTransformUpdate()
{
	GetTransform()->GetTransformData().View_.ViewToLH(GetTransform()->GetWorldPosition(), GetTransform()->GetWorldForwardVector(), GetTransform()->GetWorldUpVector());

	switch (ProjectionMode_)
	{
	case ProjectionMode::Perspective:
	{
		GetTransform()->GetTransformData().Projection_.PerspectiveFovLH(FovAngleY_, CamSize_.x, CamSize_.y, NearZ_, FarZ_);
		break;
	}
	case ProjectionMode::Orthographic:
	{
		GetTransform()->GetTransformData().Projection_.OrthographicLH(CamSize_.x, CamSize_.y, NearZ_, FarZ_);
		break;
	}
	}
}

bool ZSort(GameEngineRendererBase* _Left, GameEngineRendererBase* _Right)
{
	return _Left->GetTransform()->GetWorldPosition().z > _Right->GetTransform()->GetWorldPosition().z;
}

void CameraComponent::Render(float _DeltaTime)
{
	CameraTransformUpdate();

	float4x4 View = GetTransform()->GetTransformData().View_;
	float4x4 Projection = GetTransform()->GetTransformData().Projection_;

	LightData_.LightCount = static_cast<int>(Lights_.size());

	int LightIndex = 0;
	for (GameEngineLightComponent* Light : Lights_)
	{
		LightData_.Lights[LightIndex] = Light->GetLightData();

		LightData_.Lights[LightIndex].ViewLightDir *= View;
		LightData_.Lights[LightIndex].ViewNegLightDir = -(LightData_.Lights[LightIndex].ViewLightDir);
		LightData_.Lights[LightIndex].ViewLightPosition *= View;
		++LightIndex;
	}

	RenderDeffered(_DeltaTime);
	RenderForward(_DeltaTime);

	CameraBufferTarget_->Clear();
	CameraBufferTarget_->Merge(CameraDeferredTarget_);
	CameraBufferTarget_->Merge(CameraForwardTarget_);


	GameEngineStructuredBufferSetting::ResetLastSetting();
}

void CameraComponent::DebugRender()
{
	if (true == IsDebugCheck())
	{
		return;
	}

	CameraBufferTarget_->Setting();

	float4x4 View = GetTransform()->GetTransformData().View_;
	float4x4 Projection = GetTransform()->GetTransformData().Projection_;

	for (size_t i = 0; i < DebugRenderCount_; i++)
	{
		DebugVector_[i].Data_.Projection_ = Projection;
		DebugVector_[i].Data_.View_ = View;
		DebugVector_[i].Data_.WVPCalculation();
		DebugVector_[i].ShaderHelper_.SettingConstantBufferLink("LightsData", LightData_);
		DebugVector_[i].ShaderHelper_.Setting();
		DebugVector_[i].PipeLine_->Rendering();
		DebugVector_[i].ShaderHelper_.ReSet();
		DebugVector_[i].PipeLine_->Reset();
	}

	DebugRenderCount_ = 0;
}

void CameraComponent::ReleaseRenderer()
{
	std::map<int, std::list<GameEngineRendererBase*>>::iterator RenderMapBeginIter = RendererList_.begin();
	std::map<int, std::list<GameEngineRendererBase*>>::iterator RenderMapEndIter = RendererList_.end();
	for (; RenderMapBeginIter != RenderMapEndIter; ++RenderMapBeginIter)
	{
		std::list<GameEngineRendererBase*>& Renderers = RenderMapBeginIter->second;

		std::list<GameEngineRendererBase*>::iterator BeginIter = Renderers.begin();
		std::list<GameEngineRendererBase*>::iterator EndIter = Renderers.end();

		for (; BeginIter != EndIter; )
		{
			GameEngineRendererBase* ReleaseRenderer = *BeginIter;

			if (nullptr == ReleaseRenderer)
			{
				GameEngineDebug::MsgBoxError("Release Actor Is Nullptr!!!!");
			}

			if (true == ReleaseRenderer->IsDeath())
			{
				BeginIter = Renderers.erase(BeginIter);

				continue;
			}

			++BeginIter;

		}
	}
}

void CameraComponent::NextLevelMoveRenderer(CameraComponent* _NextCamera, GameEngineActor* _Actor)
{
	std::map<int, std::list<GameEngineRendererBase*>>::iterator RenderMapBeginIter = RendererList_.begin();
	std::map<int, std::list<GameEngineRendererBase*>>::iterator RenderMapEndIter = RendererList_.end();
	for (; RenderMapBeginIter != RenderMapEndIter; ++RenderMapBeginIter)
	{
		std::list<GameEngineRendererBase*>& Renderers = RenderMapBeginIter->second;

		std::list<GameEngineRendererBase*>::iterator BeginIter = Renderers.begin();
		std::list<GameEngineRendererBase*>::iterator EndIter = Renderers.end();

		for (; BeginIter != EndIter; )
		{
			GameEngineRendererBase* ReleaseRenderer = *BeginIter;

			if (nullptr == ReleaseRenderer)
			{
				GameEngineDebug::MsgBoxError("Release Actor Is Nullptr!!!!");
			}

			if (ReleaseRenderer->GetActor() == _Actor)
			{
				_NextCamera->RendererList_[(*BeginIter)->GetOrder()].push_back(*BeginIter);

				BeginIter = Renderers.erase(BeginIter);


				continue;
			}

			++BeginIter;

		}
	}
}

void CameraComponent::RenderForward(float _DeltaTime)
{
	float4x4 View = GetTransform()->GetTransformData().View_;
	float4x4 Projection = GetTransform()->GetTransformData().Projection_;

	CameraForwardTarget_->Clear(false);
	CameraForwardTarget_->Setting();

	for (std::pair<int, std::list<GameEngineRendererBase*>> Pair : RendererList_)
	{
		std::list<GameEngineRendererBase*>& Renderers = Pair.second;

		Renderers.sort(ZSort);

		for (GameEngineRendererBase* Renderer : Renderers)
		{
			if (false == Renderer->IsUpdate())
			{
				continue;
			}

			Renderer->GetTransform()->GetTransformData().Projection_ = Projection;
			Renderer->GetTransform()->GetTransformData().View_ = View;

			Renderer->GetTransform()->GetTransformData().WVPCalculation();

			Renderer->Render(_DeltaTime, false);

		}
	}

}

void CameraComponent::RenderDeffered(float _DeltaTime)
{
	float4x4 View = GetTransform()->GetTransformData().View_;
	float4x4 Projection = GetTransform()->GetTransformData().Projection_;

	CameraDeferredGBufferTarget->Clear();
	CameraDeferredGBufferTarget->Setting();

	// 외곽선 없는넘들
	std::list<GameEngineRendererBase*> OutLineList;
	for (std::pair<int, std::list<GameEngineRendererBase*>> Pair : RendererList_)
	{
		std::list<GameEngineRendererBase*>& Renderers = Pair.second;

		Renderers.sort(ZSort);

		for (GameEngineRendererBase* Renderer : Renderers)
		{
			if (false == Renderer->IsUpdate())
			{
				continue;
			}

			if (false == Renderer->IsPreprocessing())
			{
				Renderer->GetTransform()->GetTransformData().Projection_ = Projection;
				Renderer->GetTransform()->GetTransformData().View_ = View;
				Renderer->GetTransform()->GetTransformData().WVPCalculation();
				Renderer->Render(_DeltaTime, true);
			}
			else
			{
				std::map<GameEngineRendererBase*, GameEngineRendererBase*>::iterator FindIter = OutLineRendererList_.find(Renderer);
				if (OutLineRendererList_.end() != FindIter)
				{
					OutLineList.push_back((*FindIter).second);
				}
			}
		}
	}

	// 외곽선 먼저 렌더링
	for (GameEngineRendererBase* Renderer : OutLineList)
	{
		// 외곽선 타겟 셋팅
		CameraOutLineTarget_->Clear(false);
		CameraOutLineTarget_->Setting();

		Renderer->GetTransform()->GetTransformData().Projection_ = Projection;
		Renderer->GetTransform()->GetTransformData().View_ = View;
		Renderer->GetTransform()->GetTransformData().WVPCalculation();
		Renderer->Render(_DeltaTime, true);

		// 본래타겟에 합치고
		CameraDeferredGBufferTarget->Merge(CameraOutLineTarget_);
	}
	OutLineList.clear();

	// 외곽선이 있는얘들
	CameraDeferredGBufferTarget->Setting();
	for (std::pair<int, std::list<GameEngineRendererBase*>> Pair : RendererList_)
	{
		std::list<GameEngineRendererBase*>& Renderers = Pair.second;

		Renderers.sort(ZSort);

		for (GameEngineRendererBase* Renderer : Renderers)
		{
			if (false == Renderer->IsUpdate())
			{
				continue;
			}

			if (true == Renderer->IsPreprocessing())
			{
				Renderer->GetTransform()->GetTransformData().Projection_ = Projection;
				Renderer->GetTransform()->GetTransformData().View_ = View;
				Renderer->GetTransform()->GetTransformData().WVPCalculation();
				Renderer->Render(_DeltaTime, true);
			}
		}
	}

	CalLightEffect.Effect(_DeltaTime);
	DeferredMergeEffect.Effect(_DeltaTime);

}

void CameraComponent::CameraZoomReset()
{
	if (ProjectionMode_ == ProjectionMode::Orthographic) // 직교투영일때
	{
		CamSize_ = GameEngineWindow::GetInst().GetSize();
	}
}

void CameraComponent::CameraZoomSetting(float _Value)
{
	// 카메라가 비추는 화면 비율 조정
	if (ProjectionMode_ == ProjectionMode::Orthographic) // 직교투영일때
	{
		ZoomValue_ = _Value;
		CamSize_ = GameEngineWindow::GetInst().GetSize() * _Value;
	}
	else // 원근투영일때
	{
		// 카메라 z위치를 바꾸면 될거같긴함
	}
}

void CameraComponent::SetProjectionMode(ProjectionMode _ProjectionMode)
{
	ProjectionMode_ = _ProjectionMode;
}

void CameraComponent::PushRenderer(int _Order, GameEngineRendererBase* _Renderer)
{
	RendererList_[_Order].push_back(_Renderer);
}

void CameraComponent::PushLight(GameEngineLightComponent* _Light)
{
	Lights_.push_back(_Light);
}

void CameraComponent::PushOutLineRenderer(GameEngineRendererBase* _BaseRenderer, GameEngineOutlineRenderer* _OutLineRenderer)
{
	OutLineRendererList_[_BaseRenderer] = _OutLineRenderer;
}

void CameraComponent::PushDebugRender(GameEngineTransform* _Trans, CollisionType _Type, float4 _Color)
{
	switch (_Type)
	{
	case CollisionType::Point2D:
	{
		DebugVector_[DebugRenderCount_].PipeLine_ = GameEngineRenderingPipeLineManager::GetInst().Find("DebugRect");
		DebugVector_[DebugRenderCount_].Color_ = _Color;
		break;
	}
	case CollisionType::CirCle:
	{
		DebugVector_[DebugRenderCount_].PipeLine_ = GameEngineRenderingPipeLineManager::GetInst().Find("DebugCircle");
		DebugVector_[DebugRenderCount_].Color_ = _Color;
		break;
	}
	case CollisionType::Rect:
	{
		DebugVector_[DebugRenderCount_].PipeLine_ = GameEngineRenderingPipeLineManager::GetInst().Find("DebugRect");
		DebugVector_[DebugRenderCount_].Color_ = _Color;
		break;
	}
	case CollisionType::OrientedRect:
	{
		DebugVector_[DebugRenderCount_].PipeLine_ = GameEngineRenderingPipeLineManager::GetInst().Find("DebugRect");
		DebugVector_[DebugRenderCount_].Color_ = _Color;
		break;
	}

	case CollisionType::Point3D:
	case CollisionType::Sphere3D:
	{
		DebugVector_[DebugRenderCount_].PipeLine_ = GameEngineRenderingPipeLineManager::GetInst().Find("DebugSphere");
		DebugVector_[DebugRenderCount_].Color_ = _Color;
		break;
	}
	case CollisionType::AABBBox3D:
	{
		DebugVector_[DebugRenderCount_].PipeLine_ = GameEngineRenderingPipeLineManager::GetInst().Find("DebugBox");
		DebugVector_[DebugRenderCount_].Color_ = _Color;
		break;
	}
	case CollisionType::OBBBox3D:
	{
		DebugVector_[DebugRenderCount_].PipeLine_ = GameEngineRenderingPipeLineManager::GetInst().Find("DebugBox");
		DebugVector_[DebugRenderCount_].Color_ = _Color;
		break;
	}
	case CollisionType::MAX:
	{
		GameEngineDebug::MsgBoxError("처리할수 없는 디버그 타입입니다.");
		break;
	}
	}

	DebugVector_[DebugRenderCount_].Data_ = _Trans->GetTransformData();
	++DebugRenderCount_;
}

void CameraComponent::ChangeRendererGroup(int _Group, GameEngineRendererBase* _Renderer)
{
	RendererList_[_Renderer->GetOrder()].remove(_Renderer);
	_Renderer->SetOrder(_Group);
	RendererList_[_Renderer->GetOrder()].push_back(_Renderer);
}

void CameraComponent::Start()
{
	DebugVector_.resize(2000);
	DebugRenderCount_ = 0;

	GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Find("DebugRect");
	for (size_t i = 0; i < DebugVector_.size(); i++)
	{
		DebugVector_[i].ShaderHelper_.ShaderResourcesCheck(Pipe->GetVertexShader());
		DebugVector_[i].ShaderHelper_.ShaderResourcesCheck(Pipe->GetPixelShader());
		DebugVector_[i].Color_ = float4::RED;
		DebugVector_[i].ShaderHelper_.SettingConstantBufferLink("ResultColor", DebugVector_[i].Color_);
		DebugVector_[i].ShaderHelper_.SettingConstantBufferLink("TransformData", DebugVector_[i].Data_);
	}

	CameraBufferTarget_ = new GameEngineRenderTarget();
	CameraBufferTarget_->Create(GameEngineWindow::GetInst().GetSize(), float4::NONE);
	CameraBufferTarget_->CreateDepthBuffer(GameEngineWindow::GetInst().GetSize());

	CameraForwardTarget_ = new GameEngineRenderTarget();
	CameraForwardTarget_->Create(GameEngineWindow::GetInst().GetSize(), float4::NONE);
	CameraForwardTarget_->SetDepthBuffer(CameraBufferTarget_->GetDepthBuffer());

	CameraDeferredGBufferTarget = new GameEngineRenderTarget();
	CameraDeferredGBufferTarget->Create(GameEngineWindow::GetInst().GetSize(), float4::NONE); // 디퓨즈
	CameraDeferredGBufferTarget->Create(GameEngineWindow::GetInst().GetSize(), float4::NONE); // 포지션
	CameraDeferredGBufferTarget->Create(GameEngineWindow::GetInst().GetSize(), float4::NONE); // 노말
	CameraDeferredGBufferTarget->SetDepthBuffer(CameraBufferTarget_->GetDepthBuffer());

	CameraDeferredLightTarget = new GameEngineRenderTarget();
	CameraDeferredLightTarget->Create(GameEngineWindow::GetInst().GetSize(), float4::NONE); // 디퓨즈 라이트
	CameraDeferredLightTarget->Create(GameEngineWindow::GetInst().GetSize(), float4::NONE); // 스펙큘러 라이트
	CameraDeferredLightTarget->Create(GameEngineWindow::GetInst().GetSize(), float4::NONE); // 스펙큘러 라이트

	CameraDeferredTarget_ = new GameEngineRenderTarget();
	CameraDeferredTarget_->Create(GameEngineWindow::GetInst().GetSize(), float4::NONE); // 디퓨즈 라이트

	CalLightEffect.SetTarget(CameraDeferredGBufferTarget);
	CalLightEffect.SetEffect("DeferredCalLight");
	CalLightEffect.SetResult(CameraDeferredLightTarget);
	CalLightEffect.Initialize();
	CalLightEffect.LightDataSetting(this);

	DeferredMergeEffect.SetEffect("DeferredMerge");
	DeferredMergeEffect.SetResult(CameraDeferredTarget_);
	DeferredMergeEffect.SetDeferredTarget(CameraDeferredGBufferTarget, CameraDeferredLightTarget);

	CameraOutLineTarget_ = new GameEngineRenderTarget();
	CameraOutLineTarget_->Create(GameEngineWindow::GetInst().GetSize(), float4::NONE);
	CameraForwardTarget_->SetDepthBuffer(CameraBufferTarget_->GetDepthBuffer());
}

void CameraComponent::Update(float _DeltaTime)
{
}
