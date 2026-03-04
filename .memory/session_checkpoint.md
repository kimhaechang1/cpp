# Session Checkpoint: C++ Basics (Module 6)

## 📌 Current Goal
*   객체 지향 설계의 핵심인 **복사 생성자**와 **대입 연산자 오버로딩**을 이해하고, 이를 통한 깊은 복사(Deep Copy)와 동적 할당 메모리의 안전한 관리를 직접 구현(Blank Slate Policy)하여 숙달한다.

## ✅ Completed Tasks
1.  **[Module 6] 대입 연산자 완성**: `Inventory` 클래스에 자기 자신 대입(Self-Assignment) 방지와 기존 메모리 해제를 포함한 완벽한 얕은 복사 방지 로직 완성.
2.  **[Module 6] Friend & Static Members**: `Slime` 클래스와 `SlimeSpawner`를 통한 접근 제어 및 누적 카운팅 실습 완료.
3.  **[Module 6] Rule of 3/5/0 학습**: 수동 리소스 관리의 한계와 `std::unique_ptr`를 활용한 Rule of Zero의 철학 이해 및 문서화 완료.
4.  **Blank Slate Policy 강화**: 실습 문제 생성 시 주석에 구체적인 타입(예: 포인터, span)을 직접 명시하지 않고 "목적성"만 기재하도록 룰 업데이트.
5.  **[Cumulative Practice] M6 통과**: `practice_cumulative_subtitle3_4.cpp` (PlayerInventory 클래스) 백지 상태 구현 성공.

## 🚀 Next Steps
*   **[Module 6]** 다음 진도 진행: **constexpr** - 런타임이 아닌 컴파일 타임에 값을 계산하여 성능을 극대화하는 모던 C++ 기법 학습.
*   **[Cumulative Practice]**: Rule of 3/5/0 & constexpr 통합 누적 연습 진행 예정.

## ⚠️ Key Constraints & Learner Insights
*   학습자의 C++ 동작 원리(생성자/소멸자 시점, 깊은/얕은 복사 결과, 임시 객체의 흐름)에 대한 직관이 매우 우수함. 
*   **"왜 이렇게 비효율적인 구문이 존재하는가?"** 에 대한 물음표를 던지며 곧바로 Modern C++(R-value, emplace 등)의 도입 배경을 정확히 꿰뚫고 있음. 
*   추후 [Module 9] 모던 C++ 최적화 단원 진입 시 메모리 관련 오버헤드를 줄이는 기법에 대해 더 세밀하고 완성도 높은 안내 필요.
