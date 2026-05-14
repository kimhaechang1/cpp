#pragma once
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

// [지시사항]
// 다음 요구사항을 만족하는 'ThreadPool' 클래스를 설계하세요.

// 1. 멤버 변수 (Private):
//    - "일꾼(스레드)들을 보관할 가변 길이 1차원 컨테이너"
//    - "일거리(void() 형태의 함수 객체)들을 줄 세울 자료구조"
//    - 큐 접근을 보호할 자물쇠
//    - 스레드들을 재우고 깨울 알람시계
//    - 풀 종료 여부를 나타내는 불리언 변수 (예: bStopPool)

// 2. 생성자 (Public):
//    - "생성될 스레드의 개수"를 인자로 받습니다.
//    - 해당 개수만큼 스레드를 생성하여 컨테이너에 넣습니다.
//    - 각 스레드는 내부에서 무한루프를 돌며, 자물쇠를 걸고 대기하다가 큐에서 일거리를 꺼내 수행해야 합니다.
//    - (Hint: cv.wait()와 조건식을 정확히 활용하세요!)

// 3. 소멸자 (Public):
//    - 자물쇠를 잠근 상태에서 bStopPool 플래그를 true로 변경합니다.
//    - "모든 자고 있는 스레드"를 깨워 퇴근시킵니다. (notify_...)
//    - 컨테이너 안의 모든 스레드에 대해 join()을 호출하여 안전하게 종료를 기다립니다.

// 4. 멤버 함수 'EnqueueJob' (Public):
//    - 반환값이 없고 인자도 없는 함수 객체(std::function)를 파라미터로 받습니다.
//    - 자물쇠를 걸고 해당 작업을 큐에 추가합니다.
//    - 잠자고 있는 스레드 "하나만" 깨워서 일을 하도록 알람을 울립니다.

// 여기에 ThreadPool 클래스를 작성하세요!
class ThreadPool
{
private:
    std::vector<std::thread> _workers;
    std::queue<std::function<void()>> _jobQueue;
    std::mutex queueLock;
    std::condition_variable cv;
    bool bStopPool = false;

public:
    ThreadPool(const int nThread)
    {
        for (int i = 0; i < nThread; i++)
        {
            _workers.push_back(std::thread([&]() {
                while (true)
                {
                    std::function<void()> job;
                    {
                        std::unique_lock<std::mutex> unique_lock(queueLock);
                        cv.wait(unique_lock, [&]() { return _jobQueue.size() > 0 || bStopPool; });
                        if (_jobQueue.size() == 0 && bStopPool)
                            return;
                        job = std::move(_jobQueue.front());
                        _jobQueue.pop();
                    }
                    job();
                }
            }));
        }
    }
    ~ThreadPool()
    {
        // 락을 걸고 bStopPool 을 하지않으면
        // 무수히 많은 스레드가 읽고있는 와중에 갑자기 쓰게되므로 작동이 이상해질 수 있다.
        // 그리고 락을 검으로서 메모리 베리어 효과를 얻어서 다른스레드가 다른 코어의 캐시메모리를 읽는것이 아닌 곧바로
        // 최신화가 이뤄지게 된다.
        {
            std::unique_lock<std::mutex> unique_lock(queueLock);
            bStopPool = true;
        }
        cv.notify_all();
        for (int i = 0; i < this->_workers.size(); i++)
        {
            if (_workers[i].joinable())
            {
                _workers[i].join();
            }
        }
    }
    void EnqueueJob(std::function<void()>);
};