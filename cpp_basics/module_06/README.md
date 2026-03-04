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

4.  [**Operator Overloading (연산자 오버로딩)**](concept_operator_overloading.md)
    *   클래스 객체 간의 덧셈(`+`), 비교(`==`), 입출력(`<<`) 허용
    *   자가 대입(Self-Assignment) 방지 전략

5.  [**Friend Keyword (프렌드)**](concept_friend_keyword.md)
    *   캡슐화를 깨는 예외적 접근 허가 (Backdoor)
    *   연산자 오버로딩 및 팩토리 패턴에서의 활용

6.  [**Static Members (정적 멤버)**](concept_static_members.md)
    *   모든 객체가 공유하는 전역 데이터
    *   `static` 변수의 메모리 구조와 할당 방식

---

## 🛠️ 실습 파일 가이드
*   `practice_class_basics.cpp`: 클래스 선언과 접근 지정자 연습
*   `practice_lifecycle_basics.cpp`: 생성자와 소멸자 호출 시점 확인
*   `practice_cumulative_subtitle1_2.cpp`: **[종합]** 인벤토리 클래스 설계 (M1~M6 누적)
*   `practice_copy_constructor.cpp`: 복사 생성자와 메모리 크래시 체험
*   `practice_operator_overloading.cpp`: 대입 연산자와 각종 연산자 재정의
*   `practice_cumulative_subtitle3_4.cpp`: **[종합]** PlayerInventory 클래스 구현 (Blank Slate, Write-After-Free 디버깅)
