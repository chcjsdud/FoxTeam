#pragma once
#include "GameEngineThread.h"
#include "GameEngineIocp.h"

// �з� : Thread Queue
// �뵵 : Job Management
// ���� : 
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
			// �۾� �Ϸ���� ���
			// lpNumberOfBytesTransferred : �ۼ��ŵ� ����Ʈ ��
			// lpCompletionKey : �񵿱� I/O ��û�� �߻��� ����̽��� completion Key(IOCP�� �۾��Ҵ�� lpNumberOfBytesTransferred�� -2�� ���)
			// Ptr : �񵿱� ȣ��� ������ Overlapped ����ü�� �ּ�(IOCP�� �۾��Ҵ�� lpCompletionKey�� �ش� �۾��� ȣ��Ǵ� �Լ� ������ ���)
			_Iocp->WaitforWork(lpNumberOfBytesTransferred, lpCompletionKey, Ptr);
			switch (lpNumberOfBytesTransferred)
			{
			case -1:
			{
				return;
			}
			case -2:
			{
				// �񵿱� �Ϸ� ������ �޾����Ƿ�
				workingCount_++;

				// �ش� �۾��� �����ϰ�
				Job* PostJob = reinterpret_cast<Job*>(lpCompletionKey);
				PostJob->JobFunction();
				delete PostJob;

				// �۾��Ϸ�
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
		// ����ڰ� ���ϴ� �۾��� �Ҵ�
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
			// �ش� �ý��ۿ��� �����ϴ� �����尹�� Ȱ��
			SYSTEM_INFO Info;
			GetSystemInfo(&Info);
			ThreadCount_ = Info.dwNumberOfProcessors - 2;
		}

		// ������ 1���� �Ҵ�
		if (ThreadCount_ <= 0)
		{
			ThreadCount_ = 1;
		}

		// ó���ϴ� �����尡 ���� ���డ���� �� �����Ͽ� �̸� �Ҵ�
		Iocp_.Start(ThreadCount_);
		for (size_t i = 0; i < ThreadCount_; i++)
		{
			// �۾� �Ҵ�
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

