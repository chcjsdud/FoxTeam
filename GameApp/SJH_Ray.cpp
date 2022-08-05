#include "PreCompile.h"
#include "SJH_Ray.h"

#include <GameEngineBase/GameEngineMath.h>

#include <GameEngine/GameEngineDevice.h>
#include <GameEngine/GameEngineCore.h>
#include <GameEngine/CameraComponent.h>
#include <GameEngine/GameEngineFBXMesh.h>
#include <GameEngine/GameEngineCollision.h>

bool SJH_Ray::IsPicked(const float4& _MousePos, float4& _PickedPos, GameEngineFBXMesh* _Mesh)
{
    if (nullptr == _Mesh)
    {
        return IsColliderPicked(_MousePos, _PickedPos);
    }
    else
    {
        return IsMeshPicked(_Mesh, _MousePos, _PickedPos);
    }

    return false;
}

bool SJH_Ray::IsColliderPicked(const float4& _MousePos, float4& _PickedPos)
{
    // ������ ���念������ �̵�
    // ����� : ����������ġ, �����ǹ���
    if (false == RayAtViewSpace(_MousePos))
    {
        return false;
    }

    // ���� ������ ��� �浹ü ��� Get
    std::map<int, std::list<GameEngineCollision*>> AllList = GetLevel()->GetAllCollision();

    // �ش� ������ �����ϴ� �浹ü Ž��
    int GroupCnt = AllList.size();
    for (int Group = 0; Group < GroupCnt; ++Group)
    {
        std::list<GameEngineCollision*>::iterator StartIter = AllList[Group].begin();
        std::list<GameEngineCollision*>::iterator EndIter = AllList[Group].end();
        for (; StartIter != EndIter; ++StartIter)
        {
            // ������ ���������������� ���õ� �浹ü�� �����ϴ� ���������� �Ÿ�
            float Dist = 0.0f;
            if (true == (*StartIter)->BoundingToRayCollision((*StartIter)->GetCollisionType(), OriginPos_, Direction_, Dist))
            {
                // ������ ������ ��ǥ�� ��ȯ
                _PickedPos = OriginPos_ + (Direction_ * Dist);
                return true;
            }
        }
    }

    // �浹�ϴ� �浹ü�� ���������ʴٸ� ����
    return false;
}

bool SJH_Ray::IsMeshPicked(GameEngineFBXMesh* _Mesh, const float4& _MousePos, float4& _PickedPos)
{
    // ������ ���念������ �̵�
    // ����� : ����������ġ, �����ǹ���
    if (false == RayAtViewSpace(_MousePos))
    {
        return false;
    }

    // Mesh�� ���ٸ� ó�� �Ұ�
    if (nullptr == _Mesh)
    {
        return false;
    }

    // ���������˻縦 ���� Mesh ���� Get
    std::vector<FbxMeshSet>& vecAllMeshSet = _Mesh->GetAllMeshMap();
    std::vector<FbxExMeshInfo>& vecMeshInfo = _Mesh->GetMeshInfos();

    // ����üũ
    float Dist = 0.0f;
    for (int MeshCnt = 0; MeshCnt < static_cast<int>(vecAllMeshSet.size()); ++MeshCnt)
    {
        for (int MaterialCnt = 0; MaterialCnt < static_cast<int>(vecAllMeshSet[MeshCnt].MatialData.size()); ++MaterialCnt)
        {
            for (int FaceNumCnt = 0; FaceNumCnt < vecMeshInfo[MeshCnt].FaceNum; ++FaceNumCnt)
            {
                int Vertex0_Index = vecAllMeshSet[MeshCnt].Indexs[0][MaterialCnt][FaceNumCnt + 0];
                int Vertex1_Index = vecAllMeshSet[MeshCnt].Indexs[0][MaterialCnt][FaceNumCnt + 1];
                int Vertex2_Index = vecAllMeshSet[MeshCnt].Indexs[0][MaterialCnt][FaceNumCnt + 2];

                float4 Vertex0 = vecAllMeshSet[MeshCnt].Vertexs[Vertex0_Index].POSITION * GetTransform()->GetTransformData().WorldWorld_;
                float4 Vertex1 = vecAllMeshSet[MeshCnt].Vertexs[Vertex1_Index].POSITION * GetTransform()->GetTransformData().WorldWorld_;
                float4 Vertex2 = vecAllMeshSet[MeshCnt].Vertexs[Vertex2_Index].POSITION * GetTransform()->GetTransformData().WorldWorld_;

                // ���������� ������������ �Ÿ��� �̿��Ͽ� �ش� ��ǥ�� ��ȯ
                if (true == DirectX::TriangleTests::Intersects(OriginPos_.DirectVector, Direction_.DirectVector, Vertex0.DirectVector, Vertex1.DirectVector, Vertex2.DirectVector, Dist))
                {
                    // ������ ������ ��ǥ�� ��ȯ
                    _PickedPos = OriginPos_ + (Direction_ * Dist);
                    return true;
                }
            }
        }
    }







    // ����üũ
    //float Dist = 0.0f;
    //for (int i = 0; i < vecMeshMap.size(); i++)
    //{
    //    for (int j = 0; j < vecMeshInfos[i].FaceNum; j++)
    //    {
    //        // �ﰢ���� �̷�� ����
    //        float4 V0 = vecMeshMap[i].Vertexs[vecMeshMap[i].Indexs[i][0][j * 3 + 0]].POSITION * GetTransform()->GetTransformData().WorldWorld_;
    //        float4 V1 = vecMeshMap[i].Vertexs[vecMeshMap[i].Indexs[i][0][j * 3 + 1]].POSITION * GetTransform()->GetTransformData().WorldWorld_;
    //        float4 V2 = vecMeshMap[i].Vertexs[vecMeshMap[i].Indexs[i][0][j * 3 + 2]].POSITION * GetTransform()->GetTransformData().WorldWorld_;

    //        // ���������� ������������ �Ÿ��� �̿��Ͽ� �ش� ��ǥ�� ��ȯ
    //        if (true == DirectX::TriangleTests::Intersects(OriginPos_.DirectVector, Direction_.DirectVector, V0.DirectVector, V1.DirectVector, V2.DirectVector, Dist))
    //        {
    //            // ������ ������ ��ǥ�� ��ȯ
    //            _PickedPos = OriginPos_ + (Direction_ * Dist);
    //            return true;
    //        }
    //    }
    //}

    return false;
}

bool SJH_Ray::RayAtViewSpace(float _MousePosX, float _MousePosY)
{
    // 2D ȯ�濡���� ���콺 Ŭ�������� �̿��Ͽ�
    // 3D ȯ�濡���� ���콺 ��ǥ�� ���� ī�޶� �ٶ󺸴� ������ ����Ѵ�.
    OriginPos_ = float4(0.f, 0.f, 0.f, 0.f);

    // 1. ������ ����Ʈ���� -> ��������
    UINT ViewPortNo = 1;
    D3D11_VIEWPORT ViewPort_ = {};
    GameEngineDevice::GetInst().GetContext()->RSGetViewports(&ViewPortNo, &ViewPort_);
    float PointX = ((2.0f * _MousePosX) / ViewPort_.Width) - 1.0f;
    float PointY = (((2.0f * _MousePosY) / ViewPort_.Height) - 1.0f) * -1.0f;
    float PointZ = 1.0f;

    // 2. ������ �������� -> �俵��
    float4x4 ProjMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().Projection_;
    PointX = PointX / ProjMat._11;
    PointY = PointY / ProjMat._22;

    // 3. ������ �俵�� -> ���念��
    float4x4 ViewMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().View_;
    float4x4 InverseViewMat = ViewMat.InverseReturn();
    Direction_.x = (PointX * InverseViewMat._11) + (PointY * InverseViewMat._21) + (PointZ * InverseViewMat._31);
    Direction_.y = (PointX * InverseViewMat._12) + (PointY * InverseViewMat._22) + (PointZ * InverseViewMat._32);
    Direction_.z = (PointX * InverseViewMat._13) + (PointY * InverseViewMat._23) + (PointZ * InverseViewMat._33);
    Direction_.w = 0.0f;
    Direction_.Normalize3D();

    OriginPos_.x = InverseViewMat._41;
    OriginPos_.y = InverseViewMat._42;
    OriginPos_.z = InverseViewMat._43;
    OriginPos_.w = 0.0f;

    return true;
}

bool SJH_Ray::RayAtViewSpace(float4 _MouseClickPos)
{
    // 2D ȯ�濡���� ���콺 Ŭ�������� �̿��Ͽ�
    // 3D ȯ�濡���� ���콺 ��ǥ�� ���� ī�޶� �ٶ󺸴� ������ ����Ѵ�.
    float MousePosX = _MouseClickPos.x;
    float MousePosY = _MouseClickPos.y;

    return RayAtViewSpace(MousePosX, MousePosY);
}

void SJH_Ray::Start()
{
}

void SJH_Ray::Update(float _DeltaTime)
{
}

SJH_Ray::SJH_Ray()
    : OriginPos_(float4::ZERO)
    , Direction_(float4::ZERO)
{
}

SJH_Ray::~SJH_Ray()
{
}
