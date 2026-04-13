---
description: "[Phase 1 - Opus] Design a practice problem or concept document. Creates a Blueprint file for the /generate workflow."
---

# /design Workflow (Phase 1 - Opus)

> **담당 모델**: Opus (고품질 사고 모델)
> **목적**: 커리큘럼을 분석하고, 실습 문제나 개념 문서의 **설계도(Blueprint)**를 작성합니다.
> **출력**: `.memory/blueprint.md` 파일

## 실행 절차

1. **컨텍스트 로드**:
   - `GEMINI.md`를 읽어 프로젝트 헌법과 전체 규칙을 파악합니다.
   - 사용자가 요청한 주제에 해당하는 **페르소나 파일**(`.agent/roles/*.md`)을 읽습니다.
   - 해당 **커리큘럼 파일**(`docs/curriculum/*.md`)을 읽어 현재 진행 상황을 파악합니다.
   - `docs/profile/LEARNER_PROFILE.md`를 읽어 학습자 수준을 확인합니다.

2. **설계 작업** (여기가 Opus의 핵심 역할):
   - **Cumulative Practice인 경우**: 
     - 커리큘럼의 **통합 출제 룰**에 따라, 코어 주제 + 과거 모든 Module에서 랜덤 1개씩 추출합니다.
     - 이 개념들을 엮는 **시나리오(Scenario)**를 구상합니다.
     - **Blank Slate Policy**에 맞춰, 주석에 사용할 암시적 문구를 설계합니다.
     - **테스트 케이스**(GTest `TEST()` 또는 `assert`)를 구체적으로 설계합니다.
   - **개념 문서인 경우**:
     - 문서의 목차(Outline)와 핵심 설명 포인트를 설계합니다.
     - 샌드위치 교육법에 따른 도입부 시나리오를 구상합니다.
     - 하드웨어 검증 코드의 종류를 결정합니다.

3. **Blueprint 파일 생성**:
   - 아래 템플릿에 따라 `.memory/blueprint.md`를 작성합니다.
   - **주의**: Blueprint에는 학습자가 구현해야 할 정답 코드를 포함하지 않습니다.

4. **사용자에게 보고**:
   - 설계 의도와 포함된 개념 목록을 간략히 요약합니다.
   - "이제 `/generate` 워크플로우로 코드를 생성하세요." 라고 안내합니다.

## Blueprint 템플릿 (`.memory/blueprint.md`)

```markdown
# Practice Blueprint

## Meta
- **Type**: cumulative_practice | concept_document | tier_exam
- **Module**: (해당 모듈 번호)
- **Topic**: (핵심 주제)
- **Persona**: basic_tutor | algo_coach | dx_mentor | ...
- **TDD Mode**: gtest | assert (CMake 프로젝트면 gtest, 단독 파일이면 assert)

## Integrated Concepts (통합 출제 맵)
> 각 Module에서 추출한 개념과, 이것이 시나리오에서 어떤 역할을 하는지 명시합니다.

- **[Core - M??]** (방금 배운 핵심 주제): (시나리오 내 역할)
- **[M1]** (추출된 개념): (시나리오 내 역할)
- **[M2]** (추출된 개념): (시나리오 내 역할)
- ...

## Scenario (시나리오)
> 게임 개발 맥락의 스토리라인을 서술합니다.

(시나리오 설명)

## File Structure (파일 구조)
> 생성해야 할 파일 목록과 각 파일의 역할을 명시합니다.

- `filename.h`: (역할 설명)
- `filename.cpp`: (역할 설명 - 학습자가 구현)
- `test_filename.cpp`: (테스트 파일 - GTest 사용 시)
- `CMakeLists.txt`: (수정 사항)

## Test Cases (테스트 케이스 설계)
> 각 테스트의 목적, 입력, 기대 출력을 명시합니다.

1. **TEST(SuiteName, TestName1)**
   - 목적: (무엇을 검증하는가)
   - 기대 동작: (어떤 결과가 나와야 하는가)

2. **TEST(SuiteName, TestName2)**
   - 목적: ...
   - 기대 동작: ...

## Comment Guide (주석 가이드 - Blank Slate Policy)
> 학습자에게 제공할 주석의 정확한 문구를 설계합니다. 
> 정답 키워드(vector, map, virtual 등)를 절대 노출하지 않습니다.
> `.agent/roles/basic_tutor.md`의 Master Prompt Templates를 참조합니다.

- 함수 A: "(암시적 문구)"
- 클래스 B: "(암시적 문구)"

## Constraints (제약 사항)
- (이 문제에서 특별히 주의할 점)
- (금지된 키워드 목록)
- (난이도 조절 사항)
```

## 설계 통합성 체크리스트 (Cohesion Checklist)
> **배경**: Module 11 실습에서 Monster와 DungeonRaid가 유기적으로 연결되지 않고, 
> FloorState::Cleared가 한 번도 사용되지 않는 "기능 나열식 설계"가 발생했습니다.
> 아래 체크리스트를 통해 이런 문제를 사전에 방지합니다.

### 필수 검증 항목 (Blueprint 작성 시 반드시 확인)

1. **데이터 흐름 검증 (Data Flow)**: 
   - 시나리오 내 모든 클래스가 최소 1회 이상 **서로의 메서드를 호출하거나 데이터를 교환**해야 합니다.
   - "A 클래스가 존재하지만 B 클래스와 아무 관계가 없다"면 설계를 수정하세요.
   - 예시: Monster의 HP를 DungeonRaid가 깎고, 드롭 아이템을 DungeonRaid가 수집하는 등.

2. **Enum 완전 소비 (Enum Exhaustion)**:
   - `enum class`에 정의된 **모든 값**이 테스트 코드에서 검증되어야 합니다.
   - 값을 3개 선언하고 2개만 테스트하는 것은 금지합니다.
   - 사용하지 않을 값이라면 처음부터 enum에 넣지 마세요.

3. **반환값 활용 (Return Value Usage)**:
   - 함수가 값을 반환한다면, 테스트에서 **그 값을 활용하는 후속 동작**이 있어야 합니다.
   - `GetDropItem()`으로 아이템을 얻었다면, 그것을 인벤토리에 추가하거나 보고서에 포함하는 등의 흐름이 필요합니다.
   - 단순히 "반환값이 맞는지 확인"만 하고 끝나는 것은 부족합니다.

4. **시나리오 일관성 (Scenario Coherence)**:
   - 모든 테스트 케이스가 **하나의 게임 시나리오 안에서 자연스럽게 연결**되어야 합니다.
   - "기능 A 확인, 기능 B 확인, 기능 C 확인" 식의 독립적 체크리스트가 아니라,
   - "던전에 진입(Idle) -> 몬스터와 전투(InProgress) -> 보스 처치 + 아이템 획득 -> 클리어(Cleared)" 같은 **스토리 흐름**으로 구성하세요.

5. **Cross-Module 연결 (Integration over Isolation)**:
   - 통합 출제 맵의 개념들이 **서로 영향을 주고받도록** 설계해야 합니다.
   - 나쁜 예: "M7 다형성 테스트" + "M11 뮤텍스 테스트" (각각 독립)
   - 좋은 예: "다형적 몬스터 목록을 멀티스레드로 처리하되, 보스 몬스터 드롭 아이템을 mutex로 보호하여 인벤토리에 안전하게 추가" (유기적 통합)

## 중요 규칙
- **정답 코드 금지**: Blueprint에 학습자가 구현할 함수의 본문(Body)을 절대 작성하지 않습니다.
- **함수 원형 금지**: 반환 타입, 매개변수 타입을 Blueprint에 명시하지 않습니다 (Blank Slate Policy).
- **테스트 코드는 허용**: 테스트가 "무엇을 검증하는지"는 명시해도 됩니다 (학습자가 구현할 부분이 아니므로).
