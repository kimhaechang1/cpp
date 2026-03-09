# Module 7: 개념 1 - 상속 (Inheritance)

## 📖 1. 왜 상속을 배워야 할까? (실무 프로젝트 관점)
우리가 이전에 `Project: 텍스트 RPG`를 진행하면서, 몬스터나 캐릭터의 정보를 구조체(`struct`)로 만들고, 클래스(`class`)로 발전시켰습니다.
만약 우리 게임에 **슬라임(Slime)**, **오크(Orc)**, **드래곤(Dragon)** 이라는 세 종류의 몬스터가 있다고 가정해 보겠습니다.

상속을 모른다면 코드를 이렇게 작성하게 됩니다:

```cpp
class Slime {
public:
    int hp;
    int attack;
    int stickiness; // 슬라임만의 고유 속성 (끈적임)

    void TakeDamage(int damage) { hp -= damage; }
};

class Orc {
public:
    int hp;
    int attack;
    int rage; // 오크만의 고유 속성 (분노)

    void TakeDamage(int damage) { hp -= damage; }
};
```
보시다시피 `hp`, `attack`, `TakeDamage()` 같은 핵심 데이터와 기능들이 **똑같이 중복**되고 있습니다.
만약 방어력(`defense`)을 추가한다면 모든 몬스터 클래스를 찾아다니며 하나씩 추가해야 합니다. 유지보수가 굉장히 힘들어지죠!

이러한 **중복 코드를 제거**하고, **"A는 B의 일종이다 (A is a B)"** 라는 관계를 프로그래밍으로 표현하는 것이 바로 **상속(Inheritance)** 입니다.

---

## 🏗️ 2. 상속의 기본 문법

공통되는 속성(`hp`, `attack`)은 **부모 클래스 (Base Class)** 에 모아두고, 
개별적인 특성(`stickiness`, `rage`)만 **자식 클래스 (Derived Class)** 에서 구현합니다.

```cpp
#include <iostream>
#include <string>

// 1. 공통점만 모아놓은 "부모 클래스" (Base Class)
class Monster {
public: // (일단 모두 public으로 진행)
    int hp;
    int attack;

    Monster(int hp, int attack) : hp{hp}, attack{attack} {
        std::cout << "[Monster] 생성 완료\n";
    }

    void TakeDamage(int damage) {
        hp -= damage;
        std::cout << "데미지를 입었습니다. 남은 HP: " << hp << "\n";
    }
};

// 2. Monster를 상속받는 "자식 클래스" (Derived Class)
// "Slime은 Monster의 일종이다"
class Slime : public Monster {
public:
    int stickiness;

    // 부모의 생성자를 호출(: Monster)하여 공통 변수 초기화
    Slime(int hp, int attack, int stickiness)
        : Monster{hp, attack}, stickiness{stickiness} {
        std::cout << "[Slime] 생성 완료 (끈적임: " << stickiness << ")\n";
    }

    void SplashAttack() {
        std::cout << "끈적거리는 공격!\n";
    }
};
```

*   `class Slime : public Monster` 👉 "Slime 클래스를 만들 건데, Monster 클래스가 가진 모든 데이터와 기능을 물려받겠습니다." 라는 뜻입니다.

---

## 🔬 3. 메모리 검증코드 (HW 레벨에서 상속 확인하기)
상속을 받으면 메모리 구조는 어떻게 될까요?
물리적으로 볼 때, **자식 객체의 메모리는 [부모 객체의 데이터] + [자신의 데이터]가 결합된 형태**로 만들어집니다. 눈으로 직접 확인해 봅시다.

```cpp
#include <iostream>

class Monster {
public:
    int hp;      // 4 bytes
    int attack;  // 4 bytes
};

class Slime : public Monster {
public:
    int stickiness; // 4 bytes
};

class Orc : public Monster {
public:
    int rage;      // 4 bytes
};

int main() {
    Monster m;
    Slime s;

    std::cout << "부모(Monster)의 크기: " << sizeof(m) << " bytes\n";   
    std::cout << "자식(Slime)의 크기: " << sizeof(s) << " bytes\n";     

    // 메모리 주소 확인
    std::cout << "&s (Slime 시작 주소): " << &s << "\n";
    std::cout << "&s.hp (물려받은 hp 소): " << &s.hp << "\n";
    std::cout << "&s.attack (물려받은 attack 주소): " << &s.attack << "\n";
    std::cout << "&s.stickiness (고유 속성 주소): " << &s.stickiness << "\n";

    return 0;
}
```

*   `sizeof(m)`은 `hp`와 `attack`을 더해 **8바이트**가 됩니다.
*   `sizeof(s)`는 부모에게 물려받은 8바이트에 자신의 `stickiness` 4바이트가 더해져 **12바이트**의 메모리를 가지게 됩니다.
*   **[심화 예고]** 객체가 메모리에 연속적으로 배치되는 특성은 추후 **Data Oriented Design (Module 7 후반부)** 에서 캐시 성능을 극대화할 때 매우 중요해집니다!

---

## ⚠️ 4. 주의 사항 및 제약
1. 자식은 부모의 모든 것을 물려받지만, **생성자와 소멸자**는 그대로 물려받지 않습니다. (자식이 직접 부모의 생성자를 호출해주는 초기화 리스트 `: Monster{...}` 문법을 사용해야 합니다.)
2. `public` 상속 외에도 `private`, `protected` 상속이 있지만, **실무의 99%는 `public` 상속(IS-A 관계)**만을 사용합니다.

---

### 💡 Q&A: 부모 클래스의 생성자 호출과 순서 (Java vs C++)

**Q. 자식 클래스에서 부모 클래스의 생성자를 호출하지 않으면 어떻게 되나요? 그리고 호출 순서는 상관없나요? (Java의 `super()`와 비슷하네요!)**

정말 예리하고 훌륭한 질문입니다! 실무 면접에서도 단골로 나오는 아주 중요한 개념입니다. Java의 경험과 비교해서 생각하신 점이 매우 좋습니다. 결론부터 말씀드리면 다음과 같습니다.

#### 1. 명시적으로 호출하지 않으면: "자동으로 기본 생성자(Default Constructor)를 호출합니다."
Java에서 명시적으로 `super()`를 적지 않아도 부모의 기본 생성자가 호출되는 것과 **완벽히 동일**합니다.
만약 자식 생성자에서 부모 생성자를 부르지 않으면, C++ 컴파일러는 몰래 부모의 **기본 생성자(인자가 없는 생성자)** 를 호출하려고 시도합니다.

```cpp
class Monster {
public:
    int hp;
    // 기본 생성자 (매개변수 없음)
    Monster() { std::cout << "Monster 기본 생성자 호출!\n"; } 
    Monster(int hp) : hp{hp} {}
};

class Slime : public Monster {
public:
    int stickiness;
    
    // 부모 생성자를 명시적으로 호출하지 않음!
    // -> 컴파일러가 몰래 Monster()를 먼저 호출합니다.
    Slime(int stickiness) : stickiness{stickiness} { 
        std::cout << "Slime 생성자 호출!\n";
    }
};
```
> **🚨 주의:** 만약 부모 클래스에 `Monster(int hp)`처럼 매개변수 있는 생성자만 있고 기본 생성자가 없다면? -> **컴파일 에러가 발생합니다.** 부모를 어떻게 초기화해야 할지 컴파일러가 모르기 때문입니다.

#### 2. C++에서 부모 생성자 호출 위치 (Java의 `super()` 대체제)
Java는 생성자 본문 `{...}` 안에서 가장 첫 줄에 `super()`를 적어야 하죠.
**C++은 본문 `{...}` 안에서 부모 생성자를 부를 수 없습니다.** 대신, 우리가 이전에 배웠던 **[초기화 리스트 (Initializer List)]** 를 사용해야만 합니다.

```cpp
// C++의 방식: 초기화 리스트(:)를 사용합니다.
Slime(int hp, int stickiness) : Monster{hp}, stickiness{stickiness} {
    // 본문 진입 전(여기 도달하기 전)에 부모(Monster)가 이미 메모리에 완성되어 있습니다.
}
```

#### 3. C++의 생성 순서 (순서는 무조건 고정되어 있습니다!)
C++에서는 개발자가 초기화 리스트에 이름을 적는 순서와 상관없이, **물리적인 메모리가 지어지는 순서는 언제나 동일하게 강제**됩니다.

**🏗️ 객체 생성 순서 (항상 이 순서를 따릅니다)**
1. **부모 클래스(Base)** 의 생성자가 **가장 먼저** 실행됩니다. (기반 공사)
2. **자식 클래스의 멤버 변수**들이 선언된 순서대로 초기화됩니다. (벽 세우기)
3. **자식 클래스의 생성자 본문 `{...}`** 이 마지막에 실행됩니다. (인테리어)

**즉, C++에서도 언제나 "부모가 먼저 태어난다"는 대원칙은 변하지 않습니다.** 

---

**Q. "초기화 리스트가 구현되기 이전에는 어떻게 호출했어?"**

**A. 사실 C++의 역사 초기부터 '초기화 리스트' 문법은 존재했습니다!**
C++의 창시자인 비야네 스트로우스트룹(Bjarne Stroustrup)이 언어를 설계할 때부터 의도적으로 만든 문법입니다. 
만약 Java처럼 생성자 본문 `{...}` 안에서 부모 생성자를 부르도록 허락했다면, 메모리 관점에서 심각한 **비효율성(Overhead)** 이 발생하기 때문입니다.

*   본문 `{...}`에 진입했다는 것 = 이미 부모 객체와 멤버 변수들이 쓰레기값(또는 기본값)으로 메모리에 생성 완료되었다는 뜻입니다.
*   만약 본문 안에서 초기화하려고 한다면? 생성을 한 번 하고, 거기에 다시 값을 **'대입(Assignment)'** 하는 2중 작업이 발생합니다.
*   C++은 태생부터 **"생성됨과 동시에 완벽히 초기화되어야 한다"**는 철학을 가졌기에, 본문 진입 전 단계인 **초기화 리스트** 자리에서만 부모 생성자를 부르도록 처음부터 설계되었습니다.

---

**Q. "부모 기본 생성자가 있긴 하지만 `private` 이면 어떡함?"**

**A. 자식 객체를 생성하려 할 때 즉시 '컴파일 에러(접근 불가)'가 발생합니다!**
자식 클래스는 이 세상에 태어나려면 **반드시** 부모의 생성자를 호출해서 기초 공사를 마쳐야 합니다. 그런데 부모의 생성자가 `private`이다?

이는 부모 클래스가 시스템에 이렇게 선언한 것과 같습니다.
**"나를 외부에서 마음대로 생성하는 것도 금지하고, 심지어 자식들이 나를 상속받아 태어나는 것조차 금지하겠다!"**

`private`은 오직 '나 자신(자기 클래스 내부)' 에서만 접근할 수 있기 때문에, 자식 클래스조차 부모 생성자에 접근(호출)할 권한이 없습니다. 따라서 기초 공사 실패로 자식 객체는 생성될 수 없습니다.
*(💡 참고: 만약 외부에서 생성하는 것은 막고 싶지만, **자식들이 상속받는 것은 허락하고 싶다면** 부모의 생성자를 `protected`로 설정하면 됩니다!)*

---

### ❓ 다음 단계
부모-자식 간의 생성자 호출 순서는 결국 **"기초 공사(부모)가 튼튼해야 그 위(자식)에 집을 지을 수 있다"** 는 메모리 원칙에서 나옵니다. 

이 개념이 충분히 이해되셨다면, **"실습 진행"** 이라고 말씀해 주세요. 방금 배운 상속(클래스 분리)과 부모 생성자 호출 방식을 직접 코딩해보는 **`practice_inheritance.cpp`** 실습 파일을 드리겠습니다!
