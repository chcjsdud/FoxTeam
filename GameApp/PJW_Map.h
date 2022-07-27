#pragma once

// 분류 : 
// 용도 : 
// 설명 : 
class PJW_Map
{
public:
	PJW_Map();
	~PJW_Map();
	PJW_Map(const PJW_Map& _other) = delete;
	PJW_Map(PJW_Map&& _other) noexcept; 
	PJW_Map& operator=(const PJW_Map& _other) = delete; 
	PJW_Map& operator=(const PJW_Map&& _other) = delete; 

public:
};

