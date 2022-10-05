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

	// 0805 ������ : �ִϸ��̼� ���� �� ����/�����ũ�� �������� bool ���Դϴ�.
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

	if (CurFrame >= End) // �� �������� �� �����ӿ� �ٴٶ��� ��
	{
		// 0805 ������ : ���� �ִϸ��̼��� �ƴϸ� �� �̻��� ������ ������ ���� �ʽ��ϴ�.
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

		for (int i = 0; i < Render.BoneData.size(); i++)  // �޽��� ��� ���� for ������ ���� �ִϸ��̼��� ���Ž�����.
		{
			Bone* BoneData = ParentRenderer->FBXMesh->FindBone(Render.Index, i);

			if (true == PixAniData->AniFrameData[Render.Index][i].BoneMatData.empty())
			{
				// ���� �����ӵ����Ͱ� �������� �ֽ��ϴ�.
				// ���� �׵������ �� �ִ� ��찡 �����ϴ�.

				// GameEngineDebug::MsgBoxError("������� ������!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				Render.BoneData[i] = float4x4::Affine(BoneData->BonePos.GlobalScale, BoneData->BonePos.GlobalRotation, BoneData->BonePos.GlobalTranslation);
				return;
			}

			// ���������Ӱ� 
			FbxExBoneFrameData& CurData = PixAniData->AniFrameData[Render.Index][i].BoneMatData[CurFrame];
			// ������������ ������ �ʿ��ѵ�
			FbxExBoneFrameData& NextData = PixAniData->AniFrameData[Render.Index][i].BoneMatData[NextFrame];

			if (CurData.FrameMat == NextData.FrameMat)
			{
				int a = 0;
				return;
			}


			// ���� ������
			float4 LerpScale = float4::Lerp(CurData.S, NextData.S, CurFrameTime);
			// ���� ���ʹϿ�
			float4 SLerpQ = float4::SLerp(CurData.Q, NextData.Q, CurFrameTime);
			// ���� ������
			float4 LerpPos = float4::Lerp(CurData.T, NextData.T, CurFrameTime);

			// 


			size_t Size = sizeof(float4x4);
			// �ڽ��� ������İ� �����ش� ū �ǹ̴� ����.

			float4x4 Mat = float4x4::Affine(LerpScale, SLerpQ, LerpPos);

			//ParentRenderer->BoneData[i].Pos = LerpPos;
			//ParentRenderer->BoneData[i].Q = SLerpQ;
			//ParentRenderer->BoneData[i].Scale = LerpScale;

			//FrameBoneData[i].AniScaleMat = float4x4::Affine(LerpScale, float4::QIden, float4::ZERO);
			//FrameBoneData[i].AniRotMat = float4x4::Affine(float4::ONE, SLerpQ, float4::NONE);
			//FrameBoneData[i].AniPosMat = float4x4::Affine(float4::ONE, float4::QIden, LerpPos);

			// FrameBoneData[i].AniWorldMat = float4x4::Affine(LerpScale, SLerpQ, LerpPos);


			// ���ÿ����� �ִϸ��̼ǵ� ���
			Render.BoneData[i] = BoneData->BonePos.Offset * float4x4::Affine(LerpScale, SLerpQ, LerpPos);

			// ParentRenderer->BoneData[i].Transpose();
		}
	}
}

void FBXAnimation::UpdateOverride(float _deltaTime, FBXAnimation* _overrideAnimation)
{
	FBXAnimation* oa = _overrideAnimation;

	CurFrameTime += _deltaTime;


	if (CurFrameTime >= FrameTime)
	{
		CurFrameTime -= FrameTime;
		++CurFrame;
	}

	int NextFrame = CurFrame;
	++NextFrame;

	if (CurFrame >= End)
	{
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

	oa->CurFrameTime += _deltaTime;
	if (oa->CurFrameTime >= oa->FrameTime)
	{
		oa->CurFrameTime -= oa->FrameTime;
		++oa->CurFrame;
	}
	int oaNextFrame = oa->CurFrame;
	++oaNextFrame;

	if (oa->CurFrame >= oa->End)
	{
		if (false == oa->isLoop_)
		{
			oa->bEnd_ = true;
			return;
		}

		oa->CurFrame = oa->Start;
	}
	if (oaNextFrame >= oa->End)
	{
		oaNextFrame = 0;
	}


	for (int o = 0; o < ParentRenderer->RenderSets.size(); o++)
	{
		RenderSet& Render = ParentRenderer->RenderSets[o];

		for (int i = 0; i < Render.BoneData.size(); i++)
		{
			Bone* BoneData = ParentRenderer->FBXMesh->FindBone(Render.Index, i);

			if (ParentRenderer->overrideBoneIndexCache_.end() == ParentRenderer->overrideBoneIndexCache_.find(BoneData->Index))
			{
				if (true == PixAniData->AniFrameData[Render.Index][i].BoneMatData.empty())
				{
					Render.BoneData[i] = float4x4::Affine(BoneData->BonePos.GlobalScale, BoneData->BonePos.GlobalRotation, BoneData->BonePos.GlobalTranslation);
					return;
				}

				FbxExBoneFrameData& CurData = PixAniData->AniFrameData[Render.Index][i].BoneMatData[CurFrame];
				FbxExBoneFrameData& NextData = PixAniData->AniFrameData[Render.Index][i].BoneMatData[NextFrame];

				if (CurData.FrameMat == NextData.FrameMat)
				{
					return;
				}

				float4 LerpScale = float4::Lerp(CurData.S, NextData.S, CurFrameTime);
				float4 SLerpQ = float4::SLerp(CurData.Q, NextData.Q, CurFrameTime);
				float4 LerpPos = float4::Lerp(CurData.T, NextData.T, CurFrameTime);

				Render.BoneData[i] = BoneData->BonePos.Offset * float4x4::Affine(LerpScale, SLerpQ, LerpPos);
			}
			else
			{
				if (true == oa->PixAniData->AniFrameData[Render.Index][i].BoneMatData.empty())
				{
					Render.BoneData[i] = float4x4::Affine(BoneData->BonePos.GlobalScale, BoneData->BonePos.GlobalRotation, BoneData->BonePos.GlobalTranslation);
					return;
				}

				FbxExBoneFrameData& oaCurData = oa->PixAniData->AniFrameData[Render.Index][i].BoneMatData[oa->CurFrame];
				FbxExBoneFrameData& oaNextData = oa->PixAniData->AniFrameData[Render.Index][i].BoneMatData[oaNextFrame];

				if (oaCurData.FrameMat == oaNextData.FrameMat)
				{
					return;
				}

				float4 oaLerpScale = float4::Lerp(oaCurData.S, oaNextData.S, oa->CurFrameTime);
				float4 oaSLerpQ = float4::SLerp(oaCurData.Q, oaNextData.Q, oa->CurFrameTime);
				float4 oaLerpPos = float4::Lerp(oaCurData.T, oaNextData.T, oa->CurFrameTime);

				float4x4 matOverrideAnim = float4x4::Affine(oaLerpScale, oaSLerpQ, oaLerpPos);

				matOverrideAnim.vw += ParentRenderer->overrideBoneOffset_;

				Render.BoneData[i] = BoneData->BonePos.Offset * matOverrideAnim;

			}
		}
	}
}



// 0805 ������
// ChangeAnimation() �� �������� ��� �������� �ʱ�ȭ���ݴϴ�.
void FBXAnimation::ResetFrame()
{
	CurFrame = Start;
	CurFrameTime = 0.0f;
	End = PixAniData->TimeEndCount;
	bEnd_ = false;
}