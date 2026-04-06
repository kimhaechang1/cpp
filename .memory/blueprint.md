# Practice Blueprint

## Meta
- **Type**: cumulative_practice
- **Module**: 10
- **Topic**: Debugging & Testing (GTest 기반 TDD)
- **Persona**: basic_tutor
- **TDD Mode**: gtest
- **Target File Prefix**: `practice_cumulative_10_2` (M10의 두번째 누적 실습)
- **Working Directory**: `cpp_basics/module_10/`

## Integrated Concepts (통합 출제 맵)

- **[Core - M10]** GTest 단위 테스트: 전체 시스템의 정합성을 `TEST()` 매크로로 검증하는 테스트 파일이 시나리오의 뼈대. 학습자가 구현한 코드가 모든 테스트를 통과해야 함.
- **[M10 past]** 멀티 파일 빌드: 헤더(.h)와 소스(.cpp)를 분리하고, CMakeLists.txt에 새 테스트 타겟을 추가하는 빌드 구조 설계.
- **[M1]** enum class: 퀘스트의 상태(대기중/진행중/완료/실패)를 안전한 범위형 열거체로 정의.
- **[M2]** switch: 퀘스트 상태에 따라 다른 행동을 분기하는 제어 흐름.
- **[M3]** Call by Reference: 퀘스트의 상태를 외부에서 변경(진행중->완료)할 때 원본을 직접 수정.
- **[M4]** const T&: 퀘스트 정보를 조회할 때 불필요한 복사 없이 읽기 전용으로 접근.
- **[M5]** Stack vs Heap: 다형적 객체가 힙에 생성되어야 하는 이유 인식 (스마트 포인터와 연결).
- **[M6]** operator==: 두 퀘스트가 동일한지(이름 기준) 비교하는 연산자 오버로딩.
- **[M7]** 순수 가상 함수 & 추상 클래스: 퀘스트 기본 클래스에 보상 계산 메서드를 순수 가상으로 선언. MainQuest와 SideQuest가 각각 다르게 구현.
- **[M8]** Lambda: 퀘스트 목록에서 특정 상태의 퀘스트만 필터링하는 즉석 로직.
- **[M9]** unique_ptr + std::expected: 퀘스트의 독점 소유권 관리 + 중복 등록/미발견 에러를 안전하게 반환.

## Scenario (시나리오)

> **"Quest Journal (퀘스트 저널) 시스템"**
>
> 당신은 RPG 게임의 퀘스트 관리 시스템을 개발하고 있습니다.
> 플레이어는 다양한 종류의 퀘스트(메인 퀘스트, 서브 퀘스트)를 수주하고,
> 진행 상태를 추적하며, 완료 시 보상을 계산받습니다.
>
> 이 시스템은 반드시 **단위 테스트(GTest)**를 먼저 통과하는 방식으로 개발됩니다.
> 테스트 파일(`test_quest_journal.cpp`)이 이미 작성되어 있으며,
> 당신의 임무는 테스트가 컴파일되고 통과되도록 헤더와 소스 파일을 구현하는 것입니다.
>
> **진정한 TDD**: 테스트 코드를 먼저 읽고, 그 테스트가 요구하는 인터페이스를 역추론하여
> 클래스와 함수를 설계하세요.

## File Structure (파일 구조)

- `Quest.h`: 퀘스트 관련 열거형, 기본 퀘스트 클래스(추상), 파생 클래스(메인/서브) 선언. **학습자가 작성**. 주석 가이드만 제공.
- `QuestJournal.h`: 퀘스트 저널(관리자) 클래스 선언. **학습자가 작성**. 주석 가이드만 제공.
- `QuestJournal.cpp`: 퀘스트 저널 클래스 구현. **학습자가 작성**. 빈 파일로 제공.
- `test_quest_journal.cpp`: GTest 테스트 파일. **에이전트가 완성하여 제공**. 학습자는 이 파일을 읽고 인터페이스를 역추론.
- `CMakeLists.txt`: 기존 파일에 `QuestJournalTest` 타겟 추가.

### TDD 적용 방식 (GTest 모드)
> 기존 Blank Slate Policy에서는 함수 원형을 숨겼지만, GTest TDD 모드에서는 **테스트 코드 자체가 명세서(Specification)** 역할을 합니다.
> - 테스트 파일이 `journal.AddQuest(...)`, `quest->GetReward()` 등을 호출하므로, **함수 이름과 호출 패턴**은 자연스럽게 노출됩니다.
> - 학습자의 과제는: 테스트를 읽고, 필요한 **클래스 계층구조, 내부 자료구조, 메모리 관리 전략**을 스스로 설계하는 것입니다.
> - **주석 가이드**는 Quest.h, QuestJournal.h에만 배치하며, "어떤 개념을 써야 하는지"를 암시합니다.

## Test Cases (테스트 케이스 설계)

### Suite Name: `QuestJournalSuite`

1. **TEST(QuestJournalSuite, AddAndCount)**
   - 목적: 퀘스트를 추가하고 총 개수를 확인
   - 기대 동작: MainQuest 1개, SideQuest 1개 추가 후 `GetQuestCount()` == 2
   - 검증 개념: unique_ptr 소유권 이전(move), 가변 길이 컨테이너 저장

2. **TEST(QuestJournalSuite, DuplicateQuestError)**
   - 목적: 같은 이름의 퀘스트를 중복 등록했을 때 에러 반환 확인
   - 기대 동작: 두 번째 AddQuest 호출의 반환값이 에러 상태 (`std::expected`의 에러 채널)
   - 검증 개념: std::expected를 통한 에러 처리, operator== (이름 비교)

3. **TEST(QuestJournalSuite, FindQuest)**
   - 목적: 이름으로 퀘스트를 검색하고, 없는 퀘스트 검색 시 에러 반환
   - 기대 동작: 존재하는 이름 -> 퀘스트의 const 참조 반환. 없는 이름 -> expected 에러.
   - 검증 개념: const T& 반환, std::expected, 범위 순회

4. **TEST(QuestJournalSuite, StateTransition)**
   - 목적: 퀘스트 상태를 변경(대기->진행->완료)하고 올바르게 전이되는지 확인
   - 기대 동작: `AdvanceState(questName)` 호출 시 상태가 순차적으로 전이. switch로 분기.
   - 검증 개념: enum class 상태값, Call by Reference로 상태 변경, switch 분기

5. **TEST(QuestJournalSuite, FilterByState)**
   - 목적: 특정 상태의 퀘스트만 필터링하여 개수 확인
   - 기대 동작: 진행중인 퀘스트만 뽑아낸 결과의 size 검증
   - 검증 개념: Lambda를 인자로 받는 필터 함수, const T& 순회

6. **TEST(QuestJournalSuite, PolymorphicReward)**
   - 목적: MainQuest와 SideQuest의 보상 계산이 다형적으로 동작하는지 확인
   - 기대 동작: 같은 `GetReward()` 호출이지만, MainQuest는 기본보상*2, SideQuest는 기본보상*1 반환 (각 파생 클래스마다 다른 배율)
   - 검증 개념: 순수 가상 함수, 업캐스팅된 포인터를 통한 호출, 다형성

## Comment Guide (주석 가이드 - Blank Slate Policy)

### Quest.h 주석

1. **enum (QuestState)**:
   "오류 원천 차단을 위해, 퀘스트의 진행 상태(대기/진행/완료/실패)를 한정된 카테고리로 묶어 안전한 기호 집합으로 정의하세요."

2. **Base Quest class (추상)**:
   - 클래스 설명: "모든 퀘스트 종류의 공통 계약(Contract)을 정의하는 기반 설계도입니다. 이 설계도 자체로는 실체화(인스턴스)가 불가능합니다."
   - 멤버 데이터: "퀘스트 이름(문자열)과 기본 보상(정수), 그리고 현재 상태를 보관합니다."
   - 상태 접근: "현재 상태를 외부에서 읽을 수 있어야 합니다. 불필요한 복사 없이."
   - 이름 접근: "현재 이름을 외부에서 읽을 수 있어야 합니다. 불필요한 복사 없이."
   - 보상 계산: "파생 타입마다 보상 배율이 다릅니다. 기반 클래스에서는 계산 공식의 빈 껍데기만 약속하세요."
   - 비교 연산: "두 퀘스트가 '같다'는 것은 이름이 동일하다는 뜻입니다. 비교 연산자를 재정의하세요."
   - 상태 전이: "함수 호출이 끝난 후에도 원본 상태가 수정된 결과를 보존해야 합니다. 대기->진행->완료 순서로 전이하며, 상태에 따라 분기하세요."
   - 소멸: (주석 없음 - 학습자가 업캐스팅 구조를 보고 가상 소멸자 필요성을 스스로 인지해야 함)

3. **MainQuest class (파생)**:
   "메인 스토리 퀘스트입니다. 보상은 기본 보상의 2배입니다."

4. **SideQuest class (파생)**:
   "부가 퀘스트입니다. 보상은 기본 보상 그대로(1배)입니다."

### QuestJournal.h 주석

1. **클래스 설명**:
   "퀘스트들의 원본을 독점 소유하며 관리하는 저널입니다. 스택 수명 스코프를 벗어나는 즉시 자동으로 정리되는 똑똑한 독점 래퍼로 각 퀘스트를 감싸서 보관하세요."

2. **내부 저장소**:
   "퀘스트 래퍼들을 1차원으로 순차적, 연속적으로 보관하는 가변 길이 컨테이너를 사용하세요."

3. **AddQuest 함수**:
   "새 퀘스트를 저널에 등록합니다. 이미 같은 이름이 존재하면 에러를 반환하세요. 에러 방지를 위해 성공/실패를 안전상자로 포장해 반환하세요. 퀘스트의 소유권은 저널로 완전히 이전됩니다(복사 금지, 강탈만 허용)."

4. **FindQuest 함수**:
   "이름으로 퀘스트를 검색합니다. 찾으면 원본의 읽기 전용 참조를, 못 찾으면 에러를 안전상자에 담아 반환하세요."

5. **AdvanceState 함수**:
   "이름으로 퀘스트를 찾아 상태를 한 단계 전진시킵니다."

6. **FilterByState 함수**:
   "현장에서 이름 없는 1회용 로직 덩어리를 받아서, 조건에 맞는 퀘스트들의 읽기 전용 참조 목록을 반환하세요."

7. **GetQuestCount 함수**:
   "현재 등록된 퀘스트의 총 개수를 반환합니다."

## Constraints (제약 사항)

- **금지 키워드 (주석/가이드에서 절대 노출 금지)**: `vector`, `unique_ptr`, `virtual`, `override`, `abstract`, `expected`, `move`, `lambda`, `enum class` (대신 Master Prompt Templates의 암시적 문구 사용)
- **소멸자 힌트 금지**: 가상 소멸자는 학습자가 업캐스팅 + 힙 할당 구조를 보고 스스로 떠올려야 함
- **난이도 조절**: 
  - `std::expected`의 에러 타입은 단순 `std::string`으로 충분 (복잡한 에러 계층 불필요)
  - FilterByState의 반환 타입은 `std::vector<const Quest*>` 수준으로 단순화 (소유권 이전 불필요, 읽기 전용 포인터만)
  - 상태 전이는 단방향(Inactive->Active->Completed)만 지원. Failed 상태는 별도 함수 없이 enum에만 정의.
- **.cpp/.h 파일 내 이모지(이모티콘) 사용 절대 금지** (Windows g++ 인코딩 오류 방지)
- **코딩 컨벤션**: Variables=camelCase, Functions/Classes=PascalCase
- **Uniform Initialization** 사용 권장 (`int a{0}`)
- **include guard**: `#pragma once` 사용
- **C++ 표준**: C++23 (`-std=c++23`)
