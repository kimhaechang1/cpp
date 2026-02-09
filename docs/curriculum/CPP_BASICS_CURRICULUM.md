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

### 🏗️ ADVANCED (고급)
**목표**: "설계"를 한다. 데이터와 기능을 묶어 관리하고, 상속을 통해 코드를 재사용한다.

#### MODULE 6: 클래스와 객체 (설계의 기초)
- [ ] **[신규] 네임스페이스 (Namespace)**: 이름 충돌 방지 및 `std::`의 의미 이해, `using` 문구의 올바른 사용법
- [ ] **클래스**: `struct`와 `class`의 차이, 접근 지정자 (`private`, `public`)
- [ ] **생성자/소멸자**: 객체의 탄생과 죽음 관리 (RAII)
- [ ] **[보충] 초기화 리스트**: 대입(`=`)이 아닌 리스트(`:`) 사용법 (성능 및 `const` 멤버)
- [ ] **복사 생성자**: 얕은 복사 vs 깊은 복사 (메모리 관리의 시작)
- [ ] **연산자 오버로딩**: `+`, `==`, `<<` 등 연산자 재정의 및 대입 연산자(`=`) 오버로딩
- [ ] **Friend 키워드**: 클래스 외부에서 `private` 멤버에 접근하는 예외적 허용
- [ ] **클래스의 정적 멤버**: `static` 변수/함수를 이용한 공유 데이터 관리
- [ ] **[심화] 3/5/0 규칙**: 직접 구현하기보다 `std::unique_ptr`를 활용해 컴파일러에게 맡기는(Rule of Zero) 패턴
- [ ] **[C++11~26] constexpr**: 런타임이 아닌 컴파일 타임에 값 계산하기 (성능 최적화)

#### MODULE 7: 상속과 다형성 (설계의 심화)
- [ ] **상속**: `Monster`를 상속받는 `Slime`, `Orc` 만들기
- [ ] **[보충] protected 접근 제어자**: 상속 관계에서만 허용되는 접근 범위
- [ ] **[추가] 다중 상속과 다이아몬드 문제**: `가상 상속(Virtual Inheritance)`을 통한 메모리 중복 해결
- [ ] **가상 함수**: `virtual Attack()`으로 다형성 구현하기
- [ ] **[보충] 가상 소멸자**: 부모 포인터로 자식 삭제 시 메모리 누수 방지 (필수)
- [ ] **[보충] override 키워드**: 실수 방지를 위한 명시적 오버라이딩
- [ ] **순수 가상 함수 & 추상 클래스**: 인스턴스화가 불가능한 인터페이스 설계법
- [ ] **[심화] 데이터 지향 설계(DOD) 맛보기**: `AoS`(구조체 배열) vs `SoA`(배열의 구조체)와 캐시 적중률(Cache Hit) 이해

---

### ✨ EXPERT (심화)
**목표**: 더 안전하고, 더 빠르고, 더 편리한 "모던 C++" 기법을 익힌다.

#### MODULE 8: 템플릿과 STL
- [ ] **템플릿**: 타입에 상관없는 `Box<T>` 만들기 (일반화 프로그래밍)
- [ ] **STL**: `std::vector`, `std::map` 등으로 인벤토리/타일맵 구현
- [ ] **반복자(Iterator)**: 컨테이너의 내부 구조를 몰라도 순회할 수 있는 인터페이스
- [ ] **함수 객체(Functor) & 람다**: 알고리즘 함수(`std::sort` 등)에 사용자 정의 로직 주입
- [ ] **[C++20] Ranges & Views**: 파이프(`|`)를 사용해 직관적인 데이터 변환 및 필터링 (`std::ranges::sort`)
- [ ] **[C++20] Concepts**: 템플릿의 `typename T`에 제약조건 걸기 (`Integral T`)

#### MODULE 9: 모던 C++ (RAII & 효율성)
- [ ] **스마트 포인터**: `unique_ptr`, `shared_ptr`, `weak_ptr` (메모리 직접 해제 금지)
- [ ] **이동 시맨틱**: `std::move`와 R-value reference(`&&`)로 성능 최적화
- [ ] **Type Inference**: `auto` & `decltype`으로 복잡한 타입 추론
- [ ] **[C++17] Structured Binding**: `auto [key, val] = map` 구조 분해
- [ ] **[C++17/23] Modern Types**: `std::optional`, `std::variant`, `std::expected` (에러 처리 트렌드)

#### MODULE 10: 프로페셔널 도구 (Toolchain & QA)
- [ ] **Build Systems**: CMake를 사용하여 멀티 플랫폼 프로젝트 구성하기.
- [ ] **Package Manager**: Vcpkg나 Conan을 통해 외부 라이브러리 연동하기.
- [ ] **Profiling & Sanitizers**: AddressSanitizer(ASan)를 이용한 메모리 오염 실시간 탐지.
- [ ] **단위 테스트(Unit Test)**: Google Test나 Catch2를 활용한 코드 검증.
- [ ] **협업 기초**: .gitignore 작성 및 Git 버전 관리 전략.

#### MODULE 11: [신규] 병렬 프로그래밍 (Concurrency)
- [ ] **Thread 기초**: `std::thread`, `std::mutex`로 레이스 컨디션 이해하기
- [ ] **[C++20] jthread**: 자동으로 Join되는 안전한 스레드 (`std::jthread`) 사용하기
