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



// 0805 ������
// ChangeAnimation() �� �������� ��� �������� �ʱ�ȭ���ݴϴ�.
void FBXAnimation::ResetFrame()
{
	CurFrame = Start;
	CurFrameTime = 0.0f;
	End = PixAniData->TimeEndCount;
	bEnd_ = false;
}