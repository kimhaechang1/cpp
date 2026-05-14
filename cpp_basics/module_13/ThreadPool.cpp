#include "ThreadPool.h"
#include <mutex>

// 만약 ThreadPool.h 내에 클래스 선언만 하고 구현부를 분리하고 싶다면 여기에 작성하세요.
// 템플릿(TDD) 목적상 헤더에 모두 작성해도 무방합니다.

void ThreadPool::EnqueueJob(std::function<void()> job)
{
    {
        std::unique_lock<std::mutex> unique_lock(this->queueLock);
        _jobQueue.push(job);
    }
    cv.notify_one();
}