---
description: "[Phase 2 - Gemini] Generate code and documents from a Blueprint created by the /design workflow."
---

# /generate Workflow (Phase 2 - Gemini)

> **담당 모델**: Gemini (빠르고 경제적인 생성 모델)
> **목적**: `/design`(Opus)이 작성한 Blueprint를 읽고, 실제 코드와 문서를 생성합니다.
> **입력**: `.memory/blueprint.md`

## 실행 절차

1. **Blueprint 로드**:
   - `.memory/blueprint.md` 파일을 읽습니다.
   - 파일이 없으면 사용자에게 "먼저 `/design` 워크플로우를 실행해주세요."라고 안내하고 중단합니다.

2. **페르소나 장착**:
   - Blueprint의 `Persona` 필드에 명시된 페르소나 파일(`.agent/roles/*.md`)을 읽고 장착합니다.
   - `GEMINI.md`의 규칙을 준수합니다.

3. **Reference 코드 로드 (신규)**:
   - Blueprint에 `Reference Files` 섹션이 있으면, 해당 경로의 파일들을 반드시 읽어서 기존 프로젝트의 **코딩 패턴, 파일 구조, CMake 설정**을 참고합니다.
   - 기존 프로젝트와 일관된 스타일로 신규 파일을 생성합니다.

4. **파일 생성** (Blueprint의 `File Structure` 섹션을 **반드시** 따름):

   #### 4-1. 구현 파일 (`*.h`, `*.cpp`) - 학습자가 구현할 파일
   - **헤더 (`.h`)**:
     - `#pragma once` 가드 필수.
     - Blueprint의 Comment Guide에 따른 **암시적 주석**을 클래스 선언 위에 배치합니다.
     - **Blank Slate Policy 엄수**: 함수 원형(시그니처)은 절대 제공하지 않습니다.
     - 대신, 어떤 기능이 필요한지 **주석으로만** 암시합니다.
     - 학습자가 테스트 코드를 보고 일치하는 시그니처를 역추론해야 합니다.
   - **소스 (`.cpp`)**:
     - 헤더를 포함(`#include`)하고, 학습자가 함수 본문을 구현할 빈 영역만 남깁니다.
     - 필요한 멤버 함수의 이름조차 적지 않습니다. 학습자가 `.h`를 먼저 설계한 후 `.cpp`에서 구현합니다.

   #### 4-2. 테스트 파일 (`test_*.cpp`) - 에이전트가 **100% 완성**하는 파일
   - **[필수] 테스트 코드는 빈칸 없이 완전히 작성합니다.** (학습자의 "목표 명세서" 역할)
   - Blueprint의 Test Cases에 따라 GTest 코드를 작성합니다.
   - `TEST()` 매크로, `EXPECT_EQ`/`ASSERT_EQ` 등을 사용합니다.
   - 테스트 코드가 호출하는 **함수 이름, 매개변수, 반환 타입**이 학습자에게 역공학 힌트가 됩니다.
   - 파일 최상단에 "이 테스트가 통과하도록 구현 파일을 완성하세요"라는 안내 주석을 둡니다.

   #### 4-3. CMakeLists.txt
   - Blueprint에서 `TDD Mode: gtest`인 경우 **반드시** CMakeLists.txt를 생성하거나 기존 것을 수정합니다.
   - `find_package(GTest CONFIG REQUIRED)` 포함.
   - 테스트 실행 파일 타겟(`add_executable`)에 **테스트 소스 + 구현 소스(.cpp)**를 모두 등록합니다.
   - `target_link_libraries`에 `GTest::gtest GTest::gtest_main`를 링크합니다.
   - C++ 표준은 `set(CMAKE_CXX_STANDARD 23)` 이상으로 설정합니다.

   #### 4-4. 개념 문서 (`concept_*.md`)
   - Blueprint의 Outline과 설명 포인트에 따라 작성합니다. (해당되는 경우에만)

5. **코딩 컨벤션 적용**:
   - Variables: `camelCase`
   - Functions/Classes: `PascalCase`
   - `.cpp`/`.h` 파일 내 이모지 사용 금지
   - `int a{0}` (Uniform Initialization) 사용
   - 헤더 가드: `#pragma once`

6. **생성 결과 보고**:
   - 생성된 파일 목록을 나열합니다.
   - **빌드 명령어를 정확히 안내합니다** (CMake 기반: `cmake -B build -S .`, `cmake --build build`).
   - Blueprint에 정의된 시나리오를 학습자에게 전달합니다.

7. **Blueprint 아카이브** (선택):
   - 생성이 완료되면, Blueprint 파일을 `.memory/archive/blueprint_YYYYMMDD_topic.md`로 이동하여 보관합니다.

## 중요 규칙

- **Blueprint를 벗어나지 않기**: Blueprint에 명시되지 않은 추가 파일이나 개념을 임의로 추가하지 않습니다.
- **정답 코드 절대 금지**: 학습자가 구현할 함수의 본문(Body)은 비워둡니다.
- **테스트 코드는 100% 완성 필수**: 테스트 파일에는 빈칸(`/* ... */`)이 단 하나도 없어야 합니다. 모든 `TEST()`, `EXPECT`, `ASSERT` 구문이 완벽하게 작성되어 있어야 학습자가 "어떤 메서드를 어떤 시그니처로 만들어야 하는지" 역공학할 수 있습니다.
- **주석 그대로 사용**: Blueprint의 Comment Guide에 적힌 암시적 문구를 그대로 주석으로 사용합니다.
- **Multi-File 구조 필수** (GTest 모드): 헤더(`.h`), 소스(`.cpp`), 테스트(`test_*.cpp`), 빌드(`CMakeLists.txt`)를 반드시 분리합니다.
- **토큰 효율성**: 불필요한 설명을 최소화하고, 코드 생성에 집중합니다.
