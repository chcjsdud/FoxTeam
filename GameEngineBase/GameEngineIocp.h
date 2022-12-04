#pragma once
#include <Windows.h>
#include <process.h>

// 분류 : IOCP(Input Output Completion Port)
// 용도 : Thread Management
// 설명 : 최소한의 스레드를 사용해서 Port와 관련된 입출력을 처리하는 기법
//        1. 최소한의 스레드 사용
//           -. 무분별한 스레드 할당에 사용되는 비용 절감 -> 스레드풀(Thread Pool) 사용
//           -. 컨텍스트 스위칭(Context Switchgin) 비용 절감 -> CPU에 Dispatch되는 스레드 수를 조절
//        2. Overraped I/O(I/O 처리를 위한 작업할당시 주스레드는 할당된 작업완료까지 대기)의 확장 개념
//           -. 작업할당 후 즉시 다음 처리가능
class GameEngineIocp
{
public:
	bool Start(int ThreadCount = 0)
	{
		// CreateIoCompletionPort() : IOCP 커널 오브젝트를 생성(초기화)
		// 1. IOCP 커널 객체를 생성
		// 2. IOCP와 디바이스를 연결
		// 첫번째 인자 : _In_ HANDLE FileHandle -> 연결할 디바이스 핸들 
		// 두번째 인자 : _In_opt_ HANDLE ExistingCompletionPort -> 첫번째 인자와 연결할 IOCP의 핸들(첫번째 인자가 INVALID_HANDLE_VALUE라면 NULL을 입력)
		// 세번째 인자 : _In_ ULONG_PTR CompletionKey -> 연결할 디바이스 장비에 고유한 키 등록(비동기 응답이 온다면 해당 키를 통해서 구분가능)
		// 마지막 인자 : 처리하는 스레드가 동시 수행가능한 수(IOCP 핸들을 생성할때만 사용)
		// 리턴 : 할당된 IOCP 핸들(nullptr : 생성 실패)
		IocpHandle_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, ThreadCount);
		if (nullptr == IocpHandle_)
		{
			return false;
		}

		return true;
	}

	BOOL WaitforWork(DWORD& lpNumberOfBytesTransferred, ULONG_PTR& lpCompletionKey, LPOVERLAPPED& lpOverlapped, DWORD dwMilliseconds = INFINITE)
	{
		// GetQueuedCompletionStatus() : 비동기 완료 통지까지 대기(작업대기)
		// 1. IOCP의 I/O completion queue에서 데이터가 입력될때까지 대기
		// 첫번째 인자 : _In_ HANDLE CompletionPort -> 대기를 수행할 대상 IOCP 핸들
		// 두번째 인자 : _Out_ LPDWORD lpNumberOfBytesTransferred -> 송수신된 바이트 수
		// 세번째 인자 : _Out_ PULONG_PTR lpCompletionKey -> 비동기 I/O 요청이 발생한 디바이스의 completion Key
		// 네번째 인자 : _Out_ LPOVERLAPPED* lpOverlapped -> 비동기 호출시 전달할 Overlapped 구조체의 주소
		// 마지막 인자 : _In_ DWORD dwMilliseconds -> 대기를 수행할 시간(밀리초)
		// 리턴 : TRUE(성공), FALSE(실패)
		return GetQueuedCompletionStatus(IocpHandle_, &lpNumberOfBytesTransferred, &lpCompletionKey, &lpOverlapped, dwMilliseconds);
	}

	BOOL Makework(DWORD _WorkParameter, void* _Ptr)
	{
		// PostQueuedCompletionStatus() : 사용자가 I/O Completion queue를 입력한 것처럼 메세지를 전송(작업할당)
		// 1. 사용자가 원하는 작업을 할당
		// 첫번째 인자 : _In_ HANDLE CompletionPort -> 메세지를 전송할 대상 IOCP 핸들
		// 두번째 인자 : _In_ DWORD dwNumberOfBytesTransferred -> GetQueuedCompletionStatus()으로 전달될 두번째 인자(송수신된 바이트수)
		// 세번째 인자 : _In_ ULONG_PTR dwCompletionKey -> GetQueuedCompletionStatus()으로 전달될 세번째 인자(비동기 I/O 요청이 발생한 디바이스의 completion Key)
		// 마지막 인자 : _In_opt_ LPOVERLAPPED lpOverlapped -> GetQueuedCompletionStatus()으로 전달될 네번째 인자(비동기 호출시 전달할 Overlapped 구조체의 주소)
		// 리턴 : 0(실패)
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

