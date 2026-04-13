#include <cassert>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

// ==============================================================================
// [요구사항 1] 안전한 중앙 은행 금고(Bank) 객체 설계
// 역할: MMORPG의 서버 1대에 단 1개만 존재하는 중앙 은행 금고입니다.
// 조건:
// 1. "무결성을 위해 데이터 외부 접근을 금지하고 객체 자체적으로만 통제"되는
// 잔고(정수) 변수를 가집니다.
// 2. 금고에 돈을 입금하는 행위와, 현재 잔고를 조회하는 행위를 만드세요. (이름,
// 반환 타입, 매개변수 직접 결정)
// 3. [핵심] 입금 행위는 수백 명의 유저(스레드)가 동시에 호출할 수 있습니다.
//    "데이터가 오염될 수 있으므로, 내 작업이 끝날 때까지 아무도 진입하지
//    못하도록 독점 보호막(자물쇠)을 쳐서 안전하게 보호하세요."
// ==============================================================================

// 여기에 객체를 설계하세요.
class Bank
{
    int money;
    std::mutex lock;

  public:
    Bank(int money_) : money(money_)
    {
    }

    void hunt()
    {
        std::lock_guard<std::mutex> guard(lock);
        this->money++;
    }

    int GetMoney()
    {
        return this->money;
    }
};

// ==============================================================================
// [테스트 영역]
// Blank Slate 정책에 따라 함수/클래스 이름이 자유로우므로,
// 아래 주석을 활용하여 직접 테스트 흐름을 완성하세요.
// ==============================================================================
void TestMutex()
{
    std::cout
        << "100명의 유저(스레드)가 동시에 10,000골드씩 입금을 시도합니다...\n";

    // 1. 위에서 만든 금고 객체를 하나 생성하세요.
    Bank bank(0);

    // 2. 100개의 스레드를 담을 std::vector<std::thread> 를 준비하세요.
    std::vector<std::thread> threadpool;

    // 3. 반복문을 100번 돌며 스레드를 생성하여 vector에 넣으세요.
    // 각 스레드는 사냥을 10,000번 반복하며 1골드씩 총 10,000 골드를 입금합니다.
    // (힌트: 람다와 & 캡처를 활용하면 편리합니다)
    for (int i = 0; i < 100; i++) {
        auto thread = std::thread([&] {
            for (int i = 0; i < 10000; i++) {
                bank.hunt();
            }
        });
        threadpool.push_back(std::move(thread));
    }

    for (auto &thread : threadpool) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    // 4. 메인 스레드가 100개의 스레드 작업이 '모두 끝날 때까지' 안전하게
    // 기다리도록 조치하세요.

    // 5. 금고의 최종 잔고를 가져와서 검증합니다.

    int finalBalance = bank.GetMoney(); // 작성하신 잔고 조회 기능 호출

    std::cout << "최종 잔고: " << finalBalance << " 골드\n";
    assert(finalBalance == 1000000 && "Race Condition 발생! 돈이증발했습니다!");
    std::cout << "[PASS] 100만 골드 입금 완료! Race Condition방어 성공!\n";
}

int main()
{
    std::cout << "--- Module 11: Race Condition & Mutex Practice ---\n";

    TestMutex(); // 구현이 완료되면 주석을 풀고 실행하세요.

    return 0;
}
