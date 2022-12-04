#pragma once
#include <Windows.h>
#include <process.h>

// �з� : IOCP(Input Output Completion Port)
// �뵵 : Thread Management
// ���� : �ּ����� �����带 ����ؼ� Port�� ���õ� ������� ó���ϴ� ���
//        1. �ּ����� ������ ���
//           -. ���к��� ������ �Ҵ翡 ���Ǵ� ��� ���� -> ������Ǯ(Thread Pool) ���
//           -. ���ؽ�Ʈ ����Ī(Context Switchgin) ��� ���� -> CPU�� Dispatch�Ǵ� ������ ���� ����
//        2. Overraped I/O(I/O ó���� ���� �۾��Ҵ�� �ֽ������ �Ҵ�� �۾��Ϸ���� ���)�� Ȯ�� ����
//           -. �۾��Ҵ� �� ��� ���� ó������
class GameEngineIocp
{
public:
	bool Start(int ThreadCount = 0)
	{
		// CreateIoCompletionPort() : IOCP Ŀ�� ������Ʈ�� ����(�ʱ�ȭ)
		// 1. IOCP Ŀ�� ��ü�� ����
		// 2. IOCP�� ����̽��� ����
		// ù��° ���� : _In_ HANDLE FileHandle -> ������ ����̽� �ڵ� 
		// �ι�° ���� : _In_opt_ HANDLE ExistingCompletionPort -> ù��° ���ڿ� ������ IOCP�� �ڵ�(ù��° ���ڰ� INVALID_HANDLE_VALUE��� NULL�� �Է�)
		// ����° ���� : _In_ ULONG_PTR CompletionKey -> ������ ����̽� ��� ������ Ű ���(�񵿱� ������ �´ٸ� �ش� Ű�� ���ؼ� ���а���)
		// ������ ���� : ó���ϴ� �����尡 ���� ���డ���� ��(IOCP �ڵ��� �����Ҷ��� ���)
		// ���� : �Ҵ�� IOCP �ڵ�(nullptr : ���� ����)
		IocpHandle_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, ThreadCount);
		if (nullptr == IocpHandle_)
		{
			return false;
		}

		return true;
	}

	BOOL WaitforWork(DWORD& lpNumberOfBytesTransferred, ULONG_PTR& lpCompletionKey, LPOVERLAPPED& lpOverlapped, DWORD dwMilliseconds = INFINITE)
	{
		// GetQueuedCompletionStatus() : �񵿱� �Ϸ� �������� ���(�۾����)
		// 1. IOCP�� I/O completion queue���� �����Ͱ� �Էµɶ����� ���
		// ù��° ���� : _In_ HANDLE CompletionPort -> ��⸦ ������ ��� IOCP �ڵ�
		// �ι�° ���� : _Out_ LPDWORD lpNumberOfBytesTransferred -> �ۼ��ŵ� ����Ʈ ��
		// ����° ���� : _Out_ PULONG_PTR lpCompletionKey -> �񵿱� I/O ��û�� �߻��� ����̽��� completion Key
		// �׹�° ���� : _Out_ LPOVERLAPPED* lpOverlapped -> �񵿱� ȣ��� ������ Overlapped ����ü�� �ּ�
		// ������ ���� : _In_ DWORD dwMilliseconds -> ��⸦ ������ �ð�(�и���)
		// ���� : TRUE(����), FALSE(����)
		return GetQueuedCompletionStatus(IocpHandle_, &lpNumberOfBytesTransferred, &lpCompletionKey, &lpOverlapped, dwMilliseconds);
	}

	BOOL Makework(DWORD _WorkParameter, void* _Ptr)
	{
		// PostQueuedCompletionStatus() : ����ڰ� I/O Completion queue�� �Է��� ��ó�� �޼����� ����(�۾��Ҵ�)
		// 1. ����ڰ� ���ϴ� �۾��� �Ҵ�
		// ù��° ���� : _In_ HANDLE CompletionPort -> �޼����� ������ ��� IOCP �ڵ�
		// �ι�° ���� : _In_ DWORD dwNumberOfBytesTransferred -> GetQueuedCompletionStatus()���� ���޵� �ι�° ����(�ۼ��ŵ� ����Ʈ��)
		// ����° ���� : _In_ ULONG_PTR dwCompletionKey -> GetQueuedCompletionStatus()���� ���޵� ����° ����(�񵿱� I/O ��û�� �߻��� ����̽��� completion Key)
		// ������ ���� : _In_opt_ LPOVERLAPPED lpOverlapped -> GetQueuedCompletionStatus()���� ���޵� �׹�° ����(�񵿱� ȣ��� ������ Overlapped ����ü�� �ּ�)
		// ���� : 0(����)
		return PostQueuedCompletionStatus(IocpHandle_, _WorkParameter, reinterpret_cast<ULONG_PTR>(_Ptr), nullptr);
	}

protected:
private:

public:
	GameEngineIocp() : IocpHandle_(nullptr)
	{
	}	
	
	~GameEngineIocp() 
	{
	}

public:
protected:
private:
	HANDLE IocpHandle_;
};

