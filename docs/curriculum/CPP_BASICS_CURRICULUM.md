# C++ 언어 기초 학습 커리큘럼

## 📘 개요
단순한 문법 암기가 아닌, **"게임 개발을 위한 실전 C++"**을 목표로 합니다.
각 단계는 **개념 학습 → 실습 문제 → 미니 프로젝트(스켈레톤 코드 채우기)** 순서로 진행됩니다.

---

## 📚 학습 로드맵 (총 4단계)

### 🌱 BEGINNER (입문)
**목표**: 데이터를 변수에 담고, 조건문과 반복문으로 게임 로직을 제어할 수 있다.

#### MODULE 1: C++ 시작하기
- [x] **변수와 자료형**: `int`, `float`, `bool`, `sizeof`
- [x] **사용자 정의 자료형**: `struct` (스탯 구조체 만들기), `enum` (게임 상태 정의)
- [x] **입출력**: `cin`, `cout`
- [x] **[C++20] 초기화의 통일**: `int a{0}` (Uniform Initialization) 습관화
- [x] **[C++20] std::format**: `cout` 대신 `std::format` (Python f-string 스타일) 사용하기

#### MODULE 2: 제어 흐름
- [x] **조건문**: HP에 따른 상태 분기 (`if`, `switch`)
- [x] **반복문**: 턴제 전투의 기본 루프 (`while`, `for`)
- [x] **흐름 제어**: `break`, `continue`

#### MODULE 3: 함수
- [x] **함수 기초**: 데미지 계산 함수 분리
- [x] **매개변수 전달**: `Call by Value` vs `Call by Reference`
- [x] **재귀 함수**: 팩토리얼(경험치 테이블 등)

---
 
### 🏆 BEGINNER 대단원 마무리: 미니 프로젝트 (Tier Exam)
> **[M1 ~ M3 전체]**를 아우르는 절차지향 프로그래밍의 기초 확인.
> 복잡한 클래스 없이, 함수와 구조체만으로 동작하는 안정적인 로직 구축.
 
- [x] **[Mini-Project]**: Text-based Command RPG Prototype (범위: 변수, 구조체, 함수, 제어문 통합 사용)
 
---
 
### 🔧 INTERMEDIATE (중급)
**목표**: 메모리를 직접 조작하고 관리하여, 고정된 크기의 배열을 넘어선다.

#### MODULE 4: 포인터와 참조 (가장 중요 ⭐)
- [x] **포인터 기초**: 메모리 주소의 이해 `*`, `&`
- [x] **포인터와 배열**: 포인터 연산으로 배열 접근하기 (Decay)
- [x] **함수 포인터**: 스킬 버튼에 함수 연결하기 (콜백)
- [x] **[보충] const T&**: `const` 참조를 통한 성능과 안전성 잡기
- [x] **[보충] 2중 포인터**: 포인터의 포인터(`**`), 2차원 배열의 기초
- [x] **[C++20] std::span**: 포인터+길이를 대체하는 안전한 메모리 뷰(View)

#### MODULE 5: 동적 할당
- [x] **메모리 구조**: Stack vs Heap 이해
- [x] **동적 할당**: `new`/`delete` 및 `new[]`/`delete[]` 사용법
- [x] **메모리 누수**: `_CrtSetDbgFlag`를 이용한 누수 탐지
- [x] **[보충] 댕글링 포인터 & nullptr**: 메모리 해제 후 안전한 뒷처리 전략
- [x] **[보충] std::array**: C 스타일 배열(`int[]`)을 완벽하게 대체하는 `std::array` 학습

---
 
### 🏆 INTERMEDIATE 대단원 마무리: 미니 프로젝트 (Tier Exam)
> **[M4 ~ M5 전체]**를 아우르는 메모리 관리 및 저수준 조작 능력 확인.
> 힙 메모리를 직접 다루며 누수 없는 코드를 작성하는 하드웨어 친화적 감각 배양.
 
- [x] **[Mini-Project]**: Dynamic Memory Inventory System (범위: 포인터 연산, 동적 할당/해제, 메모리 누수 방지, std::span/array 활용)
 
---
 
### 🏗️ ADVANCED (고급)
**목표**: "설계"를 한다. 데이터와 기능을 묶어 관리하고, 상속을 통해 코드를 재사용한다.

#### MODULE 6: 클래스와 객체 (설계의 기초)
- [x] **[신규] 네임스페이스 (Namespace)**: 이름 충돌 방지 및 `std::`의 의미 이해, `using` 문구의 올바른 사용법
- [x] **클래스**: `struct`와 `class`의 차이, 접근 지정자 (`private`, `public`)
- [x] **[Cumulative Practice]**: Namespace & Class Basics (범위: Namespace/Class + [M1~M5] 각 모듈 최소 1개 필수)
- [x] **생성자/소멸자**: 객체의 탄생과 죽음 관리 (RAII)
- [x] **[보충] 초기화 리스트**: 대입(`=`)이 아닌 리스트(`:`) 사용법 (성능 및 `const` 멤버)
- [x] **[Cumulative Practice]**: Lifecycle (범위: Lifecycle + [현재 M6 과거 소제목 중 랜덤 1개] + [M1~M5] 각 모듈 최소 1개 필수)
- [x] **복사 생성자**: 얕은 복사 vs 깊은 복사 (메모리 관리의 시작)
- [x] **연산자 오버로딩**: `+`, `==`, `<<` 등 연산자 재정의 및 대입 연산자(`=`) 오버로딩
- [x] **[Cumulative Practice]**: Deep Copy & Operator Overloading (범위: Copy/Overloading + [현재 M6 과거 소제목 중 랜덤 1개] + [M1~M5] 각 모듈 최소 1개)
- [x] **Friend 키워드**: 클래스 외부에서 `private` 멤버에 접근하는 예외적 허용
- [x] **클래스의 정적 멤버**: `static` 변수/함수를 이용한 공유 데이터 관리
- [x] **[Cumulative Practice]**: Friend & Static Members (범위: Friend/Static + [현재 M6 과거 소제목 중 랜덤 1개] + [M1~M5] 각 모듈 최소 1개)
- [x] **[심화] 3/5/0 규칙**: 직접 구현하기보다 `std::unique_ptr`를 활용해 컴파일러에게 맡기는(Rule of Zero) 패턴
- [x] **[C++11~26] constexpr**: 런타임이 아닌 컴파일 타임에 값 계산하기 (성능 최적화)
- [x] **[Cumulative Practice]**: Rule of 3/5/0 & constexpr (범위: 3/5/0 & constexpr + [현재 M6 과거 소제목 중 랜덤 1개] + [M1~M5] 각 모듈 최소 1개 필수)


#### MODULE 7: 상속과 다형성 (설계의 심화)
- [x] **상속**: `Monster`를 상속받는 `Slime`, `Orc` 만들기
- [x] **[보충] protected 접근 제어자**: 상속 관계에서만 허용되는 접근 범위
- [x] **[Cumulative Practice]**: Inheritance Basics & Access Modifiers (범위: 상속/protected + [M1~M6] 각 모듈 최소 1개 필수)
- [x] **[추가] 다중 상속과 다이아몬드 문제**: `가상 상속(Virtual Inheritance)`을 통한 메모리 중복 해결
- [x] **가상 함수**: `virtual Attack()`으로 다형성 구현하기
- [x] **[Cumulative Practice]**: Multiple Inheritance & Polymorphism (범위: 다중상속/다형성 + [현재 M7 과거 소제목 중 랜덤 1개] + [M1~M6] 각 모듈 최소 1개 필수)
- [x] **[보충] 가상 소멸자**: 부모 포인터로 자식 삭제 시 메모리 누수 방지 (필수)
- [x] **[보충] override 키워드**: 실수 방지를 위한 명시적 오버라이딩
- [x] **순수 가상 함수 & 추상 클래스**: 인스턴스화가 불가능한 인터페이스 설계법 ([concept](cpp_basics/module_07/concept_abstract_class.md), [practice](cpp_basics/module_07/practice_abstract_class.cpp))
- [x] **[Cumulative Practice]**: Abstract Class & Polymorphism Safety (범위: 순수가상함수/추상클래스/가상소멸자/override + [현재 M7 과거 소제목 중 랜덤 1개] + [M1~M6] 각 모듈 최소 1개 필수) ([practice](cpp_basics/module_07/practice_cumulative_m7_3.cpp))
- [x] **[심화] 데이터 지향 설계(DOD) 맛보기**: `AoS`(구조체 배열) vs `SoA`(배열의 구조체)와 캐시 적중률(Cache Hit) 이해 ([concept](cpp_basics/module_07/concept_dod_basics.md), [practice](cpp_basics/module_07/practice_dod_compare.cpp))

---

### 🏆 ADVANCED 대단원 마무리: 미니 프로젝트 (Tier Exam)
> **[M1 ~ M7 전체]**를 총망라하는 종합 미니 프로젝트.
> 절차지향 RPG(M1~M5) 이후, 객체지향 설계(M6~M7)의 모든 것을 통합하는 **OOP 기반 RPG 전투 시스템** 구현.

- [x] **[Mini-Project]**: OOP RPG Battle System (범위: M1~M7 전체, 추상 클래스/다형성/가상 소멸자/override/Rule of 3 등 설계 원칙 총동원)

---

### ✨ EXPERT (심화)
**목표**: 더 안전하고, 더 빠르고, 더 편리한 "모던 C++" 기법을 익힌다.

#### MODULE 8: 템플릿과 STL
- [x] **템플릿**: 타입에 상관없는 `Box<T>` 만들기 (일반화 프로그래밍) ([concept](cpp_basics/module_08/concept_template_basics.md), [practice](cpp_basics/module_08/practice_templates.cpp))
- [x] **STL**: `std::vector`, `std::map` 등으로 인벤토리/타일맵 구현 ([concept](cpp_basics/module_08/concept_stl_containers.md), [practice](cpp_basics/module_08/practice_stl_containers.cpp))
- [x] **[Cumulative Practice]**: Templates & Basic STL (범위: 템플릿/STL + [M1~M7] 각 모듈 최소 1개 필수) ([practice](cpp_basics/module_08/cumulative_practice_8_1.cpp))
- [x] **반복자(Iterator)**: 컨테이너의 내부 구조를 몰라도 순회할 수 있는 인터페이스 ([concept](cpp_basics/module_08/concept_iterator.md), [practice](cpp_basics/module_08/practice_iterator.cpp))
- [x] **함수 객체(Functor) & 람다**: 알고리즘 함수(`std::sort` 등)에 사용자 정의 로직 주입 ([concept](cpp_basics/module_08/concept_lambda.md), [practice](cpp_basics/module_08/practice_lambda.cpp))
- [x] **[Cumulative Practice]**: Iterators & Functors/Lambdas (범위: 반복자/람다 + [현재 M8 과거 소제목 중 랜덤 1개] + [M1~M7] 각 모듈 최소 1개 필수) ([practice](cpp_basics/module_08/practice_cumulative_8_2.cpp))
- [x] **[C++20] Ranges & Views**: 파이프(`|`)를 사용해 직관적인 데이터 변환 및 필터링 (`std::ranges::sort`) ([concept](cpp_basics/module_08/concept_ranges.md), [practice](cpp_basics/module_08/practice_ranges.cpp))
- [x] **[C++20] Concepts**: 템플릿의 `typename T`에 제약조건 걸기 (`Integral T`) ([concept](cpp_basics/module_08/concept_concepts.md), [practice](cpp_basics/module_08/practice_concepts.cpp))
- [x] **[Cumulative Practice]**: Modern STL (Ranges & Concepts) ([practice](cpp_basics/module_08/practice_cumulative_8_3.cpp)) ✅ 완료

#### MODULE 9: 모던 C++ (RAII & 효율성)
- [x] **스마트 포인터**: `unique_ptr`, `shared_ptr`, `weak_ptr` (메모리 직접 해제 금지)
- [x] **이동 시맨틱**: `std::move`와 R-value reference(`&&`)로 성능 최적화 (std::string 등 내부 자원 탈취 원리 포함)

- [x] **[Cumulative Practice]**: Smart Pointers & Move Semantics (범위: 스마트포인터/이동시맨틱 + [M1~M8] 각 모듈 최소 1개 필수) ([practice](cpp_basics/module_09/practice_cumulative_9_1.cpp))
- [x] **Type Inference**: `auto` & `decltype`으로 복잡한 타입 추론 ([concept](cpp_basics/module_09/concept_type_inference.md), [practice](cpp_basics/module_09/practice_type_inference.cpp))
- [x] **[C++17] Structured Binding**: `auto [key, val] = map` 구조 분해 ([concept](cpp_basics/module_09/concept_structured_binding.md), [practice](cpp_basics/module_09/practice_structured_binding.cpp))
- [x] **[Cumulative Practice]**: Type Inference & Structured Binding (범위: 타입추론/구조분해 + [현재 M9 과거 소제목 중 랜덤 1개] + [M1~M8] 각 모듈 최소 1개 필수) ([practice](cpp_basics/module_09/practice_cumulative_9_2.cpp))
- [x] **[C++17/23] Modern Types**: `std::optional`, `std::variant`, `std::expected` (에러 처리 트렌드) ([concept](cpp_basics/module_09/concept_modern_types.md), [practice](cpp_basics/module_09/practice_modern_types.cpp))
- [x] **[Cumulative Practice]**: Modern Error Handling & Types (범위: 근대화타입 + [현재 M9 과거 소제목 중 랜덤 1개] + [M1~M8] 각 모듈 최소 1개 필수) ([practice](cpp_basics/module_09/practice_cumulative_9_3.cpp)) ✅ 완료

#### MODULE 10: 프로페셔널 도구 (Toolchain & QA)
- [x] **[Cumulative Practice]**: Build System & Package Manager (범위: 환경세팅 + [M1~M9] 각 모듈 최소 1개 필수) ✅ [practice](cpp_basics/module_10/practice_cumulative_10_1.cpp)
- [x] **Profiling & Sanitizers**: AddressSanitizer(ASan)를 이용한 메모리 오염 실시간 탐지. ✅ [practice](cpp_basics/module_10/practice_json.cpp) (Vcpkg 연동 과정에서 간접 체험)
- [x] **단위 테스트(Unit Test)**: Google Test(GTest) 프레임워크를 활용한 자동화된 코드 검증 ([concept](cpp_basics/module_10/concept_unit_test.md), [manual_test](cpp_basics/module_10/practice_manual_test.cpp), [gtest](cpp_basics/module_10/practice_gtest.cpp))
- [x] **[Cumulative Practice]**: Debugging & Testing (M1~M10 통합 실습 완료!) ✅ 
  ([test_quest_journal.cpp](file:///c:/cpp/cpp_basics/module_10/test_quest_journal.cpp), [Quest.h](file:///c:/cpp/cpp_basics/module_10/Quest.h), [QuestJournal.h](file:///c:/cpp/cpp_basics/module_10/QuestJournal.h), [QuestJournal.cpp](file:///c:/cpp/cpp_basics/module_10/QuestJournal.cpp))
  - [concept_modern_enum.md](file:///c:/cpp/cpp_basics/module_10/concept_modern_enum.md)
  - [concept_abstract_class_advanced.md](file:///c:/cpp/cpp_basics/module_10/concept_abstract_class_advanced.md)
  - [concept_const_correctness.md](file:///c:/cpp/cpp_basics/module_10/concept_const_correctness.md)
- [x] **협업 기초**: .gitignore 작성 및 Git 버전 관리 전략. ✅ [concept](cpp_basics/module_10/concept_git_basics.md)
- [x] **[Cumulative Practice]**: Version Control & Collaboration (범위: Git/협업 + [현재 M10 과거 소제목 중 랜덤 1개] + [M1~M9] 각 모듈 최소 1개 필수) ✅ [practice](cpp_basics/module_10/test_commit_analyzer.cpp)

#### MODULE 11: [신규] 병렬 프로그래밍 (Concurrency)
- [x] **Thread 기초**: `std::thread`, `std::mutex`로 레이스 컨디션 이해하기 ([concept](cpp_basics/module_11/concept_thread_basics.md), [practice](cpp_basics/module_11/practice_threads.cpp))
- [ ] **[C++20] jthread**: 자동으로 Join되는 안전한 스레드 (`std::jthread`) 사용하기
- [ ] **[Cumulative Practice]**: Basic Concurrency (Thread & jthread) (범위: Thread/jthread + [M1~M10] 각 모듈 최소 1개 필수)

#### MODULE 12: [신규] 네트워크 프로토콜 기초 (TCP & UDP)
> 멀티스레드(M11)의 위력을 극대화할 수 있는 실습 무대. 윈도우 기반 소켓 프로그래밍 입문.
- [ ] **Winsock2 환경 세팅**: Windows API 설정 및 통신 흐름(IP/Port) 이해.
- [ ] **신뢰성의 TCP**: 스트림(Stream) 기반 연결형 통신 실습.
- [ ] **속도의 UDP**: 데이터그램(Datagram) 기반 비연결형 통신 실습.
- [ ] **[Cumulative Practice 1]**: TCP/UDP Game Data Sync (범위: TCP/UDP 소켓 통신 + [M1~M10] 몬스터/플레이어 직렬화 전송)

#### MODULE 13: [신규] 서버 아키텍처 기초 (Synchronous vs Asynchronous)
> 진짜 게임 서버의 기틀이 되는 동기식 스레드풀과 멀티플렉싱 비동기 I/O 모델 비교.
- [ ] **Thread Pool 모델 (동기식 다중 처리)**: 일꾼(Thread)들을 미리 만들어두고 클라이언트 요청을 Queue를 통해 효율적으로 처리하기 (M11 심화).
- [ ] **Selector 모델 (비동기 처리 기초)**: `select()` 함수를 활용한 Non-blocking 다중 I/O 처리 맛보기.
- [ ] **[Cumulative Practice 2]**: Hybrid Architecture Server (범위: 스레드풀/비동기 소켓 + 세션 컨텍스트 관리 + [M1~M10] 버무리기)

---

### 🏆 EXPERT 대단원 마무리: 최종 졸업 프로젝트 (Master Exam)
> **[M1 ~ M13 전 범위]**를 총망라하는 C++ 마스터 프로젝트.
> 클라이언트-서버 아키텍처 기반의 **'실시간 멀티스레드 터미널 로그라이크(Roguelike)'** 제작!

- [ ] **[Final Project]**: 무한 돌파형 랜덤 픽셀 던전 (Server-Client Terminal Game)
  - **서버-클라이언트 (M12~M13)**: 서버(월드 상태/방 구조 통제)와 터미널 클라이언트 통신 매칭.
  - **멀티스레드 기반 실시간 전투 (M11)**: 유저 상하좌우 이동 스레드와, 독립적으로 움직이며 1초 인접 시 데미지를 주는 '몬스터 자율 이동 스레드'의 병렬 실행 및 Mutex 자원 충돌 방어.
  - **모던 C++ & 객체지향 (M6, M7)**: 무기/방어구 다형성 구조, 스마트 포인터 인벤토리, 방어력 차감 공식(최소 1 데미지 보장).
  - **절차적 생성 및 루프 (M3, M8)**: 진입 시 3칸 랜덤 방 구조 생성, 보스 클리어 후 스탯 인플레이션(몹 데미지 증가 및 체력 보상) 무한 루프 구현.

---

_본 커리큘럼은 학습자의 성장 속도에 따라 유동적으로 변경될 수 있습니다._
