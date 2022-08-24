#pragma once
#include <GameEngine/EngineVertex.h>



class PJW_NeviCell
{
public:
	//inline int GetCellInformationIndex() const
	//{
	//	return index_;
	//}
	//
	//inline bool GetFaceInformationFlag() const
	//{
	//	return faceInfoCompleted_;
	//}
	//
	//inline float4 GetCenterToGravity() const
	//{
	//	return centerOfGravity_;
	//}
	//
	//inline std::vector<PJW_NeviCell*> GetAdjacentTriangles() const
	//{
	//	return adjacentTriangles_;
	//}
	//
public:
	void CreateNaviCellInfo();


private:	// member Var

public:
	PJW_NeviCell(); // default constructer ����Ʈ ������
	~PJW_NeviCell(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	PJW_NeviCell(const PJW_NeviCell& _other) = delete; // default Copy constructer ����Ʈ ���������
	PJW_NeviCell(PJW_NeviCell&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	PJW_NeviCell& operator=(const PJW_NeviCell& _other) = delete; // default Copy operator ����Ʈ ���� ������
	PJW_NeviCell& operator=(const PJW_NeviCell&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
};

