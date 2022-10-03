#include "PreCompile.h"

#include "GameEngineFBXMesh.h"
#include "GameEngineFBXMeshManager.h"
#include "GameEngineFBXAnimation.h"
#include "GameEngineFBXAnimationManager.h"
#include "GameEngineTextureManager.h"

void FBXAnimation::Init(int _Index, bool _isLoop, float _frameTime)
{
	Animation->CalFbxExBoneFrameTransMatrix(Mesh, _Index);
	PixAniData = &Animation->AnimationDatas[_Index];
	Start = 0;



	End = PixAniData->TimeEndCount;
	bEnd_ = false;



	FrameTime = _frameTime;

	// 0805 박종원 : 애니메이션 만들 시 루프/어웨이크를 결정짓는 bool 값입니다.
	isLoop_ = _isLoop;
}

void FBXAnimation::Update(float _DeltaTime)
{
	CurFrameTime += _DeltaTime;

	if (CurFrameTime >= FrameTime)
	{
		CurFrameTime -= FrameTime;

		++CurFrame;
	}

	int NextFrame = CurFrame;
	++NextFrame;

	if (CurFrame >= End) // 현 프레임이 끝 프레임에 다다랐을 때
	{
		// 0805 박종원 : 루프 애니메이션이 아니면 더 이상의 프레임 갱신을 하지 않습니다.
		if (false == isLoop_)
		{
			bEnd_ = true;
			return;
		}

		CurFrame = Start;
	}

	if (NextFrame >= End)
	{
		NextFrame = 0;
	}

	for (int o = 0; o < ParentRenderer->RenderSets.size(); o++)
	{
		RenderSet& Render = ParentRenderer->RenderSets[o];

		for (int i = 0; i < Render.BoneData.size(); i++)  // 메시의 모든 본을 for 문으로 돌며 애니메이션을 갱신시켜줌.
		{
			Bone* BoneData = ParentRenderer->FBXMesh->FindBone(Render.Index, i);

			if (true == PixAniData->AniFrameData[Render.Index][i].BoneMatData.empty())
			{
				// 본의 프레임데이터가 없을때가 있습니다.
				// 보통 항등행렬이 들어가 있는 경우가 많습니다.

				// GameEngineDebug::MsgBoxError("아핀행렬 만들어라!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				Render.BoneData[i] = float4x4::Affine(BoneData->BonePos.GlobalScale, BoneData->BonePos.GlobalRotation, BoneData->BonePos.GlobalTranslation);
				return;
			}

			// 현재프레임과 
			FbxExBoneFrameData& CurData = PixAniData->AniFrameData[Render.Index][i].BoneMatData[CurFrame];
			// 다음프레임의 정보가 필요한데
			FbxExBoneFrameData& NextData = PixAniData->AniFrameData[Render.Index][i].BoneMatData[NextFrame];

			if (CurData.FrameMat == NextData.FrameMat)
			{
				int a = 0;
				return;
			}


			// 로컬 스케일
			float4 LerpScale = float4::Lerp(CurData.S, NextData.S, CurFrameTime);
			// 로컬 쿼터니온
			float4 SLerpQ = float4::SLerp(CurData.Q, NextData.Q, CurFrameTime);
			// 로컬 포지션
			float4 LerpPos = float4::Lerp(CurData.T, NextData.T, CurFrameTime);

			// 


			size_t Size = sizeof(float4x4);
			// 자신의 원본행렬과 곱해준다 큰 의미는 없다.

			float4x4 Mat = float4x4::Affine(LerpScale, SLerpQ, LerpPos);

			//ParentRenderer->BoneData[i].Pos = LerpPos;
			//ParentRenderer->BoneData[i].Q = SLerpQ;
			//ParentRenderer->BoneData[i].Scale = LerpScale;

			//FrameBoneData[i].AniScaleMat = float4x4::Affine(LerpScale, float4::QIden, float4::ZERO);
			//FrameBoneData[i].AniRotMat = float4x4::Affine(float4::ONE, SLerpQ, float4::NONE);
			//FrameBoneData[i].AniPosMat = float4x4::Affine(float4::ONE, float4::QIden, LerpPos);

			// FrameBoneData[i].AniWorldMat = float4x4::Affine(LerpScale, SLerpQ, LerpPos);


			// 로컬에서의 애니메이션된 행렬
			Render.BoneData[i] = BoneData->BonePos.Offset * float4x4::Affine(LerpScale, SLerpQ, LerpPos);

			// ParentRenderer->BoneData[i].Transpose();
		}
	}
}



// 0805 박종원
// ChangeAnimation() 시 마지막에 모든 프레임을 초기화해줍니다.
void FBXAnimation::ResetFrame()
{
	CurFrame = Start;
	CurFrameTime = 0.0f;
	End = PixAniData->TimeEndCount;
	bEnd_ = false;
}