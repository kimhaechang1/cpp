# Session Checkpoint: C++ Basics (Module 6 종료 및 결산)

## 📌 Current Goal
*   **[Module 6 완전 정복]**: 객체의 생명주기, 복사/이동 제어, Rule of Zero 뿐만 아니라 클래스 내부의 컴파일 타임 최적화(`constexpr`), 고유 식별 분리(`static`, `= delete`) 등 고급 클래스 설계 역량을 마스터한다.
*   **[L-Value / R-Value 심화 탐구]**: 메모리의 "방(주소)" 여부를 기준으로 임시 객체(R-value)의 본질을 파악하고, 참조자(`&`)와 `const`가 컴파일 타임에 일으키는 생명 연장(Lifetime Extension)의 마법을 증명한다.

## ✅ Completed Tasks
1.  **[Module 6] `constexpr` 실습 완료**: 런타임 계산 비용을 0으로 만드는 컴파일 타임 최적화의 원리를 하드웨어 관점(어셈블리 상수 패킹)에서 입증. 정적 배열 크기 지정 등에서 `static_assert`를 통해 완벽히 검증.
2.  **[Cumulative Practice M6 Final] 성공**: 
    *   `std::span` + `std::vector` + `std::unique_ptr` 콤보를 활용한 무결점 뷰 데이터 순회 생성 구현.
    *   C++11 `= delete` 및 C++14 `std::make_unique` 등 모던 철학 적용으로 리팩토링 완료.
    *   Memory Leak 0% (Rule of Zero)의 자동 소멸 보장 설계 달성.
3.  **[심화] L-value & R-value 원리 규명**: R-value(방이 없는 임시 값)가 `const &`를 만났을 때 스택 메모리에 임시 방이 새로 생성되는 과정 테스트 완료 (`practice_rvalue_address.cpp`).

## 🚀 Next Steps
*   **[Tier 3 고급 진입 (Module 07)]**: 객체 지향의 두 번째 기둥, **상속(Inheritance)과 다형성(Polymorphism)** 시작. Base 클래스와 Derived 클래스의 관계망 설계 (`Monster` -> `Slime`, `Orc`).
*   **[대규모 결산 준비]**: Module 7이 끝난 뒤 이어질 Tier 3(클래스/상속) 결산 미니 프로젝트(텍스트 RPG 개선판)에 대비하기.

## ⚠️ Key Constraints & Learner Insights
*   **뛰어난 직관력**: 학습자가 C++ 내부에서 일어나는 투명한 동작(참조자와 포인터의 구별, 이름 없는 임시 객체의 실체)을 의심하고 집요하게 파고들어 진리를 스스로 이끌어냄. 
*   **조기 최적화 감각**: 작은 데이터 타입(`enum`, `int`)은 참조 오버헤드나 불필요한 스택 할당(Lifetime extension)보다 값 복사(Call by value)가 훨씬 효율적임을 정확히 계산하고 꿰뚫어 보는 능력을 지님.
*   **멘토 지침**: 추후 Module 09 (모던 C++ 및 이동 시맨틱) 진입 시, 이번에 깨우친 L-Value/R-value 지식을 기반으로 폭발적인 성장이 기대됨! 관련된 이동 생성자(`&&`) 파트에서 현업 수준의 극한 최적화 모델을 제시할 것.
