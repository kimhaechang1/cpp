# 🧠 Session Checkpoint (2026-03-19)

## 📍 Current Status
*   **Persona**: `ALGO COACH` (Session ended)
*   **Current Goal**: 
    1.  **Basics**: Module 07 Inheritance & Polymorphism 완강 및 `Tier Exam` 진입 준비 완료.
    2.  **Algorithm**: 0x07 Deque 완강 (BOJ 5430 AC 완료)

## ✅ Completed Tasks (This Session)
*   **Algorithm**: 
    - BOJ 5430 (AC) 골드 5 문제 해결. 
    - `substr` 사용으로 인한 Heap Allocation 오버헤드를 발견하고, O(N^2) 문자열 복사 병목 현상을 파악함.
    - 임시 벡터(`xs`) 저장소를 제거하고 문자를 순회하며 문자를 읽는 즉시 정수로 조합해 바로 `deq`에 꽂아 넣는 극강의 **Zero-copy 파싱 로직** 구현 성공. (DOD 관점 통과)
*   **Progress Tracking**:
    - `LEARNER_PROFILE.md` 업데이트 완료 (해결 문제 수: 25).
    - `CPP_ALGORITHM_CURRICULUM.md` 0x07 덱 파트 완료 목록 갱신.

## 🔜 Next Steps
1.  **Basics Project**: **`Tier Exam: OOP RPG Battle System`** 
    - 경로: `cpp_basics/project_rpg_battle/main.cpp`
    - 목표: M1~M7 지식 총망라 (다형성, 가상 소멸자, 캐스팅 등).
2.  **Algorithm**: 다음 챕터인 `0x08 스택의 활용` 문제 진입.

## ⚠️ Key Constraints
*   **Blank Slate Policy**: 졸업 프로젝트 구현 시 문법 힌트 없이 주석 요구사항만 보고 구조를 직접 설계해야 함.
*   **Design Rule**: Stick to Game Dev perspective (Cache Locality, Zero-copy).
