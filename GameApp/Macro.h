#pragma once

#define CINT(type) static_cast<int>(type)

#define MakeState(ClassName, StateName) #StateName, std::bind(&ClassName::update##StateName, this, std::placeholders::_1) \
		, std::bind(&ClassName::start##StateName, this)

#define MakeStateWithEnd(ClassName, StateName) #StateName, std::bind(&ClassName::update##StateName, this, std::placeholders::_1) \
		, std::bind(&ClassName::start##StateName, this) \
		, std::bind(&ClassName::end##StateName, this)