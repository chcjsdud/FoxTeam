#pragma once
#include "GameEngineThread.h"
#include "GameEngineIocp.h"

// 분류 : Thread Queue
// 용도 : Job Management
// 설명 : 
class GameEngineThreadQueue
{
private:
	class Job
	{
	public:
		std::function<void()> JobFunction;
	};

private:
	static std::atomic<int> workingCount_;

public:
	static void JobWorkThread(GameEngineThread* Thread, GameEngineIocp* _Iocp)
	{
		DWORD lpNumberOfBytesTransferred = 0;
		ULONG_PTR lpCompletionKey = 0;
		LPOVERLAPPED Ptr = nullptr;
		while (true)
		{
			// 작업 완료까지 대기
			// lpNumberOfBytesTransferred : 송수신된 바이트 수
			// lpCompletionKey : 비동기 I/O 요청이 발생한 디바이스의 completion Key(IOCP에 작업할당시 lpNumberOfBytesTransferred를 -2로 등록)
			// Ptr : 비동기 호출시 전달할 Overlapped 구조체의 주소(IOCP에 작업할당시 lpCompletionKey에 해당 작업시 호출되는 함수 포인터 등록)
			_Iocp->WaitforWork(lpNumberOfBytesTransferred, lpCompletionKey, Ptr);
			switch (lpNumberOfBytesTransferred)
			{
			case -1:
			{
				return;
			}
			case -2:
			{
				// 비동기 완료 통지를 받았으므로
				workingCount_++;

				// 해당 작업을 실행하고
				Job* PostJob = reinterpret_cast<Job*>(lpCompletionKey);
				PostJob->JobFunction();
				delete PostJob;

				// 작업완료
				workingCount_--;
				break;
			}
			default:
				break;
			}
		}
	}

public:
	inline int GetWorkingCount() const 
	{
		return workingCount_;
	}

public:
	void JobPost(std::function<void()> _Job)
	{
		// 사용자가 원하는 작업을 할당
		Job* NewJob = new Job();
		NewJob->JobFunction = _Job;
		Iocp_.Makework(-2, NewJob);
	}

	void Destroy()
	{
		for (size_t i = 0; i < ThreadCount_; i++)
		{
			Iocp_.Makework(-1, nullptr);
			Sleep(1);
		}

		for (size_t i = 0; i < Threads_.size(); i++)
		{
			delete Threads_[i];
			Threads_[i] = nullptr;
		}

		Threads_.clear();
	}

protected:
private:

public:
	GameEngineThreadQueue(const std::string& _ThreadName, int _ThreadCount = 0)
	{
		ThreadCount_ = _ThreadCount;
		if (ThreadCount_ == 0)
		{
			// 해당 시스템에서 제공하는 스레드갯수 활용
			SYSTEM_INFO Info;
			GetSystemInfo(&Info);
			ThreadCount_ = Info.dwNumberOfProcessors - 2;
		}

		// 스레드 1개라도 할당
		if (ThreadCount_ <= 0)
		{
			ThreadCount_ = 1;
		}

		// 처리하는 스레드가 동시 수행가능한 수 지정하여 미리 할당
		Iocp_.Start(ThreadCount_);
		for (size_t i = 0; i < ThreadCount_; i++)
		{
			// 작업 할당
			GameEngineThread* NewThread = new GameEngineThread();
			std::string ThreadName = _ThreadName + std::to_string(static_cast<int>(i));
			NewThread->Start(ThreadName, std::bind(JobWorkThread, NewThread, &Iocp_));
			Threads_.push_back(NewThread);
		}
	}

	~GameEngineThreadQueue()
	{
		Destroy();
	}

private:
	GameEngineIocp Iocp_;
	int ThreadCount_;
	std::vector<GameEngineThread*> Threads_;
};

