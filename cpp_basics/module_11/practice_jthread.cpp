#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

// ==============================================================================
// [요구사항] 우아한 중단 (Cooperative Cancellation) 실습
// 역할: 백그라운드에서 용량이 큰 파일을 다운로드하다가, 사용자가 취소를 누르면
// 파일 저장을 중단합니다. 조건:
// 1. `std::jthread` 1개를 생성하여 아래의 `DownloadFile` 작업을 실행하세요.
// 2. 메인 스레드는 `std::this_thread::sleep_for(500ms);` 로 잠시 기다려 줍니다.
// 3. 500ms 뒤, 사용자가 '다운로드 취소'를 눌렀다고 가정하고 jthread에 '중단
// 신호'를 보냅니다.
// ==============================================================================

// [빈칸 1] 이 함수가 외부의 '중단 깃발'을 볼 수 있도록 알맞은 타입의 매개변수를
// 선언하세요.
void DownloadFile(std::stop_token stoken)
{
    int percentage = 0;
    while (percentage <= 100) {
        // [빈칸 2] 매번 진짜 다운로드를 시작하기 직전에,
        // 외부에서 깃발이 올라왔는지 확인하고 "다운로드 사용자 취소됨." 출력 후
        // 즉시 함수를 안전하게 리턴하세요.
        /* 여기에 if 검사 로직 작성 */
        if (stoken.stop_requested()) {
            if (stoken.stop_possible()) {
                return;
            }
        }

        std::cout << "다운로드 중... " << percentage << "%\n";
        percentage += 20;

        // 무거운 다운로드 작업을 시뮬레이션
        std::this_thread::sleep_for(200ms);
    }

    if (percentage >= 100) {
        std::cout << "✅ 다운로드 100% 완료!\n";
    }
}

int main()
{
    std::cout << "--- Module 11: std::jthread & stop_token ---\n";
    std::cout << "다운로드 시작...\n";

    // 1. 위 함수를 실행하는 jthread 를 하나 생성하세요.
    std::jthread thread(DownloadFile);

    // 2. 메인 흐름 대기 (사용자가 마음이 바뀌기 전 500ms 대기)
    std::this_thread::sleep_for(500ms);

    // 3. 깃발 흔들기 (스레드 중단 원격 요청)
    thread.request_stop();

    // 4. (중요) jthread의 기적을 확인하세요.
    // 예전 std::thread 처럼 수동으로 join()을 안 적어도 터지지 않고 알아서
    // 기다렸다 끝납니다!

    return 0;
}
