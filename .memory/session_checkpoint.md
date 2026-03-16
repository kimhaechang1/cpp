# Session Checkpoint

## 🎯 Current Goal
- **[Basics]** Module 07: 가상 소멸자 & override 완료 → 다음: 누적 실습 M7_3 (Safety in Polymorphism)
- **[Algorithm]** 0x07 덱 10866 완료 → 다음: 1021번 (회전하는 큐)

## ✅ Completed Tasks (2026-03-16 세션)

### Basics
- [x] **vptr / vtable / vbptr 삼총사 완전 정복**: 하루 종일 Q&A를 통해 오해 교정 완료.
    - vbptr = 클래스마다 / vptr = 객체마다 (기존 반대로 알고 있던 것 교정)
    - 상속 메모리 = 연속 블록 (Node 기반 불연속 구조로 오해했던 것 교정)
    - vtable = 컴파일 시점 고정 / vptr = 생성자 시퀀스로 덮어쓰기
    - 해당 최종 정리를 `concept_polymorphism_and_virtual.md` 말미에 보존.
- [x] **가상 소멸자 (Virtual Destructor)** 개념 문서(`concept_virtual_destructor.md`) 완독 및 이해.
    - 문서에 예제 클래스 정의 섹션(섹션 0) 추가.
    - "virtual 함수 있으면 소멸자도 virtual" 규칙 Q&A 문서에 보존.
    - "가상 소멸자는 일반 소멸자의 역할도 한다" Q&A 보존.
- [x] **가상 소멸자 실습** (`practice_virtual_destructor.cpp`) 완성 및 3/3 PASS.
    - 코드 리뷰 피드백 반영: 출력 오타 4곳 수정.

### Algorithm
- [x] **0x07 덱(Deque)** 개념 2종 문서화 (`concept_deque.md`, `concept_deque_node.md`).
- [x] **10866번 (덱)** 정적 배열 기반 직접 구현 완성.
- [x] **STL std::deque 사용법** 문서화 (`concept_deque_stl.md`).

## 🚀 Next Steps

### C++ Basics (최우선)
1. **누적 실습 M7_3**: Safety in Polymorphism
   - 범위: 가상소멸자/override + [현재 M7 과거 소제목 중 랜덤 1개] + [M1~M6] 각 1개 필수
2. **순수 가상 함수 & 추상 클래스** 개념 문서 → 실습

### Algorithm (이어서)
1. **1021번 (회전하는 큐)** — 덱을 회전시키는 횟수 최소화

## ⚠️ Key Constraints (Agent Memory)
- **Blank Slate Policy**: 실습 파일 제공 시 변수명/함수명은 주석으로 명시, 타입/구조는 절대 노출 금지.
- **누적 실습에는 어떤 주석 힌트도 금지** — 오직 비즈니스 로직(목적)만 서술.
- **M7_3 누적 실습**: 이전 누적 실습들이 RPG/전투 테마였으므로 다른 테마(예: 인벤토리, 서버 시스템 등) 고려.
