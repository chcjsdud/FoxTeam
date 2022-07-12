#include "PreCompile.h"
#include "GameEngineFBXAnimation.h"
#include "GameEngineFBXMeshManager.h"

GameEngineFBXAnimation::GameEngineFBXAnimation()
{
}

GameEngineFBXAnimation::~GameEngineFBXAnimation()
{
}

void GameEngineFBXAnimation::Load(const std::string& _Path)
{
	if (false == CreateFBXSystemInitialize(_Path))
	{
		GameEngineDebug::MsgBoxError("FBX 이니셜라이즈에 실패했습니다.");
		return;
	}

	FBXConvertScene();
	CheckAnimation();
}



// 애니메이션이 존재하는지 안존재하는지만 알아내는 함수.
bool GameEngineFBXAnimation::CheckAnimation()
{
	fbxsdk::FbxArray<FbxString*> AniNameArray;
	Scene->FillAnimStackNameArray(AniNameArray);

	if (0 == AniNameArray.Size())
	{
		return false;
	}

	// fbxsdk::FbxAnimStack::ClassId

	AnimationDatas.resize(AniNameArray.Size());

	for (unsigned int i = 0; i < AnimationDatas.size(); i++)
	{
		//fbxsdk::FbxAnimStack* pAniStatic = Scene->FindMember<fbxsdk::FbxAnimStack>(AniNameArray[i]->Buffer());

		//if (nullptr == pAniStatic)
		//{
		//	GameEngineDebug::MsgBoxError("if (nullptr == pAniStatic)");
		//	continue;
		//}

		// AnimationDatas[i].AniName = GameEngineString::UTF8ToAnsiReturn(pAniStatic->GetName());

		AnimationDatas[i].AniName = GameEngineString::UTF8ToAnsiReturn(AniNameArray[i]->Buffer());

		FbxTakeInfo* TakeInfo = Scene->GetTakeInfo(AnimationDatas[i].AniName.c_str());

		AnimationDatas[i].StartTime = TakeInfo->mLocalTimeSpan.GetStart();
		AnimationDatas[i].EndTime = TakeInfo->mLocalTimeSpan.GetStop();

		// 애니메이션이 분으로 됐는지 초로 됐는지 나노세컨드로 됐는지 프레임 기반으로 저장됐는지 등등
		AnimationDatas[i].TimeMode = Scene->GetGlobalSettings().GetTimeMode();
		AnimationDatas[i].TimeStartCount = AnimationDatas[i].StartTime.GetFrameCount(AnimationDatas[i].TimeMode);

		if (0 >= AnimationDatas[i].TimeStartCount)
		{
			AnimationDatas[i].TimeStartCount *= (FbxLongLong)-1;
		}

		AnimationDatas[i].TimeEndCount = AnimationDatas[i].EndTime.GetFrameCount(AnimationDatas[i].TimeMode);
		AnimationDatas[i].FrameCount = AnimationDatas[i].TimeEndCount - AnimationDatas[i].TimeStartCount;

		AnimationDatas[i].FbxModeCount = (long long)fbxsdk::FbxTime::GetFrameRate(AnimationDatas[i].TimeMode);
		AnimationDatas[i].FbxModeRate = (double)fbxsdk::FbxTime::GetFrameRate(AnimationDatas[i].TimeMode);

		// double Test = fbxsdk::FbxTime::GetFrameRate(m_UserAniData[i].TimeMode);

	}

	for (int i = 0; i < AniNameArray.GetCount(); i++)
	{
		delete AniNameArray[i];
	}


	return true;
}


void GameEngineFBXAnimation::ProcessAnimationLoad(GameEngineFBXMesh* _Mesh, fbxsdk::FbxNode* pNode)
{
	// 모든 노드를 다 훑기 위해서

	fbxsdk::FbxNodeAttribute* pNodeAttribute = pNode->GetNodeAttribute();
	if (nullptr != pNodeAttribute)
	{
		fbxsdk::FbxNodeAttribute::EType typetype = pNodeAttribute->GetAttributeType();
		switch (pNodeAttribute->GetAttributeType())
		{
		case fbxsdk::FbxNodeAttribute::eLODGroup:
		{
			if (0 != pNode->GetChildCount())
			{
				ProcessAnimationLoad(_Mesh, pNode->GetChild(0));
				return;
			}
		}
		break;
		case fbxsdk::FbxNodeAttribute::eMesh:
		{
			AnimationLoad(_Mesh, pNode);
		}
		break;
		default:
			break;
		}
	}

	for (int n = 0; n < pNode->GetChildCount(); ++n)
	{
		ProcessAnimationLoad(_Mesh, pNode->GetChild(n));
	}
}


void GameEngineFBXAnimation::ProcessAnimationCheckState(GameEngineFBXMesh* _Fbx)
{
	size_t userAniDataSize = AnimationDatas.size();
	for (size_t userAniDataIndex = 0; userAniDataIndex < userAniDataSize; ++userAniDataIndex)
	{
		FbxExAniData& userAniData = AnimationDatas.at(userAniDataIndex);
		fbxsdk::FbxLongLong fbxTime = userAniData.EndTime.Get() - userAniData.StartTime.Get() + 1;
		size_t aniFrameDataSize = userAniData.AniFrameData.size();
		for (size_t aniFrameDataIndex = 0; aniFrameDataIndex < aniFrameDataSize; ++aniFrameDataIndex)
		{
			FbxExBoneFrame& aniFrameData = userAniData.AniFrameData.at(aniFrameDataIndex);
			// 비어있을때.
			if (0 == aniFrameData.BoneMatData.size())
			{
				aniFrameData.BoneMatData.resize(fbxTime);
				Bone& curBone = _Fbx->AllBones[aniFrameDataIndex];
				aniFrameData.BoneIndex = curBone.Index;
				aniFrameData.BoneParentIndex = curBone.ParentIndex;
				if (-1 != curBone.ParentIndex)
				{
					FbxExBoneFrame& parentAniFrameData = userAniData.AniFrameData.at(curBone.ParentIndex);
					for (fbxsdk::FbxLongLong start = 0; start < fbxTime; ++start)
					{
						aniFrameData.BoneMatData[start].Time = parentAniFrameData.BoneMatData[start].Time;
						aniFrameData.BoneMatData[start].LocalAnimation = float4x4ToFbxAMatrix(curBone.BonePos.Local);
						aniFrameData.BoneMatData[start].GlobalAnimation = parentAniFrameData.BoneMatData[start].GlobalAnimation * aniFrameData.BoneMatData[start].LocalAnimation;
						aniFrameData.BoneMatData[start].FrameMat = FbxMatTofloat4x4(aniFrameData.BoneMatData[start].GlobalAnimation);
						aniFrameData.BoneMatData[start].S = FbxVecTofloat4(aniFrameData.BoneMatData[start].GlobalAnimation.GetS());
						aniFrameData.BoneMatData[start].Q = FbxQuaternionTofloat4(aniFrameData.BoneMatData[start].GlobalAnimation.GetQ());
						aniFrameData.BoneMatData[start].T = FbxVecToTransform(aniFrameData.BoneMatData[start].GlobalAnimation.GetT());
					}
				}
				else
				{
					for (fbxsdk::FbxLongLong start = 0; start < fbxTime; ++start)
					{
						aniFrameData.BoneMatData[start].Time = 0;
						aniFrameData.BoneMatData[start].LocalAnimation = float4x4ToFbxAMatrix(curBone.BonePos.Local);
						aniFrameData.BoneMatData[start].GlobalAnimation = aniFrameData.BoneMatData[start].LocalAnimation;
						aniFrameData.BoneMatData[start].FrameMat = FbxMatTofloat4x4(aniFrameData.BoneMatData[start].GlobalAnimation);
						aniFrameData.BoneMatData[start].S = FbxVecTofloat4(aniFrameData.BoneMatData[start].GlobalAnimation.GetS());
						aniFrameData.BoneMatData[start].Q = FbxQuaternionTofloat4(aniFrameData.BoneMatData[start].GlobalAnimation.GetQ());
						aniFrameData.BoneMatData[start].T = FbxVecToTransform(aniFrameData.BoneMatData[start].GlobalAnimation.GetT());
					}
				}
			}
		}
	}
}

void GameEngineFBXAnimation::CalFbxExBoneFrameTransMatrix(GameEngineFBXMesh* _Mesh)
{
	if (0 == AnimationDatas.size())
	{
		GameEngineDebug::MsgBoxError("애니메이션 데이터가 존재하지 않는 매쉬로 애니메이션을 만들려고 했습니다.");
		return;
	}

	for (size_t i = 0; i < AnimationDatas.size(); i++)
	{
		AnimationDatas[i].AniFrameData.resize(_Mesh->AllBones.size());
	}

	ProcessAnimationLoad(_Mesh, _Mesh->RootNode);

	ProcessAnimationCheckState(_Mesh);


	AnimationDatas;
	//if (nullptr != _AniProgress)
	//{
	//	_AniProgress(100.0f);
	//}

}



// 노드에 존재하는 정보들을 종합해서 행렬로 리턴해줍니다.
fbxsdk::FbxAMatrix GameEngineFBXAnimation::GetGeometryTransformation(fbxsdk::FbxNode* pNode)
{
	fbxsdk::FbxVector4 translation = pNode->GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);
	fbxsdk::FbxVector4 rotation = pNode->GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot);
	fbxsdk::FbxVector4 scale = pNode->GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot);

	return fbxsdk::FbxAMatrix(translation, rotation, scale);
}


bool GameEngineFBXAnimation::AnimationLoad(GameEngineFBXMesh* _Mesh, fbxsdk::FbxNode* _Node)
{
	// 어마어마하게 부담되는 작업이기 때문에.
	std::vector<FbxExBoneFrame> vecAniFrameData;
	vecAniFrameData.resize(_Mesh->GetBoneCount());

	fbxsdk::FbxMesh* pCurrMesh = _Node->GetMesh();
	int deformerCount = pCurrMesh->GetDeformerCount();
	// 이노드의 기본행렬 정보를 얻어온다.
	fbxsdk::FbxAMatrix geometryTransform = GetGeometryTransformation(_Node);

	// fbxsdk::FbxAnimStack::ClassId
	// 
	//fbxsdk::FbxAnimStack* currAnimStack = Scene->GetSrcObject<fbxsdk::FbxAnimStack>(0);
	//fbxsdk::FbxString animStackName = currAnimStack->GetName();
	for (size_t i = 0; i < AnimationDatas.size(); i++)
	{
		fbxsdk::FbxTakeInfo* takeInfo = Scene->GetTakeInfo(AnimationDatas[i].AniName.c_str());
		fbxsdk::FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
		fbxsdk::FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
		fbxsdk::FbxTime::EMode timeMode = Scene->GetGlobalSettings().GetTimeMode();

		fbxsdk::FbxAMatrix currentTransformOffset;
		fbxsdk::FbxAMatrix localTransform;
		fbxsdk::FbxAMatrix globalTransform;

		fbxsdk::FbxCluster* pCurrCluster;
		std::string currJointName;
		Bone* pBone;

		fbxsdk::FbxAMatrix transformMatrix;
		fbxsdk::FbxAMatrix transformLinkMatrix;
		fbxsdk::FbxAMatrix globalBindposeInverseMatrix;

		fbxsdk::FbxLongLong fixIndex;

		std::string linkName;

		fbxsdk::FbxLongLong endTime;
		fbxsdk::FbxLongLong startTime;

		fbxsdk::FbxTime currTime;


		for (int deformerIndex = 0; deformerIndex < deformerCount; ++deformerIndex)
		{
			fbxsdk::FbxSkin* pCurrSkin = reinterpret_cast<fbxsdk::FbxSkin*>(pCurrMesh->GetDeformer(deformerIndex, fbxsdk::FbxDeformer::eSkin));
			if (nullptr == pCurrSkin)
			{
				continue;
			}

			unsigned int numOfClusters = pCurrSkin->GetClusterCount();

			endTime = end.GetFrameCount(timeMode);
			startTime = start.GetFrameCount(timeMode);

			FbxExAniData& CurAniData = AnimationDatas[i];
			CurAniData.EndTime = endTime;
			CurAniData.StartTime = startTime;
			CurAniData.TimeMode = timeMode;

			for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
			{
				pCurrCluster = pCurrSkin->GetCluster(clusterIndex);
				currJointName = pCurrCluster->GetLink()->GetName();
				pBone = _Mesh->FindBone(currJointName);

				pCurrCluster->GetTransformMatrix(transformMatrix);
				pCurrCluster->GetTransformLinkMatrix(transformLinkMatrix);
				globalBindposeInverseMatrix = transformLinkMatrix.Inverse();

				linkName = pCurrCluster->GetLink()->GetName();
				fbxsdk::FbxNode* pLinkNode = Scene->FindNodeByName(linkName.c_str());

				FbxExBoneFrame& Frame = CurAniData.AniFrameData[pBone->Index];
				Frame.BoneMatData.resize(endTime - startTime + 1);
				Frame.BoneIndex = pBone->Index;
				Frame.BoneParentIndex = pBone->ParentIndex;

				for (fbxsdk::FbxLongLong i = startTime; i <= endTime; ++i)
				{
					fixIndex = i - startTime;

					FbxExBoneFrameData& FrameData = Frame.BoneMatData[fixIndex];

					currTime.SetFrame(fixIndex, timeMode);
					// 로
					currentTransformOffset = _Node->EvaluateGlobalTransform(currTime) * JointMatrix * geometryTransform;
					globalTransform = currentTransformOffset.Inverse() * pLinkNode->EvaluateGlobalTransform(currTime);

					localTransform.SetS(pLinkNode->EvaluateLocalScaling(currTime));
					localTransform.SetR(pLinkNode->EvaluateLocalRotation(currTime));
					localTransform.SetT(pLinkNode->EvaluateLocalTranslation(currTime));

					FrameData.Time = currTime.GetSecondDouble();
					FrameData.LocalAnimation = localTransform;
					FrameData.GlobalAnimation = globalTransform;
					FrameData.FrameMat = FbxMatTofloat4x4(FrameData.GlobalAnimation);
					FrameData.S = FbxVecTofloat4(FrameData.GlobalAnimation.GetS());
					FrameData.Q = FbxQuaternionTofloat4(FrameData.GlobalAnimation.GetQ());
					FrameData.T = FbxVecToTransform(FrameData.GlobalAnimation.GetT());
				}
			}
		}
	}

	return true;
}