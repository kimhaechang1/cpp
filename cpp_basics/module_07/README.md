# Module 07: 상속과 다형성 (Inheritance & Polymorphism)

이 폴더는 C++의 핵심인 객체지향 설계의 심화 개념들을 다룹니다. 부모 클래스의 기능을 물려받고, 런타임에 다양한 형태로 동작하는 다형성을 학습합니다.

## 📖 학습 목차 (Concepts)

| 번호 | 문서명 | 주요 내용 |
| :--- | :--- | :--- |
| 1 | [concept_inheritance.md](concept_inheritance.md) | 상속 기초: 부모-자식 관계, 생성자 호출 순서 |
| 2 | [concept_protected.md](concept_protected.md) | protected 접근 제어자: 상속 계층에서의 캡슐화 |
| 3 | [concept_multiple_inheritance.md](concept_multiple_inheritance.md) | 다중 상속과 가상 상속: 다이아몬드 문제 해결 |
| 4 | [concept_polymorphism_and_virtual.md](concept_polymorphism_and_virtual.md) | 가상 함수와 다형성: vptr, vtable, 동적 바인딩 |
| 5 | [concept_virtual_destructor.md](concept_virtual_destructor.md) | 가상 소멸자: 부모 포인터 삭제 시 메모리 누수 방지 |
| 6 | [concept_abstract_class.md](concept_abstract_class.md) | 추상 클래스와 인터페이스: 순수 가상 함수 설계 |
| 7 | [concept_object_slicing.md](concept_object_slicing.md) | **[중요] 객체 슬라이싱**: 다형성 객체 복사 금지 원칙 |
| 8 | [concept_dod_basics.md](concept_dod_basics.md) | 데이터 지향 설계(DOD): AoS vs SoA 성능 차이 |
| 9 | [concept_casting.md](concept_casting.md) | 상속 계층에서의 캐스팅: static/dynamic_cast |

---

## 🏗️ 실습 과제 (Practice)

- `practice_inheritance.cpp`
- `practice_protected.cpp`
- `practice_multiple_inheritance.cpp`
- `practice_polymorphism.cpp`
- `practice_virtual_destructor.cpp`
- `practice_abstract_class.cpp`

## 🏆 대단원 마무리

- **[Cumulative M7-1]**: 상속 ~ protected 통합 실습
- **[Cumulative M7-2]**: 다중상속 ~ 다형성 통합 실습
- **[Cumulative M7-3]**: 추상 클래스 & 다형성 안전성 실습
- **[Mini-Project]**: OOP RPG 전투 시스템
