# Module 08: Templates & STL (C++ Expert)

C++의 꽃이라 불리는 일반화 프로그래밍(Generic Programming)과 표준 템플릿 라이브러리(STL)를 학습합니다. 단순히 컨테이너의 이름을 외우는 것을 넘어, 현대 C++이 추구하는 효율적인 데이터 관리와 가공 기법(Modern STL)을 익히는 것이 목표입니다.

---

## 📚 학습 목차

### 1단계: 템플릿 기초 (Generics)
- **[Concept]** [Template Basics](concept_template_basics.md): 타입에 독립적인 클래스와 함수 설계법
- **[Practice]** `practice_templates.cpp`: `Box<T>` 등 제네릭 컨테이너 구현

### 2단계: STL 컨테이너 (Containers)
- **[Concept]** [STL Containers](concept_stl_containers.md): `vector`, `map`, `unordered_map`의 메모리 특징
- **[Practice]** `practice_stl_containers.cpp`: 게임 인벤토리 시스템 구현 기초

### 3단계: 순회와 가공 (Iteration & Operations)
- **[Concept]** [Iterators](concept_iterator.md): 컨테이너의 내부 구조를 몰라도 안전하게 훑는 법
- **[Practice]** `practice_iterator.cpp`: 반복자를 이용한 요소 검색 및 조작
- **[Concept]** [Lambda & Functor](concept_lambda.md): 이름 없는 1회용 로직 주입 (정렬, 조건부 삭제)
- **[Practice]** `practice_lambda.cpp`: 커스텀 정렬 알고리즘 설계

### 4단계: 모던 STL (Modern C++20)
- **[Concept]** [Ranges & Views](concept_ranges.md): 파이프(`|`) 연산자를 이용한 고성능 데이터 가공 **(진행 중)**
- **[Practice]** `practice_ranges.cpp`: 몬스터 리스트 필터링/트랜스폼 실습

---

## 🏆 통합 실습 (Cumulative Practice)
- **M8-1**: [Generics Integration](cumulative_practice_8_1.cpp)
- **M8-2**: [Full Iteration & Ownership](practice_cumulative_8_2.cpp) ✅ 완료

---

## ⚠️ 주의 사항
- **Blank Slate Policy**: 실습 시 `void Func()` 등의 원형을 제공하지 않으므로 직접 설계해야 합니다.
- **Game Dev Style**: 모든 변수명과 함수명은 게임 개발 컨벤션(CamelCase, PascalCase)을 따릅니다.
