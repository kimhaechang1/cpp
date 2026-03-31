# Module 09: 타입 추론 (Type Inference) - `auto`와 `decltype`

## 1. 정적 타입(Static) 언어의 구원자, `auto`
C++은 컴파일 시점에 모든 변수의 타입이 결정되어야 하는 깐깐한 정적 타입 언어입니다. Python이나 JavaScript처럼 런타임에 타입이 바뀌지 않습니다.

과거 C++ 프로그래머들은 길고 복잡한 타입을 일일이 손으로 적어야 했습니다.
```cpp
std::vector<std::unique_ptr<EnergyCell>>::const_iterator it = resources.begin(); // 🤮
```

하지만 C++11부터 **`auto`**가 등장했습니다.
```cpp
auto it = resources.begin(); // 😎 컴파일러가 타입 알아서 맞춰줌!
```
*   **주의**: `auto`는 런타임에 타입을 결정하는 것이 아닙니다! **컴파일(빌드)하는 순간** 우변의 값을 보고 타입을 계산해서 코드를 치환해 버립니다. 즉, 실행 속도 저하는 0(Zero)입니다.

---

## 2. AAA (Almost Always Auto) 원칙과 예외
현업 모던 C++의 거장들은 **"거의 항상 auto를 쓰라(AAA 결의문)"**고 권장합니다.

### ✅ 언제 쓰는가?
1. **타입이 너무 길고 보기 흉할 때** (iterator 등)
2. **우변만 봐도 타입이 너무나 뻔할 때**
   ```cpp
   auto player = std::make_unique<Player>(); // 누가 봐도 unique_ptr<Player>
   ```

### ❌ 언제 안 쓰는가?
1. **가독성을 해칠 때**: 함수의 반환 타입이 무엇인지 한눈에 안 들어올 때
   ```cpp
   auto result = CalculateDamage(); // result가 int인지 float인지 알 수 없음!
   float floatResult = CalculateDamage(); // 이럴 땐 명시해주는 것이 더 좋을 수 있습니다.
   ```
2. **명시적 형변환이 필요할 때**
   ```cpp
   auto count = 0; // 이건 무조건 int입니다. unsigned int를 원한다면 auto count = 0u; 로 써야 함.
   ```

---

## 🚨 3. (복습) `auto`의 치명적인 함정: 참조와 `const` 무시
방금 전 누적 실습에서 뼈저리게 느끼셨죠? `auto`는 **"가장 순수한 값의 타입"**만 추론합니다.

```cpp
const std::string& GetName() const;

auto name1 = GetName();        // ❌ std::string 형. (const와 &가 다 떨어져 나가서 통째로 복사됨!)
const auto& name2 = GetName(); // ✅ const std::string& 형. (읽기 전용 참조 유지)
auto& name3 = GetName();       // ❌ 컴파일 에러! (const 반환값을 그냥 &로 받을 순 없음)
```
> **현업 규칙**: 값의 복사가 필요한 게 아니라면, 반복문과 변수 선언에서 **무조건 `const auto&`를 먼저 치고 시작하라!**

---

## 4. 진정한 타입 돋보기: `decltype` (Declared Type)
`auto`는 "변수를 만들고 값을 대입할 때" 쓰지만, 때로는 **값은 넣지 않으면서 "저 변수의 타입이 뭔지만 딱 알고 싶을 때"**가 있습니다.

C++의 `decltype(표현식)`은 괄호 안의 표현식이 **정확히 어떤 매커니즘의 타입**인지 단 1%의 정보 손실도 없이(const나 & 기호까지) 완벽하게 추출해 냅니다.

```cpp
const int a = 5;
int& ref = a;

auto b = ref;          // b의 타입은 그냥 int (참조와 const 날아감)
decltype(ref) c = a;   // c의 타입은 완벽하게 const int& 유보!
```

### 🎮 어디에 쓰나요? (게임 엔진 및 템플릿)
보통 게임 엔진 프로그래머들이 "두 개의 템플릿 타입을 더했을 때, 결과물의 타입이 뭐가 될지 예측할 수 없을 때" 사용합니다.
(지금 당장은 "이런 게 있구나~" 정도만 아셔도 충분합니다. 나중에 템플릿을 아주 깊게 다룰 때 다시 등장합니다.)

```cpp
template <typename T1, typename T2>
// T1과 T2를 곱한 결과의 '타입'을 알아내서 반환 타입으로 지정!
auto Multiply(T1 t1, T2 t2) -> decltype(t1 * t2) { 
    return t1 * t2;
}
```

---

## 5. 눈으로 확인하는 검증 코드 (typeid)
C++에서는 `<typeinfo>` 헤더의 `typeid`를 사용하면 변수의 타입을 눈으로 볼 수 있습니다. (컴파일러마다 표기는 조금 다를 수 있습니다.)

```cpp
#include <iostream>
#include <typeinfo>

int main() {
    auto a = 5;       // int
    auto b = 5.5f;    // float
    auto c = 5.5;     // double

    std::cout << typeid(a).name() << "\n"; // 'i' (int)
    std::cout << typeid(b).name() << "\n"; // 'f' (float)
    std::cout << typeid(c).name() << "\n"; // 'd' (double)
}
```

---

## 💡 준비되셨나요?
문서를 다 읽어보셨다면, `auto`와 `decltype`, 그리고 C++17의 트렌디한 문법인 **Structured Binding (`auto [x, y]`)** 까지 한 번에 찍어 먹어볼 수 있는 실습 파일을 생성해 드리겠습니다. **"실습 준비 완료"**라고 말씀해 주세요!
