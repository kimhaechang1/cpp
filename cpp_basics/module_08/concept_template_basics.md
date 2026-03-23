# [Module 08] 템플릿(Templates) 기초: 컴파일러를 위한 붕어빵 틀

## 1. 실무 상황: 인벤토리의 한계
우리가 RPG 게임의 장비나 소비템을 담을 인벤토리를 만들고 있다고 가정해봅시다. 이 아이템들을 보관하는 슬롯(Slot) 역할을 하는 클래스를 만들게 될 것입니다.

```cpp
class PotionSlot {
    Potion* item;
public:
    void PutItem(Potion* p) { item = p; }
    Potion* GetItem() { return item; }
};

class WeaponSlot {
    Weapon* item;
public:
    void PutItem(Weapon* w) { item = w; }
    Weapon* GetItem() { return item; }
};
```

아이템 종류가 물약, 무기, 방어구, 퀘스트 아이템 등 수십 개, 수백 개로 늘어나면 어떻게 될까요? 내부에 들어가는 로직은 99% 똑같은데 값을 담는 자료형(`Potion*`, `Weapon*`)만 다를 뿐, 수백 개의 클래스를 복사/붙여넣기 해야 합니다. 결국 유지보수는 지옥이 됩니다.

## 2. 해결책: 템플릿(Template) - 일반화 프로그래밍
C++에서는 **"데이터의 타입" 자체를 미지수(T)로 두는 방법**을 제공합니다. 수학에서 $f(x) = x + 1$ 에 $x=2$ 를 넣듯이, 타입의 자리에 나중에 원하는 자료형을 넣을 수 있도록 "틀(Template)"을 짜는 것을 **일반화 프로그래밍(Generic Programming)**이라고 부릅니다.

템플릿은 한국어로 번역하면 "주형(틀), 붕어빵 틀"입니다. 붕어빵 틀(템플릿)은 그 자체로는 아무 동작도 하지도 먹을 수도 없지만, 밀가루(int) 반죽을 넣으면 밀가루 붕어빵(클래스)이 나오고, 팥(double) 반죽을 넣으면 팥 붕어빵(클래스)이 튀어나옵니다.

### 2-1. 함수 템플릿 (Function Template)
함수의 매개변수나 반환 타입을 특정 타입이 아니라 미지수로 바꿉니다. 관습적으로 `T` (Type의 약자)를 많이 사용합니다.

```cpp
template <typename T>
T GetMax(T a, T b) {
    return (a > b) ? a : b;
}

// 사용 시점
int maxInt = GetMax<int>(5, 10);
float maxFloat = GetMax<float>(3.14f, 5.5f);
// 최신 C++에서는 컴파일러가 매개변수를 보고 T가 굳이 무엇인지 추론할 수 있으면 괄호 <...> 부분을 생략할 수 있습니다. (Type Deduction)
```

### 2-2. 클래스 템플릿 (Class Template)
인벤토리 슬롯이나 플레이어 상태 저장고의 경우, 클래스의 멤버 변수나 메서드의 반환/매개변수 타입이 모두 `T`가 됩니다.

```cpp
template <typename T>
class Slot {
private:
    T item; 
public:
    void PutItem(T newItem) { 
        item = newItem; 
    }
    T GetItem() { 
        return item; 
    }
};

// 사용 시점: 우리가 사용할 때는 템플릿 인자 <T> 공간에 타입을 확실히 명시해야 합니다.
Slot<int> intSlot;          // int 타입 정수를 담는 슬롯
Slot<Monster*> monsterSlot; // Monster 포인터를 담는 슬롯
```

## 3. 하드웨어 관점: 컴파일러는 뒤에서 무엇을 하는가? (코드 팽창)
개발자들이 가장 많이 오해하는 것은 "오오! 템플릿이라는 요술 방망이 클래스 하나가 실행(Run Time) 도중에 모든 타입을 동적으로 받아주는구나"라고 착각하는 것입니다. **절대 아닙니다.**

이전에 배웠던 가상 함수(`virtual`)가 런타임에 vtable을 조회하는 기술이라면, **템플릿은 철저히 컴파일 타임(Compile Time, 빌드 되는 시간) 기술**입니다.

컴파일러는 개발자가 소스 코드 어딘가에서 `Slot<int>`와 `Slot<double>`을 사용하는 코드를 발견하면, 그 즉시 내부적으로 **완전히 독립적인 `int` 전용 클래스와 `double` 전용 클래스 2개를 복사/붙여넣기로 찍어냅니다.** 이를 템플릿의 **인스턴스화(Instantiation)**라고 부릅니다. 템플릿 코드는 사용되지 않으면 기계어로 변환조차 되지 않습니다.

*   ✅ **극강의 장점 성능(Zero-overhead Principle)**: 런타임에 타입을 묻지도 따지지도 않고 이미 타입별로 완벽히 맞춰진 최적화된 기계어가 생성되어 있으므로 런타임 성능 저하(오버헤드)가 철저하게 **0%**입니다. 인라인화 시키기도 가장 좋습니다.
*   ⚠️ **극강의 단점 부풀림(Code Bloat)**: 아주 다양한 타입으로 템플릿을 남발하면 컴파일러가 각각의 타입 전용 코드를 찍어내기 때문에, 컴파일 후 나오는 실행 파일 `.exe` 의 파일 사이즈가 눈덩이처럼 커지게 되고, 컴파일 속도도 느려집니다. 

### 🧐 시각적 검증 코드 (직접 눈으로 확인하세요!)
실제로 `Box<int>`와 `Box<double>`이 한 부모에게서 나온 형제가 아니라 **물리적으로 전혀 연관 없는 별개의 존재**인지 시각적으로 검증해 봅시다. 다음 코드를 빈 프로젝트에서 실행해 보세요.

```cpp
#include <iostream>
#include <typeinfo>

template <typename T>
class Box {
public:
    T item;
};

int main() {
    Box<int> intBox;
    Box<double> doubleBox;

    std::cout << "--- 메모리 크기 비교 ---" << std::endl;
    // intBox는 내부에 int 1개이므로 4바이트, doubleBox는 내부에 double 1개이므로 8바이트를 가집니다.
    std::cout << "sizeof(Box<int>): " << sizeof(intBox) << " bytes" << std::endl;
    std::cout << "sizeof(Box<double>): " << sizeof(doubleBox) << " bytes" << std::endl;

    std::cout << "\n--- 타입명 비교 ---" << std::endl;
    // 타입이 완전히 다름을 증명
    std::cout << "Type of Box<int>: " << typeid(intBox).name() << std::endl;
    std::cout << "Type of Box<double>: " << typeid(doubleBox).name() << std::endl;

    return 0;
}
```

## 4. [모던 C++ 기본기] `typename` vs `class`
오래된 C++ 서적을 보면 `template <class T>` 라고 적는 책이 아주 많습니다.

하지만 '클래스를 생성한다'는 의미의 `class` 키워드와 섞여서 혼동이 오는 것을 피하기 위하여 모던 C++ 현업 표준에서는 무조건 **`template <typename T>`**라고 적는 것을 원칙으로 합니다. (뜻은 100% 동일합니다.) 의식적으로 후자를 사용하는 습관을 들이세요!

---
## 💡 FAQ: 템플릿 선언(Prefix) 중복 사용 여부
**Q: 파일 맨 위에 `template <typename T>`를 한 번만 적어두고, 아래에 있는 여러 함수나 여러 클래스에서 `T`를 계속 재사용(중복 사용) 할 수는 없나요?**

**A: 절대 불가능합니다!**  
`template <typename T>` 라는 선언은 **바로 다음에 등장하는 단 하나의 함수나 단 하나의 클래스에만** 유효합니다. 일종의 '일회용 티켓'이라고 생각하시면 됩니다. 따라서 템플릿을 사용하는 독립적인 함수나 클래스가 10개라면, `template <typename T>` 도 그 함수/클래스 바로 위에 각각 10번을 달아주어야 합니다.

**❌ 잘못된 예시 (불가능)**
```cpp
template <typename T>  // ⬅️ 맨 위에 한 번만 선언해두고?

void Print(T value) { /* ... */ }  // 여기엔 적용됨 (티켓 소모)
void ErrorFunc(T value) { /* ... */ }  // ❌ 컴파일 에러! (T가 뭔지 모름)
class ErrorClass { T item; };    // ❌ 컴파일 에러! (T가 뭔지 모름)
```

**✅ 올바른 예시**
```cpp
template <typename T>
void Print(T value) { /* ... */ }  // ⭕ 적용 완료 (티켓 소모)

template <typename T>             // ⬅️ 새로 발급
void GoodFunc(T value) { /* ... */ }  // ⭕ 적용 완료

template <typename T>             // ⬅️ 새로 발급
class GoodClass { T item; };      // ⭕ 적용 완료
```

> **참고**: 물론, `template <typename T> class Slot { ... };` 처럼 클래스 템플릿 **내부**에 정의된 멤버 변수와 여러 개의 멤버 함수들은 그 닫는 중괄호 `}` 전까지는 `T`를 자유롭게 반복해서 사용할 수 있습니다. 클래스라는 하나의 거대한 덩어리 자체에 템플릿 티켓이 적용되었기 때문입니다.

---
## 💡 FAQ: 복수 개 선언 및 중첩(Nested) 사용
**Q: `T` 하나 말고 복수 개를 선언하는 것도 되나요? 그리고 `vector<vector<int>>` 처럼 템플릿 안에 템플릿을 중첩해서 쓸 수도 있나요?**

**A: 예, 둘 다 완벽하게 지원됩니다!**

**1. 복수 개 템플릿 파라미터 선언**
하나의 붕어빵 틀에 들어가는 재료가 여러 개라도 상관없습니다. 쉼표(`,`)로 구분하여 여러 개의 미지수를 둘 수 있습니다. 나중에 배우게 될 키-값 쌍을 저장하는 `std::map` 자료구조가 대표적인 예입니다.

```cpp
template <typename Key, typename Value> // 두 개의 파라미터
class Dictionary {
    Key k;
    Value v;
};

// 사용: 꺾쇠 안에 쉼표로 구분하여 타입을 차례대로 넣어줍니다.
Dictionary<std::string, int> dict1; // Key는 string, Value는 int
Dictionary<int, Monster*> dict2;    // Key는 int, Value는 Monster*
```

**2. 템플릿의 중첩(Nested) 사용**
템플릿의 미지수 자리에 '또 다른 템플릿 클래스' 자체가 들어가는 것도 당연히 가능합니다! 컴파일러가 아주 똑똑하게 안쪽부터 바깥쪽으로 타입을 유추해 냅니다.

```cpp
template <typename T>
class Box { T item; };

// 1. 가장 안쪽: Box<int> 클래스가 먼저 찍혀 나옵니다.
// 2. 바깥쪽: T 자리에 'Box<int>' 타입이 들어간 Box 형이 최종적으로 찍혀 나옵니다.
Box<Box<int>> nestedBox;
```
이러한 중첩 개념은 게임 개발에서 체스판이나 타일맵 같은 **2차원 배열, 혹은 그리드(Grid)**를 만들 때 `std::vector<std::vector<int>>` 의 형태로 실무에서 숨 쉬듯이 쓰이는 아주 중요한 패턴입니다.

---
## 💡 FAQ: 컴파일러의 서로 다른 클래스 이름 처리 (Name Mangling)
**Q: 컴파일러가 `int` 들어간 클래스와 `double` 들어간 클래스를 빌드 시간에 찍어낸다면, 이 둘은 서로 다른 클래스니까 기계어 수준에서는 이름(식별자)도 달라질 텐데, 컴파일러가 알아서 구별 가능한 이름으로 만들어 주나요?**

**A: 정확합니다! C++ 컴파일러는 이 과정을 완벽하게 처리하며, 이를 "네임 맹글링(Name Mangling)"이라고 부릅니다.**

개발자가 코드에 `Box<int>` 와 `Box<double>` 이라고만 적어두어도, 컴파일 과정에서 C++ 컴파일러는 기계어가 충돌나지 않도록 완전히 유일한 문자열로 클래스와 함수들의 실제 이름을 믹서기처럼 "비틀어(Mangle)" 버립니다. 

예를 들어 GCC(g++) 머신러닝 단으로 내려가면 다음과 같이 고유한 이름으로 번역됩니다:
- `Box<int>` ➔ `_ZN3BoxIiE`  (내부에 `i`가 포함되어 int를 의미)
- `Box<double>` ➔ `_ZN3BoxIdE` (내부에 `d`가 포함되어 double을 의미)

**[하드웨어/기계어 관점의 진실]**
즉, 템플릿은 "형태를 바꾸는 마법의 클래스 1개"가 아닙니다. 기계어(`.exe` 파일) 입장에서는 `Box<int>` 와 `Box<double>` 사이에 그 어떤 부모-자식 관계나 공통점도 없습니다. 이 둘은 **이름부터 크기까지 완전히 다른, 우연히 로직만 똑같이 생긴 '서로 남남인 독립된 2개의 클래스'**일 뿐입니다. 

그래서 프로그래머가 `Box<int>`의 멤버 함수를 호출하면, 컴파일러는 알아서 미리 찍어둔 `_ZN3BoxIiE` 관련 함수로 연결해 주는 수동 병합 작업을 뒤에서 조용히 처리해 줍니다!

---
## 💡 FAQ: 템플릿과 다형성의 결합 (Java Generics와의 차이점)
**Q: 자바(Java)에서는 `Collection<Integer> c = new ArrayList<>();` 처럼 인터페이스 레벨에서 제네릭(Generics)을 사용해 다형성을 누리는데, C++에서도 이렇게 인터페이스(추상 클래스) 템플릿을 만들어 부모 포인터로 자식을 가리키는 것이 가능한가요?**

**A: 문법적으로는 100% 완벽하게 가능합니다! 하지만, C++의 표준 라이브러리(STL)는 게임 개발 같은 성능 최우선 환경을 위해 이 방식을 의도적으로 기피합니다.**

**1. 문법적 허용: 템플릿 추상 클래스 만들기**
이전 Module 7에서 배운 `virtual` 기능과 추상 클래스를 템플릿 문법과 결합하여 Java와 완벽히 똑같은 구조를 짤 수 있습니다.
```cpp
// Java의 Collection<T> 와 유사한 추상 클래스(인터페이스) 템플릿
template <typename T>
class ICollection {
public:
    virtual void Push(T item) = 0;
    virtual ~ICollection() {}
};

// Java의 ArrayList<T> 와 유사한 상속 구현체 템플릿
template <typename T>
class MyArrayList : public ICollection<T> {
public:
    void Push(T item) override { /* 구현 */ }
};

// ⭕ 부모 포인터(ICollection)로 자식(MyArrayList) 다루기 완벽 지원
ICollection<int>* myCol = new MyArrayList<int>();
myCol->Push(10);
delete myCol;
```

**2. C++ STL(표준 라이브러리)이 이 방식을 안 쓰는 이유 (철학적 차이)**
Java의 제네릭은 '타입 소거(Type Erasure)' 방식으로 런타임에 단 1개의 클래스(`Object`)를 사용하며 무조건 가상 함수 기반의 다형성 검사를 거칩니다.

반면 C++ 개발자들은 **런타임 동작 성능**을 극단적으로 중요하게 여깁니다. 바로 직전 Module 7에서 배웠듯이, `virtual` 가상 함수는 실행 중 vtable(가상 함수 표)을 조회하는 오버헤드가 발생하며, **데이터 지향 설계(DOD) 관점에서의 CPU 캐시 히트율(Cache Hit)** 을 심각하게 떨어뜨릴 수 있습니다.

그래서 C++이 공식적으로 제공하는 `std::vector`, `std::list` 등의 컨테이너들은 공통 부모 `ICollection<T>` 같은 인터페이스를 **전혀 상속받지 않습니다.** 가상 함수 테이블(vtable) 자체를 아예 제거해서 "템플릿 컴파일 타임 생성"의 가장 큰 장점인 **Zero-overhead 성능**을 미친 듯이 뽑아내기 위해서입니다. (대신 상속으로 묶는 대신, 다음 시간에 배울 **"반복자(Iterator)"**라는 또 다른 템플릿 기법을 써서 사용법을 일관되게 맞춥니다.)

---
## 💡 FAQ: 템플릿과 공변성 (Covariance) 지원 여부
**Q: `Slime`이 `Monster`를 상속받은 자식 클래스일 때, 자바나 C#의 (`IEnumerable<out T>`) 처럼 `Box<Slime>` 객체를 `Box<Monster>` 변수에 담는 공변성(Covariance) 자동 치환 기능을 C++에서도 지원하나요?**

**A: 기본적으로는 절대 불가(Invariant, 철저한 무변성)합니다! (원리를 이해하셔야 합니다)**

방금 위(FAQ: 네임 맹글링)에서 배운 **컴파일러의 빌드타임 클래스 분리** 이론을 다시 떠올려보세요! 기계어 입장에서는 `Box_Slime` 과 `Box_Monster` 라는, 1글자도 이름이 겹치지 않는 완전히 별개의 독립된 클래스로 쪼개집니다. 
알맹이인 `Slime`과 `Monster`가 부모/자식 관계 일지라도, 그들을 감싸고 있는 **포장지인 `Box_Slime` 클래스 그 자체는 `Box_Monster` 클래스를 상속(public) 받은 적이 단 한 번도 없기 때문**에 C++의 철저한 타입 검사(Type Checking)를 뚫고 자동 형변환이 일어날 수가 없습니다.

```cpp
class Monster {};
class Slime : public Monster {};

// 일반 클래스 다형성 (지원)
Monster* m = new Slime(); // ⭕ 완벽히 정상 작동

// 템플릿 공변성 시도 (거부됨)
Box<Slime> slimeBox;
Box<Monster> monsterBox = slimeBox; // ❌ 컴파일 에러! (서로 다른 완전히 별개의 클래스임)
```

**[해결책: 템플릿 변환 생성자 (Member Template Constructor)로 우회하기]**
C++ 템플릿은 원천적으로 Invariant(무변성) 규칙을 따르므로 자동으로 공변성을 지원해 주지 않습니다. 자바/C#처럼 동작하게 만드려면 프로그래머가 **"타입 U(자식)를 가진 다른 템플릿 클래스를 받아주는 변환 생성자"**를 클래스 내부에 명시적으로 정의하여 강제로 뚫어주어야 합니다.

```cpp
template <typename T>
class Box {
public:
    T* item;
    Box() : item(nullptr) {}

    // 🌟 핵심: 나 자신(T)과 구별되는 '또 다른 타입(U)'로 빚어진 형제의 데이터를 받는 생성자
    template <typename U>
    Box(const Box<U>& other) {
        // Slime* 타입(other.item) 이 Monster* 타입(this->item) 에 대입 대입 연산이 가능한지는
        // 이 중괄호 내부 코드가 실행될 때 C++ 다형성 규칙에 의해 검사되고 작동합니다.
        this->item = other.item; 
    }
};

// 사용
Box<Slime> slimeBox;
Box<Monster> monsterBox = slimeBox; // ⭕ 복사 시 변환 생성자가 작동하며 통과합니다! (U=Slime, T=Monster)
```
나중에 우리가 고급 모듈(Module 9)에서 배우게 될 **모던 C++ 스마트 포인터(`std::shared_ptr<T>`)**가 상속 관계의 동적 할당된 포인터들을 안전하게 대입/공유 받기 위해 내부적으로 **정확히 똑같이 위와 같은 '멤버 템플릿 변환 생성자' 기술을 사용**하여 공변성을 흉내 내고(포장하고) 있습니다.

---
## 💡 FAQ: 멤버 템플릿에서 `T`와 `U`의 동시 사용
**Q: 위에서 배운 '변환 생성자' 코드를 보면, 템플릿 클래스 `Box<T>` 안에서 생성자는 또 다른 템플릿 파라미터 `U`를 쓰는데, 이럴 경우 생성자 매개변수로 `T` 타입과 `U` 타입을 동시에 섞어서 받을 수도 있나요?**

**A: 완벽하게 가능합니다! 이것이 C++ 템플릿의 강력무비한 특징입니다.**

클래스 템플릿의 멤버 함수(생성자 포함) 자체에 또 다른 템플릿을 거는 것을 **"멤버 템플릿(Member Template)"**이라고 부릅니다. 이 구조에서는 두 놈(`T`와 `U`)의 타입이 확정되는(결정되는) 타이밍이 서로 절묘하게 다릅니다.

1. **`T`가 정해지는 시간**: `Box<Monster> monsterBox;` 처럼 클래스(뼈대)를 바깥에서 선언할 때 이미 확정됩니다.
2. **`U`가 정해지는 시간**: 해당 멤버 함수나 생성자가 **실제로 호출될 때**, 괄호 안으로 넘어오는 매개변수의 생김새를 보고 컴파일러가 알아서 유추(Type Deduction)해 냅니다.

따라서 아래와 같이 `T`와 `U`를 한 함수 안에서 자유롭게 섞어 쓸 수 있습니다!

```cpp
template <typename T>
class Box {
public:
    T* item;

    // 나 자신(T) 타입도 하나 받고, 
    // 나랑 무관한 누군지 모를 타입(U)도 동시에 받는 자유로운 멤버 템플릿
    template <typename U>
    Box(T* myTarget, U otherSource) {
        // T는 밖에서 Box를 만들 때 이미 고정되어 알고 있고,
        // U는 이 함수가 호출되는 순간 파라미터를 역추적해서 알아냅니다.
    }
};

// Box<Monster> 로 객체를 만드므로 T = Monster 고정!
// 괄호 안에 T*(new Monster()) 와 U(100) 을 던지면 U = int 로 자동 추론됨!
Box<Monster> myBox(new Monster(), 100); 
```
이러한 "클래스 템플릿 + 멤버 템플릿"의 조합 기법은, 서로 다른 호환되지 않는 컨테이너들끼리 함수 레벨에서 유연하게 데이터를 이식(복사/대입)받을 수 있게 허용해 주는 현업 상급 기술입니다.

---
## 💡 FAQ: 컴파일러는 `T`와 `U` 조합을 어떻게 찍어낼까? (의사 코드)
**Q: 클래스를 제네릭으로 독립적으로 찍어낸다고 하셨는데, 멤버 함수(생성자)도 마찬가지인가요? 방금 본 저 `Box<T>` 안에 템플릿 생성자 `<U>`가 있는 코드를 빌드하면 컴파일러가 조용히 뒤에서 어떻게 찍어내는지 예시를 보여줄 수 있나요?**

**A: 예, 함수(메서드)도 클래스와 "완벽하게" 똑같이 무식한 복사/붙여넣기(인스턴스화) 방식으로 찍혀 나옵니다!**

여러분이 소스 코드 어딘가에서 아래와 같은 3줄의 코드를 썼다고 합시다.
```cpp
// 1번 호출 (T는 Monster 클래스 사용, 생성자 인자 U는 int 사용)
Box<Monster> box1(new Monster(), 100); 

// 2번 호출 (T는 똑같이 Monster를 쓰는데, 생성자 인자 U를 double로 사용)
Box<Monster> box2(new Monster(), 3.14); 

// 3번 호출 (T를 Slime 클래스로 사용, 생성자 인자 U는 int 사용)
Box<Slime> box3(new Slime(), 999); 
```

빌드(Build) 버튼을 누르면, **C++ 컴파일러는 위 코드들을 스캔한 뒤 뒤에서 조용히 코드를 아래와 같이 (마치 개발자가 손으로 일일이 다이핑한 것처럼) 물리적인 기계어 조합들로 빚어냅니다.** (이해를 돕기 위해 C++ 형태로 푼 의사 코드입니다)

```cpp
/* -------------------------------------
   컴파일러가 몰래 생성한 기계어 결과물 
-------------------------------------- */

// [조합 1] T = Monster 용도의 클래스 뼈대를 통째로 찍어냄
class Box_Monster {
public:
    Monster* item;
    
    // 1번 호출을 발견했으므로, U=int 인 전용 생성자 함수를 추가로 찍어냄!
    Box_Monster_Constructor_int(Monster* myTarget, int otherSource) { /* ... */ }
    
    // 2번 호출을 발견했으므로, U=double 인 전용 생성자 함수를 하나 '더' 추가로 찍어냄!
    Box_Monster_Constructor_double(Monster* myTarget, double otherSource) { /* ... */ }
};

// [조합 2] T = Slime 용도의 클래스 뼈대 통째로 새로 생성 (Monster와 겹처도 무조건 복붙!)
class Box_Slime {
public:
    Slime* item;
    
    // 3번 호출을 발견했으므로, U=int 인 전용 생성자 함수를 찍어냄!
    Box_Slime_Constructor_int(Slime* myTarget, int otherSource) { /* ... */ }
};
```

**[코드 팽창 (Code Bloat)의 치명적 부작용]**
위 예시를 보면 왜 템플릿을 **"실행 속도는 세상에서 제일 빠르지만, 파일 크기는 눈덩이(Code Bloat)"**라고 부르는지 단번에 이해되실 것입니다. 
만약 여러분이 10종류의 클래스(T)에 대해, 각각 10종류의 서로 다른 타입 파라미터(U)를 넣어서 함수를 무작위로 호출한다면? 
최종 기계어 파일(`.exe`) 내부에는 **100개(10 x 10)의 완전히 다르고 이름이 유일무이한 조합 함수들**이 복사/붙여넣기 되어 빽빽하게 쌓이게 됩니다! 이것이 템플릿의 가상 함수(vtable) 없는 압도적 스피드 뒤에 숨겨진 물리적 크기라는 치명적인 대가입니다.
