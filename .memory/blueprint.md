# Practice Blueprint

## Meta
- **Type**: cumulative_practice
- **Module**: 13
- **Topic**: Hybrid Architecture Server (Thread Pool + select() + SessionManager + writefds SendQueue)
- **Persona**: basic_tutor
- **TDD Mode**: gtest (CMake 프로젝트, M13 기존 CMakeLists.txt 확장)

## Integrated Concepts (통합 출제 맵)

> **통합 출제 룰**: 코어(M13 하이브리드 아키텍처) + M1~M12 각 모듈에서 1개씩 무작위 추출

- **[Core - M13] Half-Sync/Half-Async 하이브리드 아키텍처**: 
  - `select()` 기반 메인 스레드(Async Layer)가 I/O를 감시하고, `ThreadPool`(Sync Layer)에 작업을 위임하는 구조.
  - `SessionManager` 클래스가 유저별 SendQueue를 관리하며, writefds를 통한 비동기 전송을 지원.
  - Master-Replica fd_set 패턴 적용.

- **[M1] struct + Uniform Initialization**: 
  - 서버에서 사용하는 패킷 구조체(`CommandPacket`)를 `struct`로 정의. Uniform Initialization(`{}`) 사용 강제.
  - 시나리오 역할: 클라이언트가 보낸 원시 바이트를 의미 있는 명령 구조체로 파싱.

- **[M2] switch 분기**: 
  - 수신된 패킷의 명령 코드(CommandType)에 따라 서버 로직을 분기하는 핸들러 라우터.
  - 시나리오 역할: `ProcessCommand()` 내부에서 공격/회복/상태조회를 switch로 분기.

- **[M3] 함수 분리 + Call by Reference**: 
  - 비즈니스 로직을 독립 함수로 분리. 게임 상태를 참조(`&`)로 받아 원본을 수정.
  - 시나리오 역할: `HandleAttack()`, `HandleHeal()` 등이 공유 게임 상태를 참조로 수정.

- **[M4] const 참조 (const T&)**: 
  - 패킷 데이터를 함수에 넘길 때 불필요한 복사를 방지하며 원본 변경을 금지.
  - 시나리오 역할: `ProcessCommand(const CommandPacket& cmd, ...)` 형태의 읽기 전용 입력.

- **[M5] 동적 할당 (new/delete 우회 -> 스마트 포인터)**: 
  - Session 객체는 힙에 생성되며 스마트 포인터로 수명 관리.
  - 시나리오 역할: SessionManager가 접속한 유저를 힙에 생성하여 관리.

- **[M6] 클래스 설계 + 캡슐화 + static**: 
  - `SessionManager` 클래스: private 멤버로 세션 맵과 뮤텍스를 보유. static 싱글톤 접근자.
  - 시나리오 역할: 전역적으로 유일한 세션 관리자로서 모든 스레드에서 접근.

- **[M7] 다형성 (virtual + override + 추상 클래스)**: 
  - `ICommandHandler` 추상 인터페이스를 정의하고, `AttackHandler`, `HealHandler`, `StatusHandler`가 이를 구현.
  - 시나리오 역할: 명령 타입에 따라 적절한 핸들러를 다형적으로 호출. 가상 소멸자 필수.

- **[M8] std::unordered_map + Lambda**: 
  - SessionManager가 SOCKET을 Key로, Session 포인터를 Value로 관리하는 해시맵.
  - Lambda를 활용하여 ThreadPool에 작업을 캡처하여 전달.
  - 시나리오 역할: O(1) 속도로 소켓 번호 기반 세션 검색.

- **[M9] std::optional + Move Semantics**: 
  - SendQueue에서 데이터를 꺼낼 때, 큐가 비어있으면 "값이 없음"을 안전하게 표현.
  - 세션 검색 결과가 없을 수 있으므로 optional로 반환.
  - 시나리오 역할: `PopFromSendQueue()` -> `std::optional<std::string>`, 세션의 소유권 이동에 move 활용.

- **[M10] GTest + CMake 멀티파일 빌드**: 
  - GTest 기반 TDD. 헤더/소스 분리. CMakeLists.txt 수정.
  - 시나리오 역할: 서버를 실제 가동하지 않고도 SessionManager와 CommandHandler의 로직을 단위 테스트로 검증.

- **[M11] std::mutex + std::unique_lock (공유 자원 보호)**: 
  - 게임 월드의 공유 상태(보스 HP)에 여러 워커 스레드가 동시 접근할 때 Data Race 방지.
  - SessionManager 내부의 세션 맵 접근 시에도 뮤텍스 필요.
  - 시나리오 역할: `HandleAttack()`이 공유 보스 HP를 차감할 때 락 필수.

- **[M12] Winsock2 sockaddr_in + Endianness (htons/htonl)**: 
  - 세션(Session) 구조체가 접속한 클라이언트의 IP/Port 정보를 저장.
  - 시나리오 역할: 세션 생성 시 `sockaddr_in`에서 IP/Port를 추출하여 로깅 및 관리.

## Scenario (시나리오)

### "레이드 보스 토벌 서버 (Raid Boss Takedown Server)"

> 멀티플레이어 레이드 보스전 서버를 구축합니다.
> 여러 클라이언트(플레이어)가 TCP로 접속하여, 명령 패킷을 보내 공유 보스 몬스터를 함께 공격합니다.

**스토리 흐름:**

1. **서버 초기화**: 서버가 기동되면 `SessionManager`(싱글톤)와 `ThreadPool`(4 워커)이 생성됩니다. `BossMonster`(공유 자원, HP 10000)가 월드에 스폰됩니다.

2. **클라이언트 접속 (M12 sockaddr_in)**: 메인 스레드의 `select()`가 접속 이벤트를 감지하면, `accept()`으로 소켓을 얻고 `SessionManager`에 세션을 등록합니다. 세션은 클라이언트의 IP/Port 정보(M12), 플레이어 이름, 개인 SendQueue를 보유합니다.

3. **패킷 수신 및 파싱 (M1 struct, M4 const&)**: 메인 스레드가 `recv()`로 원시 바이트를 읽고, 이를 `CommandPacket` 구조체(M1)로 파싱합니다. 파싱된 패킷은 const 참조(M4)로 핸들러에 전달됩니다.

4. **ThreadPool 위임 (M8 Lambda)**: 메인 스레드는 파싱 완료 즉시, 패킷과 세션ID를 값 캡처한 Lambda(M8)를 `ThreadPool::EnqueueJob()`에 던지고 다음 `select()` 루프로 복귀합니다.

5. **비즈니스 로직 처리 (M2 switch, M3 함수분리, M7 다형성)**: 
   - 워커 스레드가 큐에서 작업을 꺼내 `ProcessCommand()`를 실행합니다.
   - 내부에서 `CommandType`에 따른 switch 분기(M2)로 적절한 `ICommandHandler`를 선택합니다.
   - `AttackHandler`: 보스 HP를 차감(M11 mutex 필수). 보스가 죽으면 결과를 반환.
   - `HealHandler`: 자기 자신(세션)의 HP를 회복. 최대치 초과 불가.
   - `StatusHandler`: 보스의 현재 HP와 본인 세션의 상태를 조회.
   - 각 핸들러는 `ICommandHandler`(M7 추상 클래스)를 상속받아 `Execute()`를 override합니다.

6. **응답 전송 (M9 optional, SendQueue)**: 
   - 핸들러가 반환한 응답 문자열을 해당 세션의 개인 SendQueue에 Push합니다.
   - `SessionManager`가 해당 소켓을 writeMasterSet에 등록 요청합니다.
   - 메인 스레드가 writefds로 전송 가능을 감지하면, `PopFromSendQueue()`(M9 optional)로 꺼내어 안전하게 전송합니다. 큐가 비면 writeMasterSet에서 즉각 제거(CPU 100% 방지).

7. **클라이언트 종료**: 메인 스레드가 `recv()` 반환값 0(연결 종료)을 감지하면 세션을 제거하고 소켓을 닫습니다.

## File Structure (파일 구조)

- `Session.h`: 세션 클래스 선언부. 학습자가 멤버 변수(소켓, IP/Port, 이름, HP, SendQueue)와 멤버 함수(큐 Push/Pop 등)를 설계.
- `SessionManager.h`: 세션 관리자 선언부. 싱글톤 접근자, 세션 등록/제거/검색, writefds 등록 요청 인터페이스.
- `SessionManager.cpp`: 세션 관리자 구현부. **학습자가 구현**.
- `CommandHandler.h`: `ICommandHandler` 추상 인터페이스 + 각 구체 핸들러 선언.
- `CommandHandler.cpp`: 각 핸들러의 `Execute()` 구현. **학습자가 구현**.
- `BossMonster.h`: 공유 보스 자원. HP, 뮤텍스, TakeDamage/GetHp 등.
- `BossMonster.cpp`: 보스 로직 구현. **학습자가 구현**.
- `test_raid_server.cpp`: GTest 테스트 파일 (서버 가동 없이 로직만 단위 테스트).
- `CMakeLists.txt`: 기존에 테스트 타겟 추가.
- (선택) `raid_server_main.cpp`: 실제 서버 실행용 main (테스트 통과 후 보너스).

## Test Cases (테스트 케이스 설계)

> **설계 원칙**: 서버를 실제 가동(소켓 바인딩)하지 않고, SessionManager/CommandHandler/BossMonster의 **비즈니스 로직만 격리 테스트**합니다.

### Suite 1: SessionManagerTest (세션 관리)

1. **TEST(SessionManagerTest, RegisterAndFindSession)**
   - 목적: 세션 등록 후 소켓 번호로 검색하면 해당 세션을 찾을 수 있는지 검증.
   - 기대 동작: `RegisterSession(fakeSocket, "PlayerA", ip, port)` 후 `FindSession(fakeSocket)`이 유효한 세션을 반환.

2. **TEST(SessionManagerTest, FindNonExistentSessionReturnsEmpty)**
   - 목적: 존재하지 않는 소켓으로 검색 시 안전하게 "값 없음"을 반환하는지 검증 (M9 optional).
   - 기대 동작: `FindSession(9999)`가 "비어있는 안전상자"를 반환.

3. **TEST(SessionManagerTest, RemoveSession)**
   - 목적: 세션 제거 후 더 이상 검색되지 않는지 검증.
   - 기대 동작: `RemoveSession(fakeSocket)` 후 `FindSession(fakeSocket)`이 비어있음.

4. **TEST(SessionManagerTest, SendQueuePushAndPop)**
   - 목적: 특정 세션의 개인 SendQueue에 데이터를 Push하고 Pop하는 FIFO 동작 검증.
   - 기대 동작: Push("Hello") -> Push("World") -> Pop() == "Hello" -> Pop() == "World" -> Pop()은 "값 없음".

5. **TEST(SessionManagerTest, SendQueueEmptyCheck)**
   - 목적: SendQueue가 비어있는지 확인하는 기능 검증 (writefds 가지치기 판단용).
   - 기대 동작: 초기 상태에서 `IsSendQueueEmpty(sock)` == true. Push 후 false. 전부 Pop 후 다시 true.

### Suite 2: BossMonsterTest (공유 자원 보호)

6. **TEST(BossMonsterTest, TakeDamageReducesHp)**
   - 목적: 보스가 데미지를 받으면 HP가 정확히 차감되는지 검증.
   - 기대 동작: HP 10000 -> TakeDamage(300) -> GetHp() == 9700.

7. **TEST(BossMonsterTest, HpNeverGoesBelowZero)**
   - 목적: HP가 0 이하로 내려가지 않는 하한(floor) 검증.
   - 기대 동작: HP 100 -> TakeDamage(999) -> GetHp() == 0.

8. **TEST(BossMonsterTest, IsDeadAfterLethalDamage)**
   - 목적: HP가 0이 되면 사망 상태를 반환하는지 검증.
   - 기대 동작: TakeDamage(10000) -> IsDead() == true.

9. **TEST(BossMonsterTest, ConcurrentDamageIsSafe)**
   - 목적: 여러 스레드가 동시에 TakeDamage를 호출해도 Data Race 없이 정확한 HP를 유지하는지 검증 (M11 mutex).
   - 기대 동작: 10개 스레드가 각각 TakeDamage(100)을 호출 -> 최종 HP == 10000 - (10 * 100) == 9000. (단, 보스 사망 안 하는 범위)

### Suite 3: CommandHandlerTest (다형적 명령 처리)

10. **TEST(CommandHandlerTest, AttackHandlerReducesBossHp)**
    - 목적: AttackHandler가 보스 HP를 올바르게 차감하고, 응답 문자열을 반환하는지 검증.
    - 기대 동작: Execute() 호출 후 보스 HP 감소 확인 + 반환된 응답에 "damage" 또는 차감량 정보가 포함.

11. **TEST(CommandHandlerTest, HealHandlerRestoresSessionHp)**
    - 목적: HealHandler가 세션(플레이어)의 HP를 회복시키되 최대치를 초과하지 않는지 검증.
    - 기대 동작: 플레이어 HP 70/100 -> Heal(50) -> HP == 100 (캡 적용).

12. **TEST(CommandHandlerTest, StatusHandlerReturnsBossAndPlayerInfo)**
    - 목적: StatusHandler가 보스 HP + 본인 HP를 포함한 상태 문자열을 반환하는지 검증.
    - 기대 동작: 반환 문자열에 보스 현재 HP 값과 플레이어 이름이 포함.

### Suite 4: IntegrationFlowTest (E2E 시나리오 흐름)

13. **TEST(IntegrationFlowTest, FullRaidScenario)**
    - 목적: 세션 등록 -> 공격 -> 회복 -> 상태 조회 -> 보스 처치 -> 세션 제거까지 전체 시나리오가 유기적으로 동작하는지 검증.
    - 기대 동작:
      1. SessionManager에 2명의 플레이어(A, B) 등록
      2. A가 AttackHandler로 보스 공격 -> 보스 HP 감소 + A의 SendQueue에 응답 Push
      3. B가 HealHandler로 자신 회복 -> B의 HP 증가 + B의 SendQueue에 응답 Push
      4. A가 StatusHandler로 상태 조회 -> 현재 보스 HP와 A의 HP가 포함된 응답
      5. 반복 공격으로 보스 처치 -> IsDead() == true
      6. 두 플레이어의 SendQueue를 Pop하여 모든 응답이 정상적으로 들어있는지 확인
      7. RemoveSession으로 정리 -> FindSession이 비어있음 확인

## Comment Guide (주석 가이드 - Blank Slate Policy)

> 아래의 암시적 문구를 주석에 사용합니다. 정답 키워드를 절대 노출하지 않습니다.

### Session.h
- 멤버 변수 (소켓): "이 세션이 통신하는 대상의 OS 핸들(식별자)을 보관합니다."
- 멤버 변수 (IP/Port): "접속 시 상대방이 알려준 네트워크 주소 정보를 기억합니다."
- 멤버 변수 (이름): "플레이어의 닉네임을 보관합니다."
- 멤버 변수 (HP): "플레이어의 현재 생존 수치를 보관합니다."
- 멤버 변수 (SendQueue): "이 유저에게 전송할 응답들을 선입선출(FIFO) 순서로 줄세우는 대기열입니다."
- 멤버 변수 (Lock): "여러 스레드가 동시에 이 대기열에 접근할 때 충돌을 방지할 자물쇠입니다."
- PushToSendQueue: "대기열의 맨 뒤에 응답 데이터를 안전하게 추가합니다."
- PopFromSendQueue: "대기열의 맨 앞에서 데이터를 꺼냅니다. 대기열이 비어있을 수도 있으므로, '값이 있을 수도 비어있을 수도 있는' 안전상자로 포장해 반환하세요."
- IsSendQueueEmpty: "대기열이 비어있는지 확인합니다. (writefds 가지치기 판단에 사용)"

### SessionManager.h
- 클래스 설명: "수많은 인스턴스가 생성되더라도 메모리 상에 오직 단 하나만 존재하여 전역적으로 공유되어야 합니다."
- 세션 저장소: "정렬은 무시하고 특정 Key(소켓 번호)를 통해 극강의 속도로 Value(세션) 원본을 알아내야 합니다."
- RegisterSession: "새 접속자의 세션을 저장소에 등록합니다. 세션 객체는 스택 수명 스코프를 벗어나는 즉시 delete를 스스로 뱉어내는 똑똑한 독점 메모리 래퍼로 관리하세요."
- FindSession: "소켓 번호로 세션을 검색합니다. 해당 소켓이 존재하지 않을 수 있으므로, '값이 있을 수도 비어있을 수도 있는' 안전상자로 포장해 반환하세요. 단, 원본의 소유권을 넘기면 안 되고 원본의 주소만 대여하세요."
- RemoveSession: "소켓 번호에 해당하는 세션을 저장소에서 완전히 제거합니다."

### CommandHandler.h
- ICommandHandler: "이 클래스는 직접 인스턴스화가 불가능한 설계도(인터페이스)입니다. 파생 클래스가 반드시 구현해야 할 순수한 실행 메서드를 선언합니다."
- Execute: "파생 클래스마다 고유한 게임 로직을 수행하고, 클라이언트에게 보낼 응답 문자열을 반환합니다."
- AttackHandler: "보스 몬스터의 생존 수치를 차감하는 공격 로직을 수행합니다. 여러 일꾼(스레드)이 동시에 접근할 수 있으므로 자물쇠가 필수적입니다."
- HealHandler: "자신(세션)의 생존 수치를 회복시킵니다. 최대치를 넘을 수 없습니다."
- StatusHandler: "보스와 자신의 현재 상태를 조회하여 보고서 문자열을 조립합니다."

### BossMonster.h
- HP: "무결성을 위해 외부 접근을 금지하고 객체 자체적으로만 통제합니다."
- Lock: "여러 스레드가 동시에 이 보스를 때릴 수 있으므로, 생존 수치 접근 시 충돌 방지 자물쇠가 필수입니다."
- TakeDamage: "데미지만큼 생존 수치를 차감합니다. 0 미만으로 떨어지지 않습니다."
- GetHp: "현재 생존 수치를 안전하게 반환합니다."
- IsDead: "생존 수치가 0인지 확인합니다."

## Constraints (제약 사항)

1. **정답 코드 금지**: 학습자가 구현할 함수의 Body를 Blueprint에 절대 작성하지 않음.
2. **함수 원형 금지**: 반환 타입/매개변수 타입은 주석으로 암시만 함 (Blank Slate Policy).
3. **네트워크 I/O 테스트 제외**: GTest에서는 실제 소켓을 열지 않음. SOCKET은 가짜 정수값(예: 100, 200)으로 대체하여 순수 로직만 검증.
4. **ThreadPool은 기존 것 재사용**: 학습자가 이미 구현한 `ThreadPool.h/cpp`를 그대로 링크. 수정 불필요.
5. **금지 키워드 (주석에 직접 노출 금지)**: `vector`, `unordered_map`, `unique_ptr`, `optional`, `mutex`, `virtual`, `override`, `queue`. 대신 Master Prompt Templates의 암시 문구를 사용.
6. **난이도 조절**: Session.h는 구조가 비교적 단순하므로 학습자가 가장 먼저 구현하도록 유도. CommandHandler는 다형성 설계가 핵심이므로 두 번째. SessionManager는 통합 관리자이므로 마지막에 도전.
7. **실행 가능 서버는 보너스**: 핵심 평가는 GTest 통과. 실제 서버 바이너리(`raid_server_main.cpp`)는 모든 테스트 통과 후 선택적으로 제공.

## Cohesion Checklist (설계 통합성 검증)

1. **데이터 흐름 검증**: 
   - ✅ SessionManager가 Session을 생성/검색/제거 → CommandHandler가 Session의 HP를 수정 → BossMonster가 공유 HP를 관리 → 응답이 Session의 SendQueue에 Push → 메인 스레드가 Pop하여 전송.
   - 모든 클래스가 최소 1회 이상 서로의 메서드를 호출하거나 데이터를 교환합니다.

2. **Enum 완전 소비**: 
   - ✅ CommandType enum에 Attack, Heal, Status 3개를 정의하면, 3개 모두 테스트(Suite 3)에서 검증됩니다.

3. **반환값 활용**: 
   - ✅ CommandHandler::Execute()의 반환값(응답 문자열)이 SendQueue에 Push되고, IntegrationFlowTest에서 Pop하여 내용을 검증합니다.
   - ✅ FindSession()의 optional 반환값이 존재/비존재 케이스 모두 테스트됩니다.
   - ✅ PopFromSendQueue()의 optional 반환값이 FIFO 순서 및 빈 큐 케이스에서 테스트됩니다.

4. **시나리오 일관성**: 
   - ✅ IntegrationFlowTest가 "세션 등록 → 공격 → 회복 → 상태 조회 → 보스 처치 → 정리"라는 하나의 레이드 스토리를 관통합니다.

5. **Cross-Module 연결**: 
   - ✅ 나쁜 예(각각 독립 테스트)가 아닌, "다형적 핸들러(M7)가 보스의 공유 HP를 mutex(M11) 보호 하에 차감하고, 그 결과를 세션(M5 힙, M8 맵)의 개인 SendQueue(M9 optional)에 Push하여, 메인 스레드가 writefds로 전송"하는 **유기적 통합** 구조.
