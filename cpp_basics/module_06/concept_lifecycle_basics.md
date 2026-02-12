# Module 06-2: 객체의 삶과 죽음 (생성자와 소멸자)

## 1. `Init()`의 문제점
지금까지 우리는 객체를 만들고 나서 **반드시** `Init()`을 호출해야 했습니다.
```cpp
Player p;
// 만약 여기서 Init을 깜빡한다면?
// p.hp는 쓰레기값(Garbage Value)을 가지게 되고, 버그가 발생합니다! 😱
p.Attack(); 
```

## 2. 생성자 (Constructor)
**"객체가 태어나는 순간(선언) 자동으로 호출되는 특수 함수"**입니다.
*   **이름**: 클래스 이름과 **똑같아야** 합니다.
*   **반환형**: **없습니다** (void도 안 붙임).

```cpp
class Player {
public:
    // [1] 기본 생성자 (Default Constructor)
    // Player p; 라고 선언할 때 호출됨
    Player() {
        hp = 100;
        std::cout << "Player 생성됨!\n";
    }

    // [2] 매개변수 생성자 (Parameterized Constructor)
    // Player p("Leon", 200); 라고 선언할 때 호출됨
    Player(std::string n, int h) {
        name = n;
        hp = h;
    }
};
```

### 2.1 생성자 초기화 리스트 (Initializer List) ⭐
생성자 몸통(`{}`)에서 대입(`=`)하는 것보다, **초기화 리스트(`:`)**를 쓰는 것이 **더 빠르고(성능), 더 강력(const 초기화 가능)**합니다.
```cpp
// [권장 스타일]
Player(std::string n, int h) 
    : name(n), hp(h) // 변수(값), 변수(값) 순서로 초기화
{
    // 몸통은 비워두거나 로직만 넣음
}
```

---

## 3. 소멸자 (Destructor)
**"객체가 사라지는 순간(죽음) 자동으로 호출되는 특수 함수"**입니다.
*   **이름**: 클래스 이름 앞에 **`~` (틸드)**를 붙입니다. (`~Player`)
*   **매개변수/반환형**: **없습니다** (죽을 때는 유언만 남길 뿐...)

```cpp
class Player {
public:
    ~Player() {
        std::cout << name << " 떠납니다...\n";
        // 나중에 여기서 동적 할당 해제(delete)를 하게 됩니다. (중요!)
    }
};
```

---

## 4. 객체의 수명 (Life Cycle)
변수가 선언된 **블록(`{}`)이 끝날 때**, 스택(Stack)에서 사라지며 **소멸자**가 호출됩니다.

```cpp
int main() {
    {
        Player p("Leon", 100); // [생성자 호출]
        p.Attack();
    } // 블록 끝 -> [소멸자 호출] 및 메모리 해제

    std::cout << "이 메시지는 소멸자 뒤에 출력됩니다.\n";
}
```

## 5. [Q&A] 궁금해요! (FAQ)

**Q1. 동적 할당(`new`)한 객체를 `delete`하면 소멸자가 호출되나요?**
*   **네, 무조건 호출됩니다!** (가장 중요한 특징)
*   `delete p;`를 실행하면 컴퓨터는 두 가지 일을 순서대로 합니다.
    1.  **`p->~Player()` 호출**: 유언을 남기고 뒷정리할 기회를 줌.
    2.  **메모리 해제**: `free(p)`처럼 힙 메모리를 반환함.

**Q2. "초기화 리스트"는 자료구조(List)인가요?**
*   아니요! 그냥 **"초기화할 멤버들의 목록(Listing)"**이라는 뜻입니다.
*   링크드 리스트 같은 자료구조가 아니라, 단순히 "이거랑, 이거랑, 이거를 초기화해라"라고 **나열(List)**해둔 것입니다.

**Q3. `name(n)`, `hp(h)`는 함수 호출인가요?**
*   **생성자 호출**입니다. `std::string` 같은 클래스는 이미 누군가(표준 라이브러리 제작자)가 `string(const char*)` 같은 생성자를 만들어 둔 것입니다.
*   `int` 같은 기본 타입도 C++에서는 생성자처럼 초기화(`hp(h)`)할 수 있게 문법이 지원됩니다.
*   **주의**: 이 문법은 오직 **초기화 리스트**나 **변수 선언 시**에만 쓸 수 있습니다. 나중에 `hp(10);`처럼 함수 부르듯 쓸 수는 없습니다!

---

## 6. 접근 지정자 (Access Modifiers) 완벽 정리
Java와 비슷하지만, **패키지(Package)** 개념이 없어서 `default`가 없습니다.

1.  **`public` (공개)**: "누구나" 접근 가능 (외부, 내부, 자식)
2.  **`protected` (보호)**: "나와 내 자식(상속)"만 접근 가능 (외부 불가, 상속 배울 때 중요!)
3.  **`private` (비공개)**: "나 혼자"만 접근 가능 (외부 불가, 자식도 불가!)
    *   **Class의 기본값입니다.** (Struct는 public이 기본)

### 6.1 생성자도 `private`이 되나요?
**네, 가능합니다!** (고급 기법)
생성자를 `private`으로 숨기면, **외부에서 객체를 마음대로 생성(`new Player`)할 수 없게 됩니다.**

*   **용도**:
    *   **싱글톤 패턴 (Singleton)**: 객체를 딱 하나만 만들어서 관리하고 싶을 때.
    *   **정적 팩토리 메서드**: 객체 생성을 `Create()` 같은 특정 함수로만 제한하고 싶을 때.
    
```cpp
class Admin {
private:
    Admin() {} // private 생성자! (외부에서 Admin a; 불가능)
public:
    static Admin& GetInstance() { // 이 함수로만 접근 가능
        static Admin instance;
        return instance;
    }
};
```

### 6.2 [Q&A] 파이썬처럼 이름으로 값 넣기 (Kwargs)?
**Q. `Knight(hp=100, name="Leon")` 처럼 순서 상관없이 이름으로 넣을 수 있나요?**
*   **아쉽게도 C++ 함수에는 그런 기능(Named Parameter)이 없습니다.** 😅
*   무조건 **순서**대로 넣어야 합니다. (`Knight("Leon", 100)`)
*   **대안 (C++20 Designated Initializers)**:
    *   `struct`에 한해서는 비슷하게 쓸 수 있습니다.
    *   `struct Config { int a; int b; };`
    *   `Config c = { .a = 10, .b = 20 };` (순서는 지켜야 함)
*   **대안 (Builder 패턴)**:
    *   `Knight().SetName("Leon").SetHp(100);` 처럼 체이닝하는 방식을 직접 구현해서 쓰기도 합니다.

> **RAII (Resource Acquisition Is Initialization)**: "자원 획득은 초기화다"라는 뜻으로, **생성자에서 자원을 얻고 소멸자에서 자원을 반납하는** C++의 핵심 철학입니다.
