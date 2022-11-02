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
	CameraBufferTarget_(nullptr),
	CameraShadowTarget_(nullptr)
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

	if (nullptr != CameraPreprocessingTarget_)
	{
		delete CameraPreprocessingTarget_;
		CameraPreprocessingTarget_ = nullptr;
	}

	if (nullptr != CameraShadowTarget_)
	{
		delete CameraShadowTarget_;
		CameraShadowTarget_ = nullptr;
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

	// 선처리가 필요없는 렌더러를 먼저 그리고,
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
			// 선처리가 필요한 렌더러를 사용하는 렌더러들의 각각의 선처리 렌더러를 수집
			// -> CameraDeferredGBufferTarget에 먼저 렌더링하기 위하여 리스트 수집
			else
			{
				std::map<GameEngineRendererBase*, std::list<GameEngineRendererBase*>>::iterator FindIter = PreprocessingRendererList_.find(Renderer);
				if (PreprocessingRendererList_.end() != FindIter)
				{
					for (GameEngineRendererBase* PreRenderer : (*FindIter).second)
					{
						// Off 상태라면 그리지않는다.
						if (false == PreRenderer->IsUpdate())
						{
							continue;
						}

						OutLineList.push_back(PreRenderer);
					}
				}
			}
		}
	}

	// 선처리렌더러를 먼저그리고,
	for (GameEngineRendererBase* Renderer : OutLineList)
	{
		// 선처리렌더러 타겟 셋팅
		CameraPreprocessingTarget_->Clear(false);
		CameraPreprocessingTarget_->Setting();

		// 선처리렌더러를 그리고나서
		Renderer->GetTransform()->GetTransformData().Projection_ = Projection;
		Renderer->GetTransform()->GetTransformData().View_ = View;
		Renderer->GetTransform()->GetTransformData().WVPCalculation();
		Renderer->Render(_DeltaTime, true);

		// 본래타겟에 합치고
		CameraDeferredGBufferTarget->Merge(CameraPreprocessingTarget_);
	}
	OutLineList.clear();

	// 선처리렌더러를 가지고있는 렌더러들을 그린다.
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

	// Shadow Render
	RenderShadow(_DeltaTime);
	DeferredMergeEffect.Effect(_DeltaTime);
}

void CameraComponent::RenderShadow(float _DeltaTime)
{
	// Shadow Rendering
	std::list<GameEngineLightComponent*>::iterator LightStartIter = Lights_.begin();
	std::list<GameEngineLightComponent*>::iterator LightEndIter = Lights_.end();
	for (; LightStartIter != LightEndIter; ++LightStartIter)
	{
		// Current Light Shadow RenderTarget Setting
		(*LightStartIter)->ShadowTargetSetting();

		// RenderList를 순회하며 그림자렌더링 활성화가 되어있는 렌더러를 타겟에 렌더링
		for (std::pair<int, std::list<GameEngineRendererBase*>> Pair : RendererList_)
		{
			std::list<GameEngineRendererBase*>& Renderers = Pair.second;
			for (GameEngineRendererBase* Renderer : Renderers)
			{
				if (false == Renderer->IsUpdate())
				{
					continue;
				}

				if (1 != Renderer->RendererDataInst.IsShadow)
				{
					continue;
				}

				if (static_cast<unsigned int>(Lights_.size()) != LightData_.LightCount)
				{
					GameEngineDebug::MsgBoxError("라이트 정보가 손상되었습니다.");
					return;
				}

				// Get Light View & Light Projection
				float4x4 LightView = float4x4();
				float4x4 LightProj = float4x4();
				LightView = (*LightStartIter)->GetTransform()->GetTransformData().View_;
				LightProj = (*LightStartIter)->GetTransform()->GetTransformData().Projection_;

				// Calculation Light WorldViewProjection Matrix
				Renderer->GetTransform()->GetTransformData().Projection_ = LightProj;
				Renderer->GetTransform()->GetTransformData().View_ = LightView;
				Renderer->GetTransform()->GetTransformData().WVPCalculation();

				// Shadow Rendering
				Renderer->ShadowRender(_DeltaTime);
			}
		}
	}

	// 임시주석 : DeferredCalLight 제작후 활성화
	//if (nullptr != GetLevel()->ShadowTexture_)
	//{
	//	CalLightEffect.GetShaderRes().SettingTexture("ShadowTex", GetLevel()->ShadowTexture_);
	//}

	CalLightEffect.Effect(_DeltaTime);
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

void CameraComponent::PushPreprocessingRenderer(GameEngineRendererBase* _BaseRenderer, GameEnginePreprocessingRenderer* _PreprocessingRenderer)
{
	PreprocessingRendererList_[_BaseRenderer].push_back(_PreprocessingRenderer);
}

void CameraComponent::PushDebugRender(GameEngineTransform* _Trans, CollisionType _Type, float4 _Color)
{
	if (IsDebugCheck() == true)
	{
		return;
	}

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

	CameraPreprocessingTarget_ = new GameEngineRenderTarget();
	CameraPreprocessingTarget_->Create(GameEngineWindow::GetInst().GetSize(), float4::NONE);
}

void CameraComponent::Update(float _DeltaTime)
{
}
