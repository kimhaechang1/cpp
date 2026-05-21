# [Module 13] Hybrid Architecture: Selector + Thread Pool 연동 설계

지금까지 우리는 두 개의 독립된 강력한 시스템을 만들었습니다.
1. **Selector (`select`)**: 다수의 소켓에서 I/O 이벤트(읽기/쓰기/연결)가 발생하는지 감시하는 **"관제탑"**.
2. **Thread Pool**: 큐에 쌓인 일거리(Task)를 여러 일꾼(Thread)이 병렬로 빠르게 처리하는 **"공장"**.

이제 진짜 게임 서버를 만들기 위해 이 둘을 결합해야 합니다. 관제탑에서 발견한 이벤트를 공장으로 어떻게 넘겨주어야 할까요?

---

## 1. 왜 두 시스템을 결합해야 하는가?

만약 우리가 `select()` 만을 사용한 단일 스레드 서버(`practice_select_server.cpp`)를 그대로 게임 서버로 사용한다면 끔찍한 일이 발생합니다.

*   **상황**: A 유저가 던전 입장 버튼을 눌러 패킷을 보냄. B 유저는 이동 패킷을 보냄.
*   **단일 스레드의 한계**: `select()`가 A 유저의 패킷을 감지하고 `recv()`로 읽습니다. 그리고 DB에서 던전 정보를 불러오는 작업(매우 느림, 0.5초 소요)을 시작합니다.
*   **문제 발생**: A 유저의 DB 조회를 처리하는 0.5초 동안, **서버의 유일한 스레드가 멈춰 있습니다.** 따라서 `select()`는 다시 호출되지 못하고, B 유저의 이동 패킷은 0.5초 동안 서버의 수신 버퍼(OS)에 방치됩니다. B 유저 입장에서는 **"서버 렉(Lag)"**이 걸린 것입니다.

**💡 해결책**: 무거운 비즈니스 로직(DB 조회, 길찾기, 데미지 계산)은 **Thread Pool(공장)**에 던져버리고, 메인 스레드는 즉시 `select()` 관제탑으로 복귀하여 다음 I/O 이벤트를 감시하게 해야 합니다.

---

## 2. 결합 전략: Half-Sync / Half-Async 패턴

서버 아키텍처에서 가장 널리 쓰이는 패턴 중 하나입니다.
비동기적인 I/O 처리(Async)와 동기적인 로직 처리(Sync)를 반반 나누어 담당하는 구조입니다.

*   **Async (비동기 계층) = Main Thread**: 데이터가 언제 올지 모르는 네트워크 I/O를 담당합니다. `select()`를 이용해 데이터가 준비된(이벤트가 발생한) 소켓만 골라서 비동기적으로(이벤트 기반으로) 처리하므로 블로킹되지 않습니다.
    *   *(🚨 면접 단골 질문!)*: OS 레벨에서 `select()` 자체는 이벤트를 기다리며 멈추는 **동기(Synchronous) 함수**가 맞습니다. 진정한 비동기 I/O는 윈도우의 IOCP 같은 것입니다. 하지만 아키텍처 패턴에서 이를 "Async Layer"라고 부르는 이유는, 특정 유저의 `recv` 결과에 종속되지 않고 **"이벤트가 발생할 때만 즉각 반응하는 형태(Event-Driven)"**이기 때문입니다.
*   **Sync (동기 계층) = Worker Threads (Thread Pool)**: 큐에 들어온 게임 로직(Task)을 처음부터 끝까지 순차적(절차적)으로 실행합니다. 만약 DB 저장에 0.5초가 걸린다면, 해당 워커 스레드는 0.5초 동안 그 자리에 멈춰서(블로킹되어) 결과를 기다리는 **동기적(Synchronous)**인 방식으로 코드가 흘러갑니다.

### 🏭 역할 분담 (Role Delegation)

#### 🚦 Main Thread (I/O 감시자)
*   오직 **`select()` 관찰**과 **네트워크 I/O(`accept`, `recv`)**만 담당합니다.
*   `recv()`를 통해 수신한 바이트 덩어리를 온전한 "패킷(Packet)"으로 조립합니다.
*   패킷이 완성되면, **"누가(SessionID), 어떤 데이터(Packet)를 보냈는지"**를 포장하여 **Thread Pool의 Task Queue**에 집어넣습니다(Push).
*   이후 즉시 다음 루프의 `select()`로 돌아갑니다.

#### 👷 Worker Threads (Thread Pool, 비즈니스 로직 처리자)
*   큐에 일이 들어오기를 기다리다가(`std::condition_variable`), 일이 들어오면 깨어납니다.
*   큐에서 `[SessionID, Packet]`을 꺼냅니다.
*   패킷의 헤더(명령어)를 분석하여 해당 게임 로직(예: 공격 연산, HP 차감)을 수행합니다.
*   결과를 클라이언트에게 전송(`send`)합니다.

---

## 3. 구조적 결합 시 발생하는 "핵심 딜레마 (Dilemmas)"

이 구조를 구현하려고 하면 곧바로 몇 가지 치명적인 딜레마에 부딪히게 됩니다. 실무 면접에서도 단골로 등장하는 주제들입니다.

### 🤔 딜레마 1: `recv()`는 누가 호출해야 하는가?
*   **옵션 A (메인 스레드가 `recv`까지 다 하고 패킷만 넘김)**: 가장 안전합니다. 워커 스레드들은 소켓을 직접 만질 필요가 없습니다. 하지만 메인 스레드가 `recv`를 하느라 시간을 조금 뺏깁니다.
*   **옵션 B (메인 스레드는 `select`만 하고, "N번 소켓 읽을 타이밍이야!" 라고 소켓 번호만 큐에 넘김)**: 메인 스레드는 I/O조차 안 하므로 가장 빠릅니다. **하지만 치명적인 문제가 있습니다.** 
    *   워커 스레드 1번이 N번 소켓을 `recv` 하러 간 사이, 메인 스레드의 다음 `select` 루프가 돌면 어떻게 될까요? OS 버퍼에 아직 데이터가 남아있다면 `select`는 또 "N번 소켓 읽어라!" 라고 알려줍니다.
    *   결과적으로 워커 스레드 2번, 3번이 동시에 N번 소켓을 `recv` 하려고 덤벼드는 **Data Race**가 발생하여 패킷이 갈기갈기 찢어집니다.
*   **✅ 우리의 선택**: 입문자에게 가장 안전하고 논리적인 **옵션 A (메인 스레드가 패킷 조립까지 완료 후 전달)** 방식으로 구현할 것입니다.

### 🤔 딜레마 2: `send()`는 누가 호출해야 하는가?
*   워커 스레드가 로직 처리를 끝내고 **직접 `send()`를 호출**해도 될까요?
*   지금까지는 괜찮았지만, 만약 클라이언트의 수신 버퍼가 꽉 찼다면 어떻게 될까요? (어제 배운 `writefds` 상황)
*   워커 스레드가 직접 블로킹(Blocking) 소켓에 `send()`를 했다가 OS 송신 버퍼가 꽉 차서 **워커 스레드 자체가 Block** 되어버릴 수 있습니다. (일꾼이 마비됨!)
*   **해결책**:
    1. 소켓을 Non-blocking 모드로 바꾼다.
    2. 워커 스레드가 `send()`를 시도했을 때 `WSAEWOULDBLOCK`이 뜨면, 전송을 포기하고 "이 데이터를 나중에 대신 보내줘" 라고 **세션(Session)의 SendQueue에 저장**한다.
    3. 메인 스레드의 관제탑(`select`)에 해당 소켓을 **`writefds`에 감시 등록**한다.
    4. 메인 스레드가 쓰기 가능을 감지하면 그때 남은 데이터를 전송한다.
*   *이 부분은 매우 고난도이므로, 이번 미니 실습에서는 워커 스레드가 직접 `send()` 하는 방식으로 일단 출발하여 문제점을 눈으로 확인해볼 예정입니다.*

### 🤔 딜레마 3: State 공유 (동기화)
*   워커 스레드 1번: A 유저가 B 유저를 때림 (B의 HP 감소 로직 수행)
*   워커 스레드 2번: B 유저가 포션을 먹음 (B의 HP 증가 로직 수행)
*   동시에 하나의 데이터(B의 HP)에 접근합니다! M11에서 배웠던 **Data Race**가 발생합니다.
*   어떻게 해결해야 할까요? 게임 월드의 자물쇠(`std::mutex`) 설계가 필요해집니다.

---

## 4. 핵심 설계 패턴: `fd_set` Master-Replica 구조

초기 실습에서는 `std::vector`에 소켓을 담고 매 루프마다 `for`문을 돌려 `FD_SET`을 호출했지만, 클라이언트가 수만 명으로 늘어나면 이는 성능 저하(O(N))를 유발합니다. 
이를 최적화하기 위해 구조체 자체를 복사하는 **Master-Replica (원본-사본) 패턴**을 사용합니다.

```cpp
fd_set masterSet; // [Master]: 출석부 원본 (수정 전용)
FD_ZERO(&masterSet);
FD_SET(serverSocket, &masterSet); // 초기 세팅

while(true) {
    // [Replica]: 사본 생성 (조회용)
    // C++ 구조체의 얕은 복사(Shallow Copy)로 단 한 번의 대입 연산만에 전체 소켓 배열 복사 완료! (for문 불필요)
    fd_set copySet = masterSet; 
    
    // select()는 자신이 넘겨받은 사본(copySet)을 이벤트가 발생한 소켓만 남기고 다 지워버립니다(파괴적 동작).
    // 하지만 우리는 다음 루프 때 masterSet에서 온전한 사본을 다시 떠올 것이므로 끄떡없습니다.
    int activeCount = select(0, &copySet, nullptr, nullptr, &timeout);
    
    if (activeCount > 0) {
        if (FD_ISSET(serverSocket, &copySet)) {
            SOCKET clientSocket = accept(...);
            // 새 유저 접속 시 Master 출석부에 추가(Write)!
            FD_SET(clientSocket, &masterSet); 
        }
        // ... (연결 종료 시 FD_CLR(activeSocket, &masterSet);)
    }
}
```

---

## 5. 심화 아키텍처: `writefds` 기반 비동기 Send 처리

기본 하이브리드 실습에서는 워커 스레드가 게임 로직 처리 직후 소켓에 직접 `send()`를 호출했습니다. 하지만 상대방의 수신 버퍼가 꽉 차서 OS 송신 버퍼에 공간이 없다면, **해당 워커 스레드가 블로킹되어 공장이 멈출 위험**이 있습니다.
진정한 하이브리드 아키텍처에서는 **워커 스레드가 직접 전송하지 않고, 메인 스레드에게 "이거 OS 버퍼 남을 때 대신 좀 보내줘" 라고 위임**합니다.

```cpp
// ----------------------------------------------------
// [동기 계층: 워커 스레드]
// ----------------------------------------------------
void ProcessGameLogic(SOCKET clientSocket, std::string packet) {
    std::string response = "[DB Saved] " + packet;
    
    // 직접 send() 하지 않습니다! 해당 유저의 개인 SendQueue에 담아만 둡니다.
    SessionManager::GetInstance()->PushToSendQueue(clientSocket, response);
    
    // 메인 스레드의 관제탑(writeMasterSet)에 해당 소켓을 추가해달라고 요청합니다.
    SessionManager::GetInstance()->RequireWriteEvent(clientSocket);
}
```

```cpp
// ----------------------------------------------------
// [비동기 계층: 메인 스레드]
// ----------------------------------------------------
fd_set readMasterSet;  // 수신 감시 원본
fd_set writeMasterSet; // 송신 감시 원본 (보낼 데이터가 생긴 소켓만 추가됨)

while(true) {
    fd_set readCopy = readMasterSet;
    fd_set writeCopy = writeMasterSet; 

    // Read와 Write를 동시에 감시합니다!
    select(0, &readCopy, &writeCopy, nullptr, &timeout);

    // 1. 송신 가능 이벤트 처리 (OS 송신 버퍼에 드디어 빈 공간이 생김!)
    for (u_int i = 0; i < writeCopy.fd_count; ++i) {
        SOCKET sock = writeCopy.fd_array[i];
        
        // 해당 유저의 SendQueue에서 데이터를 꺼내와서 안전하게 전송
        std::string data = SessionManager::GetInstance()->PopFromSendQueue(sock);
        send(sock, data.c_str(), data.length(), 0);
        
        // [중요] 남은 보낼 데이터가 없다면 반드시 Master에서 즉각 제거해야 합니다!
        // 제거하지 않으면 OS 송신 버퍼가 비어있을 때마다 select()가 계속 True를 반환하여, 
        // 무의미한 메인 스레드 무한 루프(CPU 100% 점유 현상)가 발생합니다.
        if (SessionManager::GetInstance()->IsSendQueueEmpty(sock)) {
            FD_CLR(sock, &writeMasterSet);
        }
    }
    
    // 2. 수신 이벤트(readCopy) 처리 로직... (이하 생략)
}
```

이처럼 `readfds`와 `writefds`를 동시에 활용하면, 워커 스레드는 소켓 전송 블로킹의 위험에서 완전히 해방되어 오직 **CPU 연산(순수 게임 로직)**에만 100% 집중할 수 있습니다! 이것이 바로 현대 게임 서버의 근간이 되는 아키텍처입니다.

---

## 6. 심화 Q&A: 왜 유저(Socket)별로 "개인 Send Queue"를 가져야 하는가?

**Q. 워커 스레드가 직접 `send()` 하지 않고, 왜 굳이 '유저별 큐'를 만들어서 메인 스레드가 보내게 할까요?**

1. **느린 클라이언트로부터 서버 보호 (전송 속도 불균형)**
   - A 유저는 기가인터넷(빠름), B 유저는 3G(느림) 환경일 때, 워커 스레드가 B에게 직접 `send`를 시도하다 OS 버퍼가 꽉 차면 워커 스레드 전체가 블로킹(마비)됩니다. B의 개인 큐에 담아두고 메인 스레드가 여유될 때 보내게 하면, 워커 스레드는 즉시 다음 작업을 하러 떠날 수 있습니다.
2. **패킷 찢어짐(Interleaving) 방지**
   - 워커 1번이 C 유저에게 "공격 패킷(100바이트)"을 보내고, 동시에 워커 2번이 C 유저에게 "이동 패킷(100바이트)"을 직접 `send()` 하면, TCP 스트림 상에서 두 패킷이 50바이트씩 뒤섞여 찢어질 위험이 있습니다. Thread-Safe한 큐에 순서대로 쌓은 뒤 메인 스레드 혼자서 큐에서 꺼내 보내면 절대 섞이지 않습니다.
3. **병목(Lock Contention) 최소화**
   - 전체 유저 통합 큐 1개만 쓰면 모든 스레드가 큐에 접근할 때마다 글로벌 Lock이 걸려 병목이 심해집니다. 유저별로 개인 큐를 분리하면 서로 간섭 없이 완벽한 병렬 처리가 가능해집니다.

---

## 7. 주의사항: 멀티스레드와 공유 자원 (Data Race 방어)

하이브리드 아키텍처에서 가장 주의해야 할 점은 **"여러 워커 스레드가 동시에 같은 데이터(공유 자원)를 건드릴 때"**입니다. 
예를 들어, 워커 스레드 1번은 보스의 HP를 깎고 있고, 동시에 워커 스레드 2번은 보스의 HP를 회복시키려 한다면 Data Race가 발생합니다. 반드시 `std::mutex`를 통해 자물쇠를 채워야 합니다.

```cpp
#include <mutex>

class BossMonster {
private:
    int hp = 10000;
    std::mutex hpLock; // 보스 전용 자물쇠

public:
    void TakeDamage(int damage) {
        // [중요] 이 구간은 한 번에 하나의 워커 스레드만 들어올 수 있습니다!
        std::unique_lock<std::mutex> lock(hpLock); 
        
        hp -= damage;
        if (hp < 0) hp = 0;
        
        // 함수가 끝나며 lock이 소멸될 때 자동으로 자물쇠가 풀립니다. (RAII)
    }
};
```
이처럼 비즈니스 로직(워커 스레드 실행 공간) 내부에서는 언제든 다른 스레드와의 충돌을 예상하고, 최소 단위의 락(Lock)을 걸어 데이터를 보호해야 합니다.
