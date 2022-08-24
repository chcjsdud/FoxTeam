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
	PJW_NeviCell(); // default constructer 디폴트 생성자
	~PJW_NeviCell(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	PJW_NeviCell(const PJW_NeviCell& _other) = delete; // default Copy constructer 디폴트 복사생성자
	PJW_NeviCell(PJW_NeviCell&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	PJW_NeviCell& operator=(const PJW_NeviCell& _other) = delete; // default Copy operator 디폴트 대입 연산자
	PJW_NeviCell& operator=(const PJW_NeviCell&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
};

