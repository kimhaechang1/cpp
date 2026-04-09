# Practice Blueprint

## Meta
- **Type**: cumulative_practice
- **Module**: 10 (Cumulative Practice 10-3: Version Control & Collaboration)
- **Topic**: Git/Collaboration + M1~M10 Full Integration
- **Persona**: basic_tutor
- **TDD Mode**: gtest (CMake project)

## Integrated Concepts (통합 출제 맵)

> 이전 M10 누적 실습(QuestJournal)에서 사용한 `std::vector` + `std::expected` 패턴과의 **차별화**를 위해,
> 이번에는 `std::map` 기반 저장소 + `std::optional` 반환 전략을 채택합니다.

- **[Core - M10]** Git/협업 (버전 관리):
  - 시나리오의 핵심 도메인. 커밋(Commit) 객체가 해시(Hash), 메시지(Message), 작성자(Author)를 가지며 버전 이력을 추적합니다.
  - `.gitignore` 개념을 간접 반영: "무시 패턴" 목록에 해당하는 파일을 커밋에서 제외하는 필터링 로직.

- **[M10 Past - Random]** 단위 테스트 (GTest):
  - 모든 기능 검증을 `TEST()` 매크로로 수행. `EXPECT_EQ`, `ASSERT_TRUE`, `EXPECT_FALSE` 등 활용.

- **[M1]** Uniform Initialization (`{}`) + `enum class`:
  - 모든 변수/객체 초기화에 `{}` 사용 강제.
  - 커밋 타입(Feature/Bugfix/Refactor)을 안전한 기호 집합(enum class)으로 정의.

- **[M2]** 조건 분기 (`switch`):
  - 커밋 타입(enum class)에 따라 서로 다른 우선순위(Priority) 점수를 부여하는 분기 로직.

- **[M3]** Call by Reference (`const&`):
  - 커밋 객체 전달 시 불필요한 복사를 방지하는 안전한 읽기 전용 참조 전달.

- **[M4]** `std::span`:
  - 주간 커밋 빈도 배열(7일치)에서 평일(월~금, 5개)만 잘라낸 "시야(View)"를 대여.

- **[M5]** `std::array`:
  - 7일 고정 크기의 주간 커밋 빈도 버퍼. `std::array<int, 7>` (월~일).

- **[M6]** `operator<<` (Friend) + `static` 멤버:
  - 커밋 객체를 `std::cout <<`으로 출력 시 "[타입] 해시(8자): 메시지" 형식으로 예쁘게 출력.
  - 전체 커밋 생성 횟수를 추적하는 전역 공유 카운터 (static).

- **[M7]** 상속 / 추상 클래스 / 가상 소멸자:
  - `Commit` 추상 기반 클래스 → `FeatureCommit`, `BugfixCommit` 파생.
  - 순수 가상 함수 `GetTypeLabel()`을 자식이 구현.
  - **가상 소멸자는 스켈레톤에 제공하지 않음** (Blank Slate Policy). 업캐스팅 구조를 보고 학습자가 판단.

- **[M8]** `std::map` + Lambda:
  - 커밋 해시(string)를 키로, 커밋 소유권(unique_ptr)을 값으로 저장하는 자동 정렬 컨테이너.
  - 람다를 인자로 받아 조건부 필터링 수행 (예: 특정 타입만 추출).

- **[M9]** `std::unique_ptr` + `std::optional`:
  - 커밋의 독점 소유권 관리.
  - 해시로 커밋 조회 실패 시 "비어있을 수 있는 상자(optional)"에 담아 반환.
  - `std::move`를 활용한 소유권 이전.

## Scenario (시나리오)

### "커밋 히스토리 분석기 (Commit History Analyzer)"

> 당신은 게임 스튜디오의 **내부 빌드 관리 도구**를 개발하고 있습니다.
> 개발팀은 Git으로 코드를 관리하며, 매일 수십 개의 커밋이 올라옵니다.
>
> 팀장은 다음과 같은 기능을 가진 **"커밋 분석 시스템"**을 요청했습니다:
> 1. 다양한 종류의 커밋(신규 기능, 버그 수정)을 등록하고 관리
> 2. 커밋 해시로 특정 커밋을 빠르게 조회
> 3. 이번 주에 요일별로 커밋이 몇 개 올라왔는지 통계 확인
> 4. 특정 조건(타입별)으로 커밋 필터링
> 5. 커밋 정보를 보기 좋게 출력
>
> 이 시스템의 핵심 클래스들을 TDD(테스트 주도 개발) 방식으로 구현하세요.

## File Structure (파일 구조)

- `Commit.h`: 커밋 기반 클래스 및 파생 클래스 선언 (스켈레톤 제공, 학습자가 빈칸 채움)
- `Commit.cpp`: 커밋 클래스 멤버 함수 구현 (학습자가 전체 구현)
- `CommitAnalyzer.h`: 커밋 관리자 클래스 선언 (스켈레톤 제공, 학습자가 빈칸 채움)
- `CommitAnalyzer.cpp`: 커밋 관리자 멤버 함수 구현 (학습자가 전체 구현)
- `test_commit_analyzer.cpp`: GTest 파일 (완전 제공, 학습자는 수정하지 않음)
- `CMakeLists.txt`: 기존 파일에 새로운 테스트 타겟 추가

## Test Cases (테스트 케이스 설계)

### Suite 1: CommitTest (커밋 객체 자체 검증)

1. **TEST(CommitTest, FeatureCommitCreation)**
   - 목적: FeatureCommit이 올바르게 생성되고 GetTypeLabel()이 "Feature"를 반환하는지 검증
   - 기대 동작: hash="abc12345", message="Add player", author="Kim" → GetTypeLabel() == "Feature", GetHash() == "abc12345"

2. **TEST(CommitTest, BugfixCommitCreation)**
   - 목적: BugfixCommit이 올바르게 생성되고 GetTypeLabel()이 "Bugfix"를 반환하는지 검증
   - 기대 동작: hash="def67890", message="Fix crash" → GetTypeLabel() == "Bugfix"

3. **TEST(CommitTest, StreamOutput)**
   - 목적: `operator<<`가 "[타입] 해시(8자): 메시지" 형식으로 출력하는지 검증
   - 기대 동작: FeatureCommit → 출력 문자열이 "[Feature] abc12345: Add player system" 형식과 일치
   - 검증 방법: `std::ostringstream`에 출력 후 문자열 비교

4. **TEST(CommitTest, StaticCommitCounter)**
   - 목적: 모든 Commit 파생 객체의 생성 횟수를 static 카운터가 정확히 추적하는지 검증
   - 기대 동작: FeatureCommit 2개 + BugfixCommit 1개 생성 → static 카운터 == 3
   - 주의: 테스트 간 간섭 방지를 위해 테스트 시작 시 카운터 리셋 필요 (ResetCounter static 함수)

5. **TEST(CommitTest, PriorityByType)**
   - 목적: 커밋 타입에 따라 우선순위 점수가 다르게 반환되는지 검증 (switch 분기)
   - 기대 동작: Feature → 1, Bugfix → 3 (버그 수정이 더 급함)
   - 이 함수는 Commit의 멤버 함수로, 내부에서 GetTypeLabel()의 결과에 따라 switch 분기

### Suite 2: AnalyzerTest (관리자 검증)

6. **TEST(AnalyzerTest, RegisterAndFindCommit)**
   - 목적: 커밋을 등록하고 해시로 조회했을 때 올바른 커밋이 반환되는지 검증
   - 기대 동작: RegisterCommit → FindByHash("abc12345") → optional이 값을 가지며, 해당 커밋의 메시지가 일치

7. **TEST(AnalyzerTest, FindReturnsEmptyOnMiss)**
   - 목적: 존재하지 않는 해시로 조회 시 빈 optional이 반환되는지 검증
   - 기대 동작: FindByHash("nonexistent") → has_value() == false

8. **TEST(AnalyzerTest, WeeklyStatsArray)**
   - 목적: 7일 고정 크기 통계 배열에 요일별 커밋 수를 기록하고 정확히 반환하는지 검증
   - 기대 동작: RecordCommitOnDay(0, 3) → GetWeeklyStats()[0] == 3 (월요일에 3개)
   - 반환 타입: const std::array<int, 7>&

9. **TEST(AnalyzerTest, WeekdayStatsSpanView)**
   - 목적: 7일 배열에서 평일(인덱스 0~4)만 잘라낸 span을 반환하는지 검증
   - 기대 동작: GetWeekdayStats() → span의 size() == 5, 각 요소가 원본 배열의 0~4번과 동일
   - 반환 타입: std::span<const int, 5> (또는 동적 span)

10. **TEST(AnalyzerTest, FilterByTypeWithLambda)**
    - 목적: 람다를 인자로 받아 특정 타입의 커밋만 필터링하는지 검증
    - 기대 동작: Feature 2개, Bugfix 1개 등록 → Feature만 필터링 → 결과 크기 == 2
    - 람다: `[](const Commit& c) { return c.GetTypeLabel() == "Feature"; }`

11. **TEST(AnalyzerTest, GetTotalCommitCount)**
    - 목적: Analyzer에 등록된 커밋의 총 개수를 정확히 반환하는지 검증
    - 기대 동작: 3개 등록 → GetTotalCount() == 3

## Comment Guide (주석 가이드 - Blank Slate Policy)

> 아래 문구들은 `.agent/roles/basic_tutor.md`의 Master Prompt Templates에서 직접 인용하거나 변형한 것입니다.
> 정답 키워드(vector, map, virtual, unique_ptr 등)를 절대 노출하지 않습니다.

### Commit.h 주석

- **CommitType enum class**:
  "오류 원천 차단을 위해, 커밋의 종류(신규 기능 추가, 버그 수정)를 한정된 카테고리로 묶어 안전한 기호 집합으로 정의하세요."

- **Commit 기반 클래스**:
  "인스턴스화가 불가능한 인터페이스 설계법을 사용하세요. 커밋의 종류를 문자열로 반환하는 행위를 자식 클래스가 반드시 구현하도록 강제하세요."

- **소멸자 (Blank Slate - 제공 금지!)**:
  주석 없음. 업캐스팅 구조(`unique_ptr<Commit>`)를 보고 학습자가 스스로 판단.
  단, 헤더에 `// [Safety] 이 클래스의 사용 패턴을 고려하여, 필요하다면 소멸자를 설계하세요.` 정도의 넌지시 유도만 허용.

- **static 카운터**:
  "수많은 커밋 인스턴스가 생성되더라도 메모리 상에 오직 단 하나만 존재하여 전역적으로 공유되어야 하는 카운터입니다."

- **operator<< (Friend)**:
  "외부(main, 테스트)에서 private 멤버에 접근하여 커밋의 타입, 해시, 메시지를 예쁘게 스트림에 출력할 수 있도록 예외적 허용을 사용하세요."

- **GetPriority 함수**:
  "커밋의 종류에 따라 서로 다른 우선순위 점수를 반환해야 합니다. 종류별로 갈라지는 다중 경로 분기를 사용하세요." (switch 유도)

- **FeatureCommit / BugfixCommit**:
  "조상 클래스의 인터페이스를 구체적으로 실현하는 파생 클래스입니다."

### CommitAnalyzer.h 주석

- **커밋 저장소 (데이터 멤버)**:
  "항상 정렬된 상태를 자동 유지하며 특정 Key(해시 문자열)로 Value(커밋)를 찾는 컨테이너를 사용하세요."

- **Value의 소유권**:
  "스택 수명 스코프를 벗어나는 즉시 delete를 스스로 뱉어내는 똑똑한 독점 메모리 래퍼를 사용하세요."

- **RegisterCommit 함수**:
  "커밋의 소유권을 통째로 강탈(이동)하여 저장소에 등록합니다. 무거운 데이터를 복사하여 CPU를 낭비하지 마세요."

- **FindByHash 함수**:
  "에러 방지를 위해 -1이나 nullptr 대신 '값이 있을 수도 비어있을 수도 있는' 안전상자로 포장해 반환하세요."

- **주간 통계 배열 (데이터 멤버)**:
  "7일이라는 고정된 하드웨어 크기를 가지는 연속 메모리 블록을 사용하세요." (std::array 유도)

- **GetWeekdayStats 함수**:
  "원본 배열의 소유권은 갖지 않은 채, 연속된 데이터 중 평일(5일)의 '시야(View)'만 대여받아 요소를 읽을 수 있도록 반환하세요."

- **FilterByType 함수**:
  "함수를 외부에 억지로 빼지 말고, 그 즉시 현장에서 이름 없는 1회용 로직 덩어리를 만들어 넘겨받으세요. 불필요한 복사 없이 읽기 전용 참조로 결과를 전달하세요."

## Constraints (제약 사항)

1. **정답 코드 금지**: Blueprint에 학습자가 구현할 함수의 본문(Body) 없음.
2. **함수 원형 미제공**: `.h` 파일의 함수 선언부도 주석(요구사항 설명)만 제공. 반환 타입/매개변수 타입은 학습자가 테스트 코드를 역추론하여 결정.
3. **가상 소멸자 미제공**: `Commit` 클래스에 `virtual ~Commit()` 을 명시하지 않음. `unique_ptr<Commit>` 패턴을 보고 학습자가 추론.
4. **이전 실습과의 차별화**:
   - QuestJournal: `std::vector` + `std::expected` → 이번: `std::map` + `std::optional`
   - 새로운 요소: `std::array` + `std::span` (주간 통계), `static` 카운터, `switch` 분기, `operator<<`
5. **Uniform Initialization**: 모든 변수/멤버 초기화에 `{}` 사용.
6. **소스 코드 내 이모지 금지**: `.cpp`, `.h` 파일에 이모지 절대 사용하지 않음.
7. **static 카운터 테스트 간섭 방지**: `ResetCounter()` static 함수를 제공하여 각 테스트의 독립성 보장.
8. **CMakeLists.txt**: 기존 타겟(App, TestApp, QuestJournalTest)을 유지하고 새 타겟(`CommitAnalyzerTest`)만 추가.
9. **GetPriority의 switch 분기**: GetTypeLabel()이 반환하는 문자열이 아닌, 내부 CommitType enum을 기준으로 분기해야 자연스러움. 이를 위해 Commit 기반 클래스에 `GetType()` (CommitType 반환) 순수 가상 함수도 필요. 단, 주석에는 "종류를 기호 집합으로 반환하는 행위"로만 암시.
