#include "TaskQueue.h"

void TaskQueue::Start() {
    for (int i = 0; i < threadNum; i++) {
        threads[i] = CreateThread(NULL, 0, Process, this, 0, NULL);
    }
}

DWORD TaskQueue::Process(LPVOID param) {
    auto taskQueue = (TaskQueue*)param;
    do {
        std::function<void()> task;
        EnterCriticalSection(&taskQueue->criticalSection);
        bool wasEmpty = taskQueue->IsEmpty();
        if (!wasEmpty) {
            task = taskQueue->Pop();
        }
        LeaveCriticalSection(&taskQueue->criticalSection);
        if (!wasEmpty) {
            task();
        }
    } while (!taskQueue->IsEmpty());
    return 0;
}


void TaskQueue::WaitAll() {
    WaitForMultipleObjects(threadNum, threads, true, INFINITE);
}

TaskQueue::~TaskQueue() {
    DeleteCriticalSection(&criticalSection);
    CloseHandle(this->mutex);
    delete this->threads;
    delete this->taskQueue;
}

bool TaskQueue::IsEmpty() {
	return taskQueue->empty();
}

void TaskQueue::Push(std::function<void()> task) {
    DWORD dwWaitResult;
    while (true) {
        dwWaitResult = WaitForSingleObject(mutex, INFINITE);
        if (dwWaitResult == WAIT_OBJECT_0) {
            taskQueue->push(task);
            ReleaseMutex(mutex);
            return;
        }
    }
}

std::function<void()> TaskQueue::Pop() {
    DWORD dwWaitResult;
    while (true) {
        dwWaitResult = WaitForSingleObject(mutex, INFINITE);

        if (dwWaitResult == WAIT_OBJECT_0) {
            auto task = taskQueue->front();
            if (taskQueue->empty()) {
                task = nullptr;
            }
            else {
                task = taskQueue->front();
                taskQueue->pop();
            }
            ReleaseMutex(mutex);
            return task;
        }
    }
}

TaskQueue::TaskQueue(int numOfThreads) : threadNum(numOfThreads) {
    taskQueue = new std::queue<std::function<void()>>();
    mutex = CreateMutex(nullptr, false, nullptr);
    InitializeCriticalSection(&criticalSection);
    threads = new HANDLE[numOfThreads];
}

int TaskQueue::getNumOfThreads() const {
    return threadNum;
}