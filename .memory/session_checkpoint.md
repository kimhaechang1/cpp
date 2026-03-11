# Session Checkpoint

## 🎯 Current Goal
- Module 07: 상속과 다형성(가상 함수, vtable, vptr) 마스터 및 누적 실습 진행

## ✅ Completed Tasks
- [x] **상속(Inheritance) 기초 체득**: `Monster`, `Slime`, `Orc` 부모-자식 클래스 분리 및 초기화 리스트를 통한 부모 생성자 호출 실습 통과 (`practice_inheritance.cpp`).
- [x] **접근 제어자 (protected) 매커니즘 완벽 이해**: 부모의 상속 모드에 따른 권한 필터링 원리, 그리고 `friend` 키워드와의 연관 관계를 깊이 있게 파악하고 증명 (`practice_protected.cpp`).
- [x] **누적 복습 통과 (Cumulative M1~M7)**: 지난 7개의 모듈을 총결산하는 게임 로직 통합 실습 완수. 현업자 기준 피드백(`const` 참조, 자기 대입 방어, 제한적 `friend` 선언)까지 문서화하여 완벽 반영 (`practice_cumulative_m7_1.cpp`).
- [x] **다중 상속과 다이아몬드 문제**: 다이아몬드 상속 구조로 인한 이름 모호성과 메모리 중복(2개의 객체 생성) 현상을 확인하고, 가상 상속(`virtual`)을 통한 vbptr(Virtual Base Pointer) 주입과 물리적인 메모리 구조(`sizeof`) 변화를 증명 완료 (`practice_multiple_inheritance.cpp`).
- [x] **다형성과 가상 함수(Virtual Functions)**: 정적/동적 바인딩의 차이, vtable(클래스 단위 명부)과 vptr(객체 단위 나침반)의 하드웨어 레벨 동작 원리를 완벽하게 분석하고 실습 증명 완료 (`practice_polymorphism.cpp`).
- [x] **가상 소멸자의 선행 이해**: 업캐스팅 파괴 시 발생하는 힙 커럽션(Heap Corruption)의 원리를 분석하고 문서 발행 (`concept_virtual_destructor.md`).
- [x] `CPP_BASICS_CURRICULUM.md` 상태 최신화 동기화 완료 (가상 함수 체크).

## 🚀 Next Steps
- **C++ Basics (Next Session)**:
  1. **[Cumulative Practice]: Multiple Inheritance & Polymorphism** 실습 과제 진행 (M1~M6 누적 통합 코딩 테스트).
  2. **가상 소멸자 (Virtual Destructor)** 실습 진행 (미리 발급한 `concept_virtual_destructor.md` 개념을 바탕으로 정식 커리큘럼 실습 재개).

## ⚠️ Key Constraints (Agent Memory)
- **Workflow**: 다음 세션 시작 시, 사용자가 `@[/basic]` 워크플로우를 호출하면 세션 체크포인트를 읽고 누적 실습부터 즉시 제공할 것.
- **Context 유지**: 학습자의 C++ 메모리 통찰력(주소 캐스팅, 포인터 뷰)이 마스터 수준에 도달했음. 다음 세션에서도 단순 문법이 아닌 내부 동작 원리 위주의 깊이 있는 질의응답을 유지할 것.
