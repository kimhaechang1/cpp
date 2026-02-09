# Module 06: 클래스와 객체 (Subtitle 1: 기본기)

## 1. 네임스페이스 (Namespace): 이름표 붙이기
협업을 하다 보면 `Monster`라는 이름이 겹칠 수 있습니다. (전투 팀의 Monster, UI 팀의 Monster...)
이때 **"누구네 Monster인지"** 성(Last Name)을 붙여주는 것이 네임스페이스입니다.

```cpp
namespace Battle {
    struct Monster { int hp; };
}

namespace UI {
    struct Monster { int x, y; };
}

int main() {
    Battle::Monster m1; // 전투 팀 몬스터
    UI::Monster m2;     // UI 팀 몬스터
}
```

### 1.1 `using namespace std;`의 진실
*   `std::cou`t은 "Standard(표준) 네임스페이스에 있는 cout"이라는 뜻입니다.
*   `using namespace std;`를 쓰면 `Battle::`, `UI::` 없이 그냥 이름을 부르겠다는 뜻인데, **실무에서는 이름 충돌 위험 때문에 권장하지 않습니다.** (특히 헤더 파일에서는 절대 금지!)
*   **대안**: `using std::cout;` `using std::string;` 처럼 필요한 것만 콕 집어서 여는 것이 좋습니다.

---

## 2. 구조체(`struct`) vs 클래스(`class`)
C++에서 둘은 기능적으로 **99% 똑같습니다.**
단 하나의 차이점은 **"접근 지정자(Access Modifier)의 기본값"**입니다.

### 2.1 접근 지정자 (공개 범위)
*   **`public`**: "누구나 만져도 돼!" (개방)
*   **`private`**: "나(클래스 내부)만 만질 거야!" (폐쇄)

### 2.2 차이점
*   **`struct`**: 기본인 **`public`** (모두 공개) -> 데이터 묶음(DTO) 용도로 주로 사용
*   **`class`**: 기본이 **`private`** (모두 비공개) -> **캡슐화(Encapsulation)**를 통한 설계 용도로 사용

```cpp
struct Data {
    int a; // public (기본값)
};

class Logic {
    int b; // private (기본값) -> 외부에서 접근 불가!
public:
    void SetB(int v) { b = v; } // public 구역을 따로 열어줘야 함
};
```

---

## 3. 왜 `private`을 써야 하나요? (캡슐화의 이유)
**"데이터를 함부로 건드리지 못하게 하고, 반드시 '함수'를 통해서만 조작하게 하기 위해서"**입니다.
이것이 **객체지향 프로그래밍(OOP)**의 시작입니다.

*   **나쁜 예**: `player.hp = -9999;` (외부에서 말도 안 되는 값 대입 가능)
*   **좋은 예**: `player.TakeDamage(9999);` (함수 내부에서 `hp < 0`이면 `0`으로 보정 가능)

> **Agent's Rule**: 앞으로 모든 클래스의 **멤버 변수(데이터)**는 원칙적으로 **`private`**입니다.
