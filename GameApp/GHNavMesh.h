#pragma once

class GHNavMesh
{
public:
	GHNavMesh();
	~GHNavMesh();

public:
	int Index;
	std::vector<int> AdjacentMeshIndices;
	float4 Vertices[3];
};

