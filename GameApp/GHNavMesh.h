#pragma once

class GHNavMesh
{
public:
	GHNavMesh();
	~GHNavMesh();

public:
	UINT Index;
	std::vector<UINT> AdjacentMeshIndex;
	float4 Vertices[3];
};

