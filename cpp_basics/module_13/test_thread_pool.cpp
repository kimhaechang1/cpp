#include <gtest/gtest.h>
#include "ThreadPool.h"
#include <atomic>
#include <chrono>

// 1. 생성과 소멸이 안전하게 이루어지는지 테스트
TEST(ThreadPoolTest, InitializeAndDestruct) {
    // 4개의 스레드를 가지는 스레드 풀 생성
    ThreadPool pool(4);
    // 이 스코프를 벗어날 때 소멸자가 정상적으로 호출되어 행(Hang)이 안 걸리면 성공!
}

// 2. 여러 일거리를 큐에 넣었을 때 분산 처리가 되는지 테스트
TEST(ThreadPoolTest, EnqueueMultipleJobs) {
    std::atomic<int> counter{0};
    {
        ThreadPool pool(4);

        // 100개의 일을 큐에 넣기 (멀티스레드 환경이므로 atomic 사용)
        for(int i = 0; i < 100; ++i) {
            pool.EnqueueJob([&counter]() {
                // 인위적으로 일을 늦게 처리하게 만들어 스레드들이 겹쳐서 일하게 유도
                std::this_thread::sleep_for(std::chrono::milliseconds(2));
                counter++;
            });
        }
    } // <-- 여기서 pool의 소멸자가 호출되면서 큐에 남은 일들을 전부 끝낼 때까지 대기(join)함

    // 소멸자가 정상적으로 모든 작업을 기다려주었다면 100이어야 함!
    EXPECT_EQ(counter.load(), 100);
}
