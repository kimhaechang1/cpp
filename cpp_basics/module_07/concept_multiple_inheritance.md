# Module 07. 다중 상속과 다이아몬드 문제 (Multiple Inheritance & Diamond Problem)

## 1. 다중 상속 (Multiple Inheritance) 이란?

C++의 가장 큰 특징 중 하나는 **한 클래스가 여러 부모 클래스를 동시에 상속받을 수 있다**는 점입니다. (참고로 Java나 C# 같은 언어에서는 상태를 가지는 일반 클래스의 다중 상속을 엄격히 금지합니다.)

> **왜 사용할까요? (실무 프로젝트 상황)**
> MMO RPG 게임을 개발 중입니다. 기본 직업으로 `Warrior`(전사)와 `Mage`(마법사)가 있습니다.
> 새로운 하이브리드 직업인 **`MagicSwordsman`(마검사)**를 기획하게 되었습니다. 기획팀의 요구대로라면, 마검사는 전사의 강력한 근접 공격력과 마법사의 넓은 광역 마법을 모두 사용할 수 있어야 합니다. 
> 이때, `Warrior`와 `Mage` 두 클래스를 **동시에 상속**받으면 쉽게 구현할 수 있지 않을까요?

```cpp
class Warrior {
public:
    void Slash() { /* 베기 공격 */ }
};

class Mage {
public:
    void CastMeteor() { /* 메테오 마법 */ }
};

// 다중 상속: 콤마(,)를 연달아 써서 여러 부모를 상속받습니다.
class MagicSwordsman : public Warrior, public Mage {
    // 이제 Slash()와 CastMeteor()를 모두 사용할 수 있습니다!
};
```

편리해 보이죠? 하지만 여기서 **C++ 역사상 가장 악명 높은 문제** 중 하나가 탄생합니다.

---

## 2. 다이아몬드 문제 (The Diamond Problem)

모든 직업은 결국 하나의 `Character`라는 최상위 부모를 상속받게 마련입니다. 
`Character`에는 `hp`(체력) 같은 공통 상태 데이터가 들어있겠죠?

구조를 그려보면 다음과 같습니다:
```text
      Character (hp)
       /       \
      /         \
  Warrior      Mage
     \          /
      \        /
  MagicSwordsman
```
클래스 상속 다이어그램이 **다이아몬드(◇) 모양**이 되었습니다! 이것이 바로 **다이아몬드 문제**입니다.

### 🚨 무엇이 문제일까요?

#### 문제점 1. 이름의 모호성 (Ambiguity)
마검사 객체를 만들고 `hp`를 깎으려고 합니다.
```cpp
MagicSwordsman player;
player.hp -= 10; // ❌ 컴파일 에러 발생!
```
컴파일러 입장에서는 패닉이 옵니다. *"잠깐만, 이 `hp`가 `Warrior`를 통해 물려받은 `hp`인가, 아니면 `Mage`를 통해 물려받은 `hp`인가?"* 
어느 쪽 부모의 변수인지 알 수 없어서 컴파일 오류를 뱉어냅니다. (`player.Warrior::hp -= 10` 처럼 명시해야만 쓸 수 있습니다)

#### 문제점 2. 메모리 중복 (Hardware Verification)
더 큰 문제는 눈에 보이지 않는 **메모리** 쪽에 있습니다. 코드로 직접 확인해 봅시다!

```cpp
#include <iostream>

class Character {
public:
    int hp;
};

class Warrior : public Character {
    int rage; // 전사 전용 자원
};

class Mage : public Character {
    int mana; // 마법사 전용 자원
};

class MagicSwordsman : public Warrior, public Mage {
    // 텅 빈 클래스
};

int main() {
    std::cout << "Character 크기: " << sizeof(Character) << " byte" << '\n';
    std::cout << "Warrior 크기: " << sizeof(Warrior) << " byte" << '\n';
    std::cout << "Mage 크기: " << sizeof(Mage) << " byte" << '\n';
    std::cout << "MagicSwordsman 크기: " << sizeof(MagicSwordsman) << " byte" << '\n';

    return 0;
}
```
**[예상되는 출력 결과]**
- Character: 4 byte (int hp)
- Warrior: 8 byte (Character의 hp 4 + 자신의 rage 4)
- Mage: 8 byte (Character의 hp 4 + 자신의 mana 4)
- **MagicSwordsman: 16 byte** (Warrior의 8 + Mage의 8)

결과가 이상하지 않나요? `MagicSwordsman` 하나를 만들었는데, 그 안에는 **`Character` 객체가 2개 생성**되어 버렸습니다! (체력이 두 개 존재하는 기괴한 마검사가 된 것입니다). 이는 심각한 **메모리 낭비**이자 **논리적 오류**입니다.

---

## 3. 해결책: 가상 상속 (Virtual Inheritance)

C++은 이 다이아몬드 문제를 해결하기 위해 **가상 상속(`virtual` 키워드)** 이라는 문법을 제공합니다.
상속을 받을 때, 부모 클래스 이름 앞에 `virtual`을 붙여주면 됩니다.

> *"이 클래스를 상속받을 때, 나중에 다이아몬드 구조로 합쳐지더라도 최상위 부모는 **메모리 상에 단 1개만** 유지해라!"* 라는 명령입니다.

```cpp
class Character {
public:
    int hp;
};

// Character를 가상 상속받습니다.
class Warrior : virtual public Character {
    int rage; 
};

// Character를 가상 상속받습니다.
class Mage : virtual public Character {
    int mana; 
};

// 여기는 그대로 둡니다.
class MagicSwordsman : public Warrior, public Mage {
};
```

### 🔍 하드웨어 레벨의 비밀 (어떻게 중복을 막았을까?)
`virtual` 상속을 적용한 뒤 `sizeof(Warrior)` 구조를 찍어보면 8바이트가 아니라 **더 커져 있습니다** (보통 64비트 운영체제 기준 16바이트로 늘어남).

왜 체력(4 byte) + 분노(4 byte)인데 16바이트가 될까요? 
컴파일러가 몰래 **`vbptr` (Virtual Base Pointer)** 라는 '포인터(주소값)'를 하나 끼워넣기 때문입니다. 이 포인터는 *"진짜 공통 부모(Character)의 데이터가 메모리 어딘가에 딱 1개 있으니까 거기로 찾아가라"* 하고 방향을 알려주는 역할을 합니다.

결과적으로, `MagicSwordsman`의 크기는 최상위 Character의 변수 1개분만 공유하여 메모리 중복을 피할 수 있게 되고, `player.hp -= 10;` 도 모호성 없이 하나의 `hp`를 향해 안전하게 조작될 수 있습니다.

> 🙋‍♂️ **Q. `vbptr`이라는 건 C++의 키워드(예: `virtual`, `class` 등) 인가요?**
> **A. 아닙니다! `vbptr`은 우리가 코드에 직접 치는 예약어(Keyword)가 아닙니다.**
> 프로그래머가 `virtual` 상속을 사용하면, **컴파일러가 우리 몰래 백그라운드에서 클래스 메모리 안에 삽입해 놓는 '포인터(변수)'**의 이름(개념적 명칭)입니다. (`Virtual Base Pointer`의 약자)
> 우리가 `int *p`를 만들듯이 컴파일러가 알아서 포인터를 하나 추가해버리기 때문에 `sizeof`를 찍어보면 우리가 선언하지도 않은 8바이트(64비트 기준 포인터 크기)가 추가되어 있는 것을 눈으로 확인할 수 있습니다.

> 🙋‍♂️ **Q. 만약 양쪽 다 말고, 한쪽(예: Warrior)에만 `virtual` 상속을 붙이면 어떻게 되나요?**
> **A. 다이아몬드 문제는 전혀 해결되지 않고, 여전히 2개의 부모(Character)가 메모리에 생성됩니다!**
> - **Warrior (virtual):** "내 `Character` 부모는 가상이니까, 나중에 다른 직업이랑 합쳐지면 1개로 통일하자!" 하고 준비를 해 둡니다. (`vbptr` 생성)
> - **Mage (일반 상속):** `virtual`을 안 썼기 때문에, "난 통일할 생각 없고, 내 몫의 `Character`를 통째로 만들 거야!" 하고 고집을 부리게 됩니다.
> - **결과:** 마검사(`MagicSwordsman`) 안에는 Warrior가 준비한 "공용 Character" 1개와, Mage가 만들어버린 "독점 Character" 1개, **총 2개의 `hp`가 여전히 존재하게 됩니다.** 양쪽 모두에서 합의(`virtual`)가 이루어져야만 메모리가 완전히 하나로 합쳐질 수 있습니다!

> 🙋‍♂️ **Q. 컴파일러가 알아서 강제해주면 좋을 텐데, 결국 개발자가 조심해야 하는 부분인가요?**
> **A. 빙고! 정확합니다. C++의 기본 철학("네가 뭘 하는지 안다면, 막지 않겠다") 때문입니다.**
> C++ 컴파일러는 `Mage`가 고집부려서 메모리를 따로 잡는 행위를 "오류"로 보지 않습니다. *"개발자가 의도적으로 `Mage` 전용의 분리된 `Character` 상태를 유지하고 싶었나 보지?"* 라고 추측하고 그대로 컴파일해 버립니다.
> 바로 이런 골치 아픈 점 때문에 현업(모던 C++)에서는 **"상태(변수)를 가진 클래스의 다중 상속은 그냥 하지 마라!"**가 국룰이 되었습니다. 개발자가 일일이 `virtual`을 신경 쓰며 상속도를 짜느니, 차라리 **인터페이스(변수 없는 뼈대)**만 다중 상속받거나 포함(Composition) 관계로 우회하는 것이 훨씬 안전하기 때문입니다.

> 🙋‍♂️ **Q. 마지막으로 궁금한 건, `virtual`이라는 키워드가 다른 의미로도 쓰이나요?**
> **A. 네! 사실 C++에서 `virtual`의 "진짜 주 무대"는 상속이 아니라 **함수(Function)** 앞입니다!**
> 곧이어서 배울 **'다형성(Polymorphism)'**의 핵심이 바로 **`virtual function`(가상 함수)** 입니다.
> 부모 클래스의 함수 앞에 `virtual`을 붙이면, **"자식 클래스야, 네가 이 함수를 너만의 방식대로 덮어써도 좋아(Overriding)!"** 라는 권한을 열어주는 뜻이 됩니다. (사실 다중 상속에서의 `virtual`은 아주 특이한 케이스로 쓰이는 것이고, 실무에서 `virtual`은 99% 덮어쓰기 허용 목적인 '가상 함수'를 만들 때 사용됩니다.)

> 🙋‍♂️ **Q. 왜 내 컴퓨터(Visual Studio)에서는 `Warrior` 크기가 16바이트가 아니라 24바이트로 뻥튀기되나요?**
> **A. 컴파일러 엔진마다 "메모리를 정렬하고 패딩(Padding)을 넣는 규칙(ABI)"이 다르기 때문입니다!**
> C++ 표준 위원회는 가상 상속 컴파일 규칙을 하나로 강제하지 않았습니다.
> **[1] GCC/Clang 컴파일러 (최적화 중시):**
> 1. `vbptr` (8 byte)
> 2. `rage` (4 byte)
> 3. `Character::hp` (4 byte)
> 👉 패딩 없이 꽉꽉 압축해서 **정확히 16 byte**로 조립합니다.
> 
> **[2] MSVC 컴파일러 (Visual Studio, 관리 중시):**
> "가상 상속받은 부모는 무조건 메모리 맨 뒤로 격리시킨다"는 독특한 규칙을 가집니다.
> 1. `vbptr` (8 byte)
> 2. `rage` (4 byte)
> 3. **[빈 공간 패딩] (4 byte)** (자신만의 블록을 끝내며 8의 배수로 맞춤)
> 4. `Character::hp` (4 byte) (뒤로 밀려난 가상 부모 영역)
> 5. **[마무리 패딩] (4 byte)** (최종 크기를 8의 배수로 맞춤)
> 👉 결과적으로 **24 byte**라는 괴랄한 크기가 나옵니다. 자식 객체를 `sizeof` 할 때는 본체뿐만 아니라 뱃속의 "가상 부모의 크기 + 삽입된 패딩 + 본체"가 통째로 합산되기 때문입니다. 현업에서 구조체를 통째로 네트워크로 쏘면 안 되는 이유가 바로 이 ABI(컴파일러 간 메모리 맵핑 차이) 때문입니다.

> 🙋‍♂️ **Q. 가상 상속을 했더니 부모들(Warrior, Mage)이 최상위(Character) 생성자를 호출하는 코드가 무시되고, 막내(마검사)에서 직접 호출해야 하던데요? 왜 그런가요?**
> **A. 다이아몬드 상속 구조에서 "누가 공통 부모를 초기화할지" 모호해지기 때문입니다!**
> 만약 `MagicSwordsman`이 `Warrior`와 `Mage`를 상속받았는데, `Warrior`도 `Character` 생성자를 부르고 `Mage`도 `Character` 생성자를 부른다면? **어느 쪽이 전달한 체력(hp) 값으로 초기화해야 할지** 결정할 수 없는 충돌이 발생합니다!
> 결국 C++은 다음과 같은 엄격한 규칙을 세웠습니다:
> **"가상 기저 클래스(Virtual Base Class)의 생성 호출 권한은 오직 가장 하단에 있는 '최종 자식(Most Derived Class)' 단 하나만 가진다!"** 
> 중간 부모들(`Warrior`, `Mage`)이 아무리 이니셜라이저(`: Character{...}`)를 적어놔도 모두 무시되고, 최종 조립을 담당하는 최하단의 `MagicSwordsman`만이 공통 부모를 한 번만 초기화할 권리를 갖는 것입니다.

---

## 4. 모던 C++ 트렌드와 현업에서의 시각 

가상 상속으로 문제를 해결하긴 했지만, 눈에 보이지 않는 참조 포인터(`vbptr`)를 따라가며 메모리에 접근해야 하기 때문에 다음과 같은 단점이 생깁니다.
1. 메모리를 일직선상이 아닌 여기저기 참조하게 되어, CPU의 **캐시 히트율(Cache Hit Rate)**이 떨어집니다. (성능 저하)
2. 생성자의 호출 순서가 매우 복잡해지고, 구조 예측이 힘들어집니다.

그래서 현업 게임 엔진 개발이나 모던 C++ (Modern C++)에서는 다음과 같은 원칙을 따릅니다:

> 💡 **"상태(변수)를 가지는 부모의 다중 상속은 최대한 피하라. 대신, 기능(메소드)만 있는 '인터페이스(Interface)' 만을 다중 상속받거나, 상속보다는 **컴포지션(Composition, 포함 관계)**을 활용해라."**

이 개념을 이해하는 것은 다음 챕터에서 다룰 **'다형성(Polymorphism)'과 '순수 가상 함수를 활용한 인터페이스(Interface)'**의 기반이 됩니다.

---

### 🤔 질문 타임!
한 번 천천히 읽어보시고, `다중 상속의 문제점(다이아몬드 문제)`이나 `가상 상속`과 관련하여 이해가 안 되거나 더 궁금한 부분이 있다면 자유롭게 질문해 주세요! 질문이 없으시다면 바로 **오류 검증 실습 및 하드웨어 메모리 사이즈 확인**을 위한 `practice.cpp` 코드를 작성해 드리겠습니다.
