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

3. **파일 생성** (Blueprint의 `File Structure` 섹션을 따름):
   - **개념 문서 (`concept_*.md`)**: Blueprint의 Outline과 설명 포인트에 따라 작성합니다.
   - **구현 파일 (`*.h`, `*.cpp`)**: 
     - 헤더에는 Blueprint의 Comment Guide에 따른 **암시적 주석**만 포함합니다.
     - 함수 본문은 **비워둡니다** (학습자가 구현).
     - **Blank Slate Policy 엄수**: Blueprint에 함수 원형이 없으므로, 주석 가이드만 보고 적절한 암시적 힌트를 배치합니다.
   - **테스트 파일 (`test_*.cpp`)**: 
     - Blueprint의 Test Cases에 따라 GTest 코드를 작성합니다.
     - `TEST()` 매크로, `EXPECT_EQ`/`ASSERT_EQ` 등을 사용합니다.
   - **CMakeLists.txt**: 필요 시 테스트 타겟을 추가합니다.

4. **코딩 컨벤션 적용**:
   - Variables: `camelCase`
   - Functions/Classes: `PascalCase`
   - `.cpp`/`.h` 파일 내 이모지 사용 금지
   - `int a{0}` (Uniform Initialization) 사용

5. **생성 결과 보고**:
   - 생성된 파일 목록을 나열합니다.
   - 빌드 및 실행 방법을 안내합니다.
   - Blueprint에 정의된 시나리오를 학습자에게 전달합니다.

6. **Blueprint 아카이브** (선택):
   - 생성이 완료되면, Blueprint 파일을 `.memory/archive/blueprint_YYYYMMDD_topic.md`로 이동하여 보관합니다.

## 중요 규칙

- **Blueprint를 벗어나지 않기**: Blueprint에 명시되지 않은 추가 파일이나 개념을 임의로 추가하지 않습니다.
- **정답 코드 금지**: 테스트 코드만 작성하고, 학습자가 구현할 함수의 본문은 비워둡니다.
- **주석 그대로 사용**: Blueprint의 Comment Guide에 적힌 암시적 문구를 그대로 주석으로 사용합니다.
- **토큰 효율성**: 불필요한 설명을 최소화하고, 코드 생성에 집중합니다.
