# Module 06: 클래스와 객체 (Class & Object)

이 폴더는 **객체지향 프로그래밍(OOP)**의 핵심인 클래스와 객체의 생명주기를 다룹니다.

## 📚 개념 학습 목차

1.  [**Namespace & Class Basics**](concept_class_basics.md)
    *   네임스페이스로 이름 충돌 방지하기
    *   구조체(`struct`)와 클래스(`class`)의 차이 (접근 지정자)
    *   캡슐화(Encapsulation)와 정보 은닉

2.  [**Life Cycle (Constructor & Destructor)**](concept_lifecycle_basics.md)
    *   생성자(Constructor): 객체가 태어날 때 (초기화)
    *   소멸자(Destructor): 객체가 죽을 때 (메모리 해제)
    *   초기화 리스트(Initializer List)의 중요성

3.  [**Copy Constructor (Deep vs Shallow)**](concept_copy_constructor.md)
    *   얕은 복사(Shallow Copy)의 위험성 (Double Free)
    *   깊은 복사(Deep Copy) 구현 방법
    *   Rule of Three (3의 법칙)

---

## 🛠️ 실습 파일 가이드
*   `practice_class_basics.cpp`: 클래스 선언과 접근 지정자 연습
*   `practice_lifecycle_basics.cpp`: 생성자와 소멸자 호출 시점 확인
*   `practice_cumulative_subtitle1_2.cpp`: **[종합]** 인벤토리 클래스 설계 (M1~M6 누적)
*   `practice_copy_constructor.cpp`: 복사 생성자와 메모리 크래시 체험
