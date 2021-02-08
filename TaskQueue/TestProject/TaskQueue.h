#pragma once
#include "queue"
#include "functional"
#include "iostream"
#include "windows.h"
#define NUM_OF_THREADS 5

class TaskQueue
{
public:
	std::queue<std::function<void()>> *taskQueue;
	HANDLE mutex;

	int threadNum;

	void Start();

	static DWORD WINAPI Process(LPVOID param);

	void WaitAll();

	HANDLE *threads;

	~TaskQueue();

	bool IsEmpty();

	void Push(std::function<void()> task);

	std::function<void()> Pop();

	TaskQueue(int threadNum = NUM_OF_THREADS);

	int getNumOfThreads() const;

	CRITICAL_SECTION criticalSection;
};

