# Module 08: Modern C++ Concepts (제약 조건)

템플릿은 강력하지만, 가끔 너무 관대해서 문제입니다. 잘못된 타입이 들어왔을 때 발생하는 '읽기 난해한 컴파일 에러'를 방지하고, 코드의 의도를 명확히 하는 것이 **Concepts**의 목적입니다.

---

## 1. 지옥의 에러 메시지는 이제 그만!
기존에는 템플릿에 `+` 연산이 안 되는 타입을 넣으면, 깊숙한 STL 내부 코드 어딘가에서 에러가 터져서 원인을 찾기 어려웠습니다. **Concepts**를 쓰면 함수 호출 시점에 즉시 "이 타입은 안 돼요!"라고 친절하게 알려줍니다.

---

## 2. 사용법: `requires` 와 `concept`

1.  **기본 제공 Concept 사용**: (C++20 표준 라이브러리)
    - `std::integral<T>`: 정수 계열인가?
    - `std::floating_point<T>`: 실수인가?
    - `std::sortable<I>`: 정렬 가능한가?

```cpp
#include <concepts>

// T는 반드시 정수(int, long 등)여야 함!
template <std::integral T>
T Add(T a, T b) {
    return a + b;
}
```

2.  **나만의 Concept 만들기**: `Monster` 비슷한 녀석들만 받고 싶을 때

```cpp
template <typename T>
concept HasPower = requires(T v) {
    v.power; // v라는 타입이 .power 라는 멤버를 가지고 있어야 함!
};

template <HasPower T>
void Attack(T const& m) {
    std::cout << m.power << " 데미지로 공격!" << std::endl;
}
```

---

## 3. 왜 Concepts를 쓰는가? (Game Dev 관점)

1.  **컴파일 타임 계약**: `Enemy` 클래스를 직접 상속받지 않았더라도, `Attack()` 멤버 함수만 있다면 공격 가능한 객체로 인정해 주는 유연한 설계를 할 수 있습니다. 
2.  **가독성**: 템플릿 인자만 봐도 "아, 이 함수는 어떤 인터페이스가 필요하구나"를 즉시 알 수 있습니다.
3.  **성능**: `if/else` 분기보다 컴파일 시점에 결정되므로 런타임 비용이 0입니다.

---

### 🏛️ 하드웨어 검증 (컴파일 타임 가드)
Concepts는 실제 실행 파일에 코드로 남지 않습니다. 오직 컴파일러의 '검문소' 역할만 수행합니다.

```cpp
// 만약 문자열(string)을 Add()에 넣으려고 하면?
// Add("Hello", "World"); 

// -> 컴파일 에러 메시지: "the associated constraints were not satisfied"
// "T가 std::integral을 만족하지 않습니다!" 라고 명확히 뜹니다.
```

---

### [CheckPoint]
이제 **Concepts**를 이용하여 우리가 만든 `Box<T>`나 `Command` 시스템에 **"특정 능력치를 가진 객체만 허용"**하는 제약을 걸 수 있습니다. 

내용이 이해되셨나요? 특히 Java의 `Interface`나 `Generics Bounds`(`<T extends Number>`)와 비교해 보시는 것도 재미있을 거예요. 준비되셨으면 마지막 실습으로 달려가겠습니다! 🔥

---

## 4. 실전 예시: 직접 만든 클래스를 `std::sortable`하게 만들기

멤버 변수 3개를 가진 `Hero` 클래스를 만들고, `std::sortable`이 요구하는 **비교 능력**과 **교체 능력**을 부여해 주는 과정입니다.

### 핵심 원리
`std::sortable`은 주로 이터레이터에게 묻지만, 그 이터레이터가 가리키는 **원소 타입**이 아래 조건을 만족해야 합니다.
- `operator<` (비교): "누가 더 작은가?"를 알아야 정렬할 수 있습니다.
- `std::swap` (교체): 위치를 바꿀 수 있어야 합니다. (기본 제공되지만, `=` 연산자가 있어야 함)

```cpp
#include <iostream>
#include <vector>
#include <algorithm>   // std::ranges::sort
#include <concepts>    // std::sortable

// 1. 멤버 변수 3개짜리 커스텀 클래스
struct Hero {
    std::string name;    // 이름
    int level;           // 레벨
    float attackSpeed;   // 공격 속도

    // [핵심 1] 비교 연산자: std::sortable 의 '비교 능력' 요건 충족
    // 여기서는 'level' 오름차순으로 정렬 기준을 정함
    bool operator<(const Hero& other) const {
        return this->level < other.level;
    }
    // [핵심 2] 복사 대입 연산자: std::sortable 의 '교체(Swap)' 요건 충족
    // Hero 멤버가 모두 기본 타입(string, int, float)이므로 컴파일러가 자동 생성해줌.
    // (만약 직접 동적 할당이 있었다면 Rule of 3 원칙에 따라 직접 구현해야 함!)
};

// 2. std::sortable 을 직접 강제하는 제네릭 정렬 함수
template <std::sortable I>
void SortHeroes(I first, I last) {
    std::ranges::sort(first, last);
}

// 3. 정렬 기준을 직접 지정하고 싶을 때 (Java의 Comparator 역할)
//    -> 람다를 사용하므로 sortable 검문과는 별개로 동작함
void SortByAttackSpeed(std::vector<Hero>& heroes) {
    std::ranges::sort(heroes, [](const Hero& a, const Hero& b) {
        return a.attackSpeed > b.attackSpeed; // 공격 속도 내림차순
    });
}

int main() {
    std::vector<Hero> party = {
        {"Warrior",  50, 1.2f},
        {"Archer",   30, 2.5f},
        {"Mage",     70, 0.8f}
    };

    // [Case 1] operator< 기준 (레벨 오름차순) 정렬
    SortHeroes(party.begin(), party.end());
    std::cout << "=== Level Order ===" << std::endl;
    for (const auto& h : party) {
        std::cout << h.name << " (Lv." << h.level << ")" << std::endl;
    }
    // Archer(30) -> Warrior(50) -> Mage(70)

    // [Case 2] 람다로 기준 변경 (공격 속도 내림차순)
    SortByAttackSpeed(party);
    std::cout << "=== Attack Speed Order ===" << std::endl;
    for (const auto& h : party) {
        std::cout << h.name << " (" << h.attackSpeed << ")" << std::endl;
    }
    // Archer(2.5) -> Warrior(1.2) -> Mage(0.8)
}
```

### Java와 비교

Java에서 `Hero`를 정렬 가능하게 만들려면 `implements Comparable<Hero>` 후 `compareTo`를 구현해야 합니다. C++은 `operator<`만 구현하면 **`std::sortable` 조건이 자동으로 충족**되며, 별도의 상속이나 인터페이스 선언 없이 모든 STL 알고리즘에서 인정해 줍니다. (Duck Typing!)
