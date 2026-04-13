# Practice Blueprint

## Meta
- **Type**: cumulative_practice
- **Module**: 11
- **Topic**: Basic Concurrency (Thread & jthread) - M1~M10 통합
- **Persona**: basic_tutor
- **TDD Mode**: gtest (Multi-File CMake 프로젝트)

## Reference Files
> 생성 모델이 반드시 읽어서 기존 프로젝트 패턴을 참고해야 할 파일들입니다.

- `cpp_basics/module_10/CMakeLists.txt`: CMake 설정 패턴 참고 (GTest find_package, add_executable, target_link_libraries)
- `cpp_basics/module_10/test_quest_journal.cpp`: GTest 테스트 파일 작성 패턴 참고 (TEST 매크로, EXPECT/ASSERT 사용법, 안내 주석 스타일)
- `cpp_basics/module_10/QuestJournal.h`: 헤더 파일 작성 패턴 참고 (#pragma once, 암시적 주석+함수 선언 병기 스타일)
- `cpp_basics/module_10/Quest.h`: 추상 클래스 헤더 패턴 참고

## Integrated Concepts (통합 출제 맵)

- **[Core - M11]** `std::thread`, `std::mutex`, `std::lock_guard`, `std::jthread`, `std::stop_token`: 멀티스레드 작업 파견, 공유 자원 보호, RAII 기반 자동 조인, 협력적 중단
- **[M1]** `enum class` + `std::format`: 던전 층(FloorState) 상태 열거형 + 전투 보고서 포맷팅
- **[M2]** Range-based for + 흐름 제어: 스레드/몬스터 벡터 순회
- **[M3]** `const reference` 매개변수: 무거운 데이터를 복사 없이 전달
- **[M4]** `std::span`: 몬스터 목록의 소유권 없는 읽기 전용 뷰
- **[M5]** Stack vs Heap: 스레드별 독립 스택 vs 공유 힙 자원 인지
- **[M6]** `class` 캡슐화 + `static` 멤버 + 초기화 리스트: DungeonRaid 클래스, static 전역 처치 통계
- **[M7]** `virtual` + `override` + 추상 클래스 + 가상 소멸자: Monster 추상 클래스 → Goblin/Dragon 다형성
- **[M8]** `std::vector` + 람다: 몬스터 벡터 관리, 람다 기반 필터링
- **[M9]** `std::unique_ptr` + `std::optional`: 몬스터 소유권 관리, 보스 드롭 아이템
- **[M10]** GTest TDD: 모든 기능을 TEST() 매크로로 검증

## Scenario (시나리오)

### "무한 던전 탐험대 - 병렬 층 공략 시스템"

MMORPG 서버의 던전 탐험대 시스템. 던전의 각 층(Floor)을 여러 공격대(스레드)가 동시에 공략합니다.

**스토리라인:**
1. 던전에는 다양한 몬스터(Goblin/Dragon)가 배치된 층이 존재합니다.
2. 탐험대 본부(메인 스레드)는 각 층에 공격대(워커 스레드)를 파견합니다.
3. 각 공격대는 층의 몬스터를 처치하고, 결과를 중앙 전투 기록부(DungeonRaid, 공유 자원)에 안전하게 기록합니다.
4. 탐험대장은 긴급 상황 시 모든 공격대에게 철수 명령(stop_token)을 내릴 수 있습니다.
5. 공략 완료 후, 전투 보고서(std::format)를 출력합니다.

## File Structure (파일 구조)

### 학습자가 구현할 파일 (빈칸)
- `Monster.h`: Monster 추상 클래스 선언 (순수 가상 함수, 가상 소멸자, optional 드롭)
- `Monster.cpp`: Goblin/Dragon 파생 클래스의 메서드 구현
- `DungeonRaid.h`: DungeonRaid 클래스 선언 (mutex 보호, static 통계, format 보고서)
- `DungeonRaid.cpp`: DungeonRaid 클래스의 메서드 구현

### 에이전트가 100% 완성하는 파일
- `test_dungeon_raid.cpp`: GTest 테스트 파일 (빈칸 없이 완전 작성, 학습자의 목표 명세서)
- `CMakeLists.txt`: GTest 연동 빌드 설정

## Test Cases (테스트 케이스 설계)

### Suite 1: MonsterSuite (다형성 & 소유권)

1. **TEST(MonsterSuite, GoblinHpAndAttack)**
   - 목적: Goblin 인스턴스의 HP와 공격 이름 검증
   - 기대 동작:
     - `std::make_unique<Goblin>()` 로 생성
     - `GetHp()` == 50
     - `GetAttackName()` == "Slash"
     - 부모 포인터(`unique_ptr<Monster>`)로 호출해도 다형적으로 동작

2. **TEST(MonsterSuite, DragonHpAndAttack)**
   - 목적: Dragon 인스턴스의 HP와 공격 이름 검증
   - 기대 동작:
     - `GetHp()` == 500
     - `GetAttackName()` == "Fire Breath"

3. **TEST(MonsterSuite, BossDropOptional)**
   - 목적: 보스 드롭 아이템의 optional 반환 검증
   - 기대 동작:
     - Dragon(보스): `GetDropItem().has_value()` == true, 값 == "Dragon Heart"
     - Goblin(일반): `GetDropItem().has_value()` == false

4. **TEST(MonsterSuite, SpanMonsterView)**
   - 목적: std::span으로 몬스터 벡터의 읽기 전용 뷰 확인
   - 기대 동작:
     - `std::vector<std::unique_ptr<Monster>>`에 Goblin 2마리, Dragon 1마리 추가
     - `std::span` 으로 뷰를 빌려 순회하며 각 몬스터의 `GetAttackName()`이 올바르게 반환됨
     - span의 size()가 3

### Suite 2: DungeonRaidSuite (스레드 안전성 & 자원 보호)

5. **TEST(DungeonRaidSuite, SingleThreadRecord)**
   - 목적: 단일 스레드에서 기록 기능의 기본 동작 검증
   - 기대 동작:
     - DungeonRaid 객체 생성 (floorNumber = 1)
     - `RecordKill()` 5회 호출
     - `GetKillCount()` == 5
     - static `GetTotalGlobalKills()` >= 5

6. **TEST(DungeonRaidSuite, MultiThreadMutexProtection)**
   - 목적: 10개 스레드가 동시에 100회씩 기록 시 Race Condition 방어 검증
   - 기대 동작:
     - `DungeonRaid::ResetGlobalKills()` 호출 (이전 테스트의 static 값 초기화)
     - 10개의 `std::jthread`를 vector에 넣고, 각 스레드에서 `RecordKill()` 100번 호출
     - vector를 clear하여 모든 jthread 조인 대기
     - `GetKillCount()` == 1000
     - `GetTotalGlobalKills()` == 1000

7. **TEST(DungeonRaidSuite, FormatReport)**
   - 목적: std::format 기반 전투 보고서 문자열 검증
   - 기대 동작:
     - DungeonRaid(floorNumber=3) 생성, RecordKill() 42회 호출
     - `GetReport()` 반환 문자열이 "Floor 3"과 "42" 를 포함(string::find != npos)

8. **TEST(DungeonRaidSuite, FloorStateEnum)**
   - 목적: enum class FloorState의 존재 및 상태 전이 검증
   - 기대 동작:
     - DungeonRaid 초기 상태: `GetState()` == `FloorState::Idle`
     - RecordKill() 후: `GetState()` == `FloorState::InProgress`

### Suite 3: ConcurrencySuite (jthread & stop_token)

9. **TEST(ConcurrencySuite, JthreadAutoJoin)**
   - 목적: jthread가 스코프를 벗어날 때 자동 조인됨을 검증
   - 기대 동작:
     - `bool flag{false}` 선언
     - 별도 스코프(`{ }`) 안에서 `jthread` 생성, 내부에서 `flag = true` 설정
     - 스코프 탈출 후 `flag == true` 확인 (자동 조인이 되었으므로 결과가 반영됨)

10. **TEST(ConcurrencySuite, StopTokenEvacuation)**
    - 목적: stop_token을 통한 협력적 중단(긴급 철수) 검증
    - 기대 동작:
      - `DungeonRaid` 객체와 충분히 많은 몬스터 목록 준비
      - `jthread`로 `ExecuteRaid()` 함수를 실행 (이 함수는 stop_token을 첫 번째 인자로 받아야 함)
      - 메인 스레드에서 `std::this_thread::sleep_for(200ms)` 후 `worker.request_stop()` 호출
      - worker가 조인된 후, `GetKillCount()`가 0보다 크고 총 몬스터 수보다 작음 (중간에 멈췄으므로)

## Comment Guide (주석 가이드 - Blank Slate Policy)

### Monster.h 주석
- 클래스 상단: "모든 몬스터의 뼈대(골격)가 되는 최상위 설계도를 만드세요. 이 설계도 자체로는 실체화(인스턴스)가 불가능해야 합니다."
- 순수 가상 함수: "각 몬스터가 반드시 자신만의 방식으로 구현해야 하는 공격 이름 조회 기능을 선언하세요. 이 기능은 본체(Body)가 없어야 합니다."
- 가상 소멸자: "부모 설계도를 통해 자식 인스턴스가 소멸될 때 메모리 누수가 일어나지 않도록 특별한 장치를 해야 합니다."
- HP 접근 제어: "외부 노출은 철저히 막되, 오직 나를 상속받은 자식 클래스에게만 데이터 접근 권한을 물려주어야 합니다."
- HP 조회: "현재 몬스터의 체력을 외부에서 읽기 전용으로 조회할 수 있는 기능이 필요합니다."
- optional 드롭: "보스급 몬스터만 아이템을 드롭합니다. 에러 방지를 위해 -1이나 nullptr 대신 '값이 있을 수도 비어있을 수도 있는' 안전상자로 포장해 반환하세요."
- Goblin 클래스: "하급 몬스터입니다. HP는 50이며, 'Slash' 공격을 사용합니다. 보스가 아니므로 드롭 아이템이 없습니다."
- Dragon 클래스: "보스급 몬스터입니다. HP는 500이며, 'Fire Breath' 공격을 사용합니다. 처치 시 'Dragon Heart'를 드롭합니다."

### DungeonRaid.h 주석
- 클래스 상단: "던전 탐험대의 중앙 전투 기록부입니다. 여러 공격대(스레드)가 동시에 접근하는 공유 자원을 안전하게 관리합니다."
- killCount: "무결성을 위해 데이터 외부 접근을 금지하고 객체 자체적으로만 통제되는 처치 카운터를 가집니다."
- mutex: "수백 명의 공격대(스레드)가 동시에 호출할 수 있습니다. 데이터 보호를 위한 자물쇠를 선언하세요."
- static: "수많은 탐험대 인스턴스가 생성되더라도 메모리 상에 오직 단 하나만 존재하여 전역적으로 공유되어야 합니다."
- 생성자: "생성자에서 초기화 시 대입(Assignment) 비용을 제거하는 엄격한 초기화 방식을 사용하세요."
- RecordKill: "데이터가 오염될 수 있으므로, 내 작업이 끝날 때까지 아무도 진입하지 못하도록 독점 보호막(자물쇠)을 쳐서 안전하게 보호하세요. 반드시 예외 안전한 RAII 방식의 자물쇠 관리자를 사용하세요."
- GetReport: "문자열 템플릿 내부에 중괄호 {}를 열어 값을 주입하는 모던 C++ 문자열 조립 기법을 쓰세요."
- FloorState: "상태(State)들을 한정된 카테고리로 묶어 안전한 기호 집합으로 정의하세요. Idle(대기), InProgress(공략중), Cleared(클리어) 상태가 필요합니다."

### ExecuteRaid 함수 주석 (DungeonRaid.h 에 자유 함수로 선언하거나 별도 헤더에 선언)
- "단일 렌더링 흐름을 멈추지 않고 나란히 병렬 실행되는 백그라운드 작업자를 소멸 시 자동 Join 되도록 소환하세요."
- "이 함수가 외부의 '중단 깃발'을 볼 수 있도록 알맞은 타입의 매개변수를 첫 번째 자리에 선언하세요."
- "반복 작업 도중 틈틈이 외부에서 깃발이 올라왔는지 확인하여, 신호가 오면 현재 상태를 정리하고 안전하게 리턴하세요."

## Constraints (제약 사항)
- **Multi-File GTest 프로젝트**: 헤더(.h), 소스(.cpp), 테스트(test_*.cpp), CMakeLists.txt 분리 필수
- **CMake 빌드**: `cmake -B build -S . --toolchain=C:/vcpkg/scripts/buildsystems/vcpkg.cmake`, `cmake --build build`
- **테스트 파일 100% 완성**: test_dungeon_raid.cpp에는 `/* ... */` 빈칸이 단 하나도 없어야 함
- **정답 코드(함수 본문) 절대 포함 금지** (Blank Slate Policy): 헤더/소스의 구현부는 학습자가 채움
- **네이밍 컨벤션**: 변수 camelCase, 함수/클래스 PascalCase
- **Uniform Initialization** (`{}`) 사용 강제
- **반드시 `lock_guard`** 사용 (수동 lock/unlock 금지)
- **`std::jthread`** 사용 (구형 std::thread 사용 최소화)
- **몬스터는 `std::unique_ptr<Monster>`**로 관리 (원시 포인터 금지)
- **.cpp/.h 파일 내 이모지 사용 금지** (Windows g++ 인코딩 에러 방지)
- **C++ 표준**: C++23 (`set(CMAKE_CXX_STANDARD 23)`)
