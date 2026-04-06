# C++ 기초: Enum (열거형)

## 1. Enum (열거형) 기초부터 심화까지

### 🐣 기초 Usage (입문자용)

`enum`은 **"선택지"**를 만들 때 가장 좋습니다.

```cpp
// 1. 정의하기
enum class State { // class를 붙여야 :: 사용 가능
    IDLE, 
    RUN, 
    ATTACK 
};

// 2. 사용하기
State s = State::RUN; // "State 소속의 RUN"
```

### 🙋‍♂️ Q&A: "Enum을 바로 문자열로 출력할 수 있나요?"

> **Q. `cout << State::RUN;` 하면 "RUN"이라고 나오게 할 수 없나요?**
>
> **A. 아쉽게도 C++에는 그런 기능이 기본으로 없습니다. (0으로 나옵니다)**
> C++ 입장에서 Enum은 그냥 **숫자(Integer)**일 뿐, "RUN"이라는 글자는 코드 짤 때만 쓰는 가짜 이름이기 때문입니다.
>
> **해결책**: 직접 **번역기 함수**를 만들어야 합니다. (노가다...😅)
>
> ```cpp
> string StateToString(State s) {
>     switch(s) {
>         case State::IDLE:   return "IDLE";
>         case State::RUN:    return "RUN";
>         case State::ATTACK: return "ATTACK";
>         default:            return "UNKNOWN";
>     }
> }
> 
> // 사용
> cout << StateToString(s); // "RUN" 출력됨!
> ```

### 🙋‍♂️ Q&A: "왜 점(.)이 아니라 콜론(::)을 쓰나요?"

> **Q. `State.RUN`이 아니라 왜 `State::RUN`인가요?**
>
> 1.  **점 (`.`)**: **"살아있는 객체(변수)의 내용물"**을 꺼낼 때 씁니다.
>     *   `Player p;` (p는 살아있는 변수) -> `p.hp` (O)
>
> 2.  **콜론 두 개 (`::`)**: **"소속(Category)을 나타낼 때"** 씁니다. (Scope Resolution Operator)
>     *   `State`는 변수가 아니라 **타입(분류표)**입니다.
>     *   `RUN`은 `State`라는 분류표 **안에 소속된** 단어입니다.
>     *   그래서 "State **소속의** RUN"이라는 뜻으로 `State::RUN`을 씁니다.

---

### 🚀 심화 Learning (Enum Class)

단순히 "숫자에 이름 붙이기"가 아닙니다. 모던 C++에서는 **타입 안전성(Type Safety)**의 핵심입니다.

### 🆚 c-style `enum` vs modern `enum class`

**1. 전통적인 `enum` (C-Style)**
*   **문제점 1**: 이름이 전역으로 퍼집니다 (Namespace Pollution).
    *   `enum Color { RED, BLUE };`
    *   `enum TrafficLight { RED, GREEN };` // ❌ 에러! RED가 겹침!
*   **문제점 2**: 암시적으로 `int`로 변환됩니다.
    *   `if (Color::RED == 2)` // 논리적으로 이상한데 비교가 되어버림.

**2. 모던 `enum class` (강력 추천 👍)**
*   **장점 1**: 이름이 겹치지 않습니다.
    *   `Color::RED` 와 `TrafficLight::RED`는 완전히 다릅니다.
*   **장점 2**: 엄격한 타입 검사 (int와 비교 불가).

```cpp
enum class State { IDLE, RUN, ATTACK };

State s = State::RUN;
// if (s == 1)      // ❌ 컴파일 에러! (안전함)
// if (s == State::RUN) // ⭕ OK
```

### 💾 용량 다이어트 (Underlying Type)의 원리
"어떻게 4바이트를 1바이트로 압축하나요?"

*   **원리**: 사실 압축 기술이 아닙니다. 컴파일러에게 **"야, 이거 숫자 0, 1, 2, 3 밖에 안 쓰니까 굳이 큰 그릇(`int`) 쓰지 말고 작은 간장 종지(`char`)에 담아!"** 라고 명령하는 것입니다.
*   **Default**: 아무 말 안 하면 컴파일러는 안전하게 `int` (4바이트)를 씁니다.
*   **`: char`**: 명시적으로 지정하면 `char` (1바이트) 그릇을 씁니다.

```cpp
// : char 를 붙이면 1바이트가 됩니다!
enum class ElementType : char { 
    FIRE,   // 0 (0000 0000)
    WATER,  // 1 (0000 0001)
    EARTH,  // 2 (0000 0010)
    WIND    // 3 (0000 0011)
}; 
// 1바이트(256까지 표현 가능)로 충분하므로 OK!
// 만약 항목이 256개가 넘어가면 컴파일러가 "야, 넘치는데?" 하고 에러를 냅니다.

cout << sizeof(ElementType); // 결과: 1 byte
```
> **활용**: 네트워크 패킷이나 저장 데이터를 줄일 때 필수적입니다.

### 🙋‍♂️ Q&A: "enum과 enum class의 정확한 차이가 뭔가요?" (M10 복습)

> **Q. 왜 실무에서는 단순 `enum`보다 `enum class`를 쓰라고 하나요?**
>
> **1. 범위(Scope)의 차이**: 
> - `enum`: 이름을 밖으로 내뱉습니다 (`Inactive`라고만 써도 됨). 다른 enum에 같은 이름이 있으면 충돌합니다.
> - `enum class`: 이름을 가둡니다 (`QuestState::Inactive`). 다른 곳에서 같은 이름을 써도 안전합니다.
>
> **2. 타입 안전성(Type Safety)**: 
> - `enum`: 사실상 **정수(int)**입니다. `if (state == 1)` 같은 실수하기 쉬운 코드가 돌아갑니다.
> - `enum class`: **독립적인 타입**입니다. 정수와 비교하려면 `static_cast<int>(...)`로 명시적 변환을 해야만 합니다. 실수할 틈을 주지 않습니다.

| 비교 항목 | enum | enum class |
| :--- | :--- | :--- |
| **범위 (Scope)** | 전역 (또는 소속 범위) | 열거형 내부에 한정 (`::` 필수) |
| **정수 반환** | 자동으로 정수 변환됨 | `static_cast` 필요 |
| **안전성** | 낮음 (이름 충돌 기회 많음) | 높음 (독립된 타입 취급) |

> [!TIP]
> **결론**: 특별한 이유(C 언어 호환 등)가 없다면 항상 **`enum class`**를 사용하여 이름 충돌을 방지하고 코드의 의도를 명확히 하세요.
