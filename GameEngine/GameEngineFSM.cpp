#include "PreCompile.h"
#include "GameEngineFSM.h"
#include <GameEngineBase\GameEngineDebug.h>

GameEngineFSM::GameEngineFSM()
	: Current_(nullptr)
	, Next_(nullptr)
{
}

GameEngineFSM::~GameEngineFSM()
{
	for (auto& State : AllState_)
	{
		delete State.second;
		State.second = nullptr;
	}
}

void GameEngineFSM::Update(float _Time)
{
	if (nullptr != Next_)
	{
		if (nullptr != Current_ && nullptr != Current_->End_)
		{
			Current_->End_();
		}
		Current_ = Next_;
		//Current_->Time = 0.0f;

		if (nullptr != Current_->Start_)
		{
			Current_->Start_();
		}
		Next_ = nullptr;
	}

	if (nullptr == Current_)
	{
		GameEngineDebug::MsgBoxError("상태가 존재하지 않는데 상태머신을 작동시키려고 했습니다.");
		return;
	}

	//Current_->Time += _Time;
	Current_->Update_(_Time);
}

void GameEngineFSM::operator<<(const std::string& _stateName)
{
	if (nullptr != Current_)
	{
		if (Current_->Name_ == _stateName)
		{
			return;
		}
	}

	std::map<std::string, State*>::iterator FindIter = AllState_.find(_stateName);

	if (AllState_.end() == FindIter)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 스테이트로 체인지하려고 했습니다..");
		return;
	}

	Next_ = FindIter->second;
}

void GameEngineFSM::CreateState(
	const std::string& _Name,
	std::function<void(float)> _Update,
	std::function<void()> _Start /*= nullptr*/,
	std::function<void()> _EndStart /*= nullptr*/,
	std::function<void()> _Init /*= nullptr*/
)
{
	if (AllState_.end() != AllState_.find(_Name))
	{
		GameEngineDebug::MsgBoxError("이미 존재하는 스테이트를 또 만들려고 했습니다.");
		return;
	}

	if (nullptr != _Init)
	{
		_Init();
	}

	AllState_.insert(std::map<std::string, State*>::value_type(_Name, new State(_Name, _Start, _Update, _EndStart)));
}

void GameEngineFSM::ChangeState(const std::string& _Name, bool _bForceChange)
{
	if (nullptr != Current_)
	{
		if (Current_->Name_ == _Name && _bForceChange == false)
		{
			return;
		}
	}

	std::map<std::string, State*>::iterator FindIter = AllState_.find(_Name);

	if (AllState_.end() == FindIter)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 스테이트로 체인지하려고 했습니다..");
		return;
	}

	Next_ = FindIter->second;
}