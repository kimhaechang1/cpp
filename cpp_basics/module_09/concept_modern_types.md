# [Module 09] 모던 C++의 새로운 타입: 안전한 에러 처리와 데이터 표현 (C++17/23)

과거의 C++에서는 코드를 짜다 보면 이런 고민을 매일같이 했습니다.
- "값을 찾지 못했을 때 nullptr을 반환할까? 아니면 특수한 에러 코드(-1)를 반환할까?"
- "함수에서 성공 값도 반환하고 에러 정보도 동시에 반환해야 하는데 어떡하지? 참조자(&)나 포인터를 파라미터로 받아서 밖으로 빼낼까?"
- "Union 구조체를 써보려고 하니 타입 안정성이 너무 떨어지는데..."

모던 C++(C++17 및 최신 C++23)에서는 이런 전통적인 딜레마를 아주 우아하게 해결해 주는 세 가지 **새로운 래퍼(Wrapper) 타입**을 제공합니다. 
이 녀석들은 모두 `<optional>`, `<variant>`, `<expected>` 헤더에 들어있습니다.

---

## 1. `std::optional<T>`: "값이 있을 수도 있고, 없을 수도 있습니다" (C++17)

데이터베이스나 배열에서 무언가를 검색할 때 가장 흔한 상황입니다. 값을 찾으면 대상 객체를 주고 싶고, 못 찾으면 "없다"고 알려줘야 합니다.

**[과거의 방식 - 포인터 반환]**
포인터를 반환하면 `nullptr`인지 계속 체크해야 하고, 소유권과 생명 주기가 모호해졌습니다. 또는 특수값(-1)을 정해놓기도 했죠.

**[모던 C++ 방식 - optional]**
`std::optional`은 **"값 하나를 담거나 비어있을 수 있는 가장 작은 컨테이너"**입니다.
`std::nullopt`라는 키워드를 사용하여 "완벽한 빈 상태"를 표현합니다.

```cpp
#include <iostream>
#include <optional>
#include <string>

// 아이디로 유저 닉네임을 찾습니다. 못 찾을 수도 있습니다.
std::optional<std::string> FindUserNickname(int id) {
    if (id == 1) return "Faker";           // 값을 반환하면 자동으로 optional로 감싸짐
    if (id == 2) return "Deft";
    return std::nullopt;                   // 실패 시 '비어있음'을 반환! (-1이나 빈 문자열보다 훨씬 안전)
}

int main() {
    auto result = FindUserNickname(3);

    // if문 안에 객체를 바로 넣으면 (bool) 형변환이 되며 값이 있는지 검사합니다.
    if (result) {
        // 값을 꺼낼 때는 역참조(*)나 .value() 함수를 사용합니다.
        std::cout << "유저 찾음: " << *result << "\n";
    } else {
        std::cout << "유저 없음!\n";
    }

    // 또는 '값이 없을 때 쓸 기본값'을 우아하게 한 줄로 지정할 수도 있습니다.
    std::string safeName = result.value_or("Unknown");
}
```

---

## 2. `std::variant<T...>`: "타입에 안전한 다목적 박스(Type-safe Union)" (C++17)

게임 설정값 같은 것을 관리할 때, 설정값이 정수(`int`), 실수(`float`), 혹은 문자열(`string`) 중 하나일 수 있습니다.
과거에는 C언어의 `union`(공용체)을 썼지만, 내가 안에 뭘 담았는지 추적을 못해서 버그의 온상이었습니다.
`variant`는 자기가 안에 지금 **어떤 타입의 데이터를 품고 있는지 기억하는 똑똑한 다목적 박스**입니다.

```cpp
#include <variant>

// 이 변수는 int, float, string 셋 중 하나만 가질 수 있습니다!
std::variant<int, float, std::string> settingValue;

settingValue = 100;         // 안에는 이제 int가 들어있음
settingValue = "High";      // 안에는 이제 string이 들어있고, 기존 int는 파괴됨!

// 값을 안전하게 꺼내는 방법 (std::get)
// 만약 다른 타입을 꺼내려 하면 `std::bad_variant_access` 예외가 발생하여 터집니다!
std::string curr = std::get<std::string>(settingValue);

// 특정 타입이 들어있는지 확인하려면?
if (std::holds_alternative<std::string>(settingValue)) {
    // string이 확실함!
}
```
**특징:** 다형성(상속)을 쓰지 않고도 전혀 다른 객체들을 한 묶음으로 묶어버리는 강력한 기술입니다.

---

## 3. `std::expected<T, E>`: 성공(Value) 혹은 실패(Error)의 단판 승부 (C++23)

현대의 게임 서버나 네트워크 코딩에서最も(가장) 트렌디한 방식입니다. 
함수가 "성공했을 때 반환할 타입 `T`"와 "실패했을 때 반환할 타입 `E`"를 명시합니다. 
이는 C++의 무거운 **예외 처리(try-catch)**를 대체하는 엄청나게 빠른 최신식 기법입니다.

```cpp
#include <expected>
#include <string>

enum class ErrorCode { NetworkDown, InvalidPassword, BannedUser };

// 성공하면 string(토큰)을 주고, 실패하면 ErrorCode를 줍니다.
std::expected<std::string, ErrorCode> LoginServer(std::string pwd) {
    if (pwd == "1234") {
        return "SUCCESS_TOKEN_9999"; 
    } else {
        // 에러를 반환할 때는 unexpected로 감싸서 던집니다!
        return std::unexpected(ErrorCode::InvalidPassword);
    }
}

int main() {
    auto response = LoginServer("0000");

    if (response.has_value()) { // 성공했다면
        std::cout << "로그인 성공! 토큰: " << *response << "\n";
    } else {                    // 에러가 났다면
        ErrorCode err = response.error();
        std::cout << "로그인 실패: 에러코드 " << (int)err << "\n";
    }
}
```
*핵심: `Out Parameter`(&참조를 통해 반환값을 여러 개 뽑는 구시대적 방식)를 완전히 멸종시킨 모던 C++의 걸작입니다.*

---

## 🚀 Q&A 시간 

### Q1. "예외 처리(try-catch)와 `std::expected`의 차이가 뭔가요?"
전통적인 서버나 게임 물리 엔진에서 `try-catch`를 기피하는 이유와 정확히 맞닿아 있습니다!
1. **성능 폭탄 (Stack Unwinding)**: `throw` 예외를 던지면 프로그램은 에러를 잡을(catch) 때까지 함수 호출 단계를 미친 듯이 거꾸로 거슬러 올라가며 상태를 전부 엎어버립니다. 이는 엄청난 CPU 성능 하락을 가져옵니다. 반면 `std::expected`는 그저 **'실패했다는 데이터'를 일반적인 `return`을 통해 아주 우아하게 돌려줄 뿐**이므로 추가적인 오버헤드가 제로(0)에 가깝습니다.
2. **함수 시그니처 (예측 불가능성)**: `int Divide(int a, int b);` 라는 함수가 있다고 가정해 봅시다. 이 함수가 0으로 나눴을 때 예외를 뿜어내는지, 문서를 안 읽어보면 개발자는 알 길이 없습니다. 하지만 `std::expected<int, ErrorType> Divide(int a, int b);` 라고 반환 타입을 명시하면, 컴파일러가 강제로 개발자에게 "이 함수는 실패할 가능성이 있으니 안전하게 에러 처리를 작성하라"고 압박할 수 있습니다 (이 방식을 C++보다 먼저 채택하여 대성공한 언어가 바로 Rust 입니다).

### Q2. "`std::variant`로 다형성(상속)을 어떻게 흉내 낼 수 있나요?"
모듈 07에서 배운 일반적인 다형성은 `Item`이라는 부모 클래스를 상속받아 가상 함수(`virtual`)를 만들고, 꼭 힙(Heap) 메모리에 할당해서 포인터로 배열에 담아야 했습니다. (오버헤드가 큽니다!)
하지만 `std::variant`를 쓰면 **상속 구조나 부모 클래스 없이 직교하는 클래스들을 하나로 묶을 수 있습니다.**

```cpp
struct Sword { void Attack() { std::cout << "칼질!\n"; } };
struct Bow   { void Shoot()  { std::cout << "활쏘기!\n"; } };

// 상속 없이, 완전히 다른 두 클래스를 하나로 합쳐버립니다! (가상 함수 없음, 힙 할당 없음)
using CharacterWeapon = std::variant<Sword, Bow>;

CharacterWeapon cw = Bow{}; // 활 장착!

// std::visit 이라는 마법의 함수를 통해 "지금 들어있는 타입에 맞는 함수"를 자동으로 실행시킵니다.
std::visit([](auto& w) {
    // w가 Sword면 w.Attack()을 해야하고, Bow면 w.Shoot()을 해야 하는데, 
    // 모던 C++ 기술을 쓰면 컴파일 타임 다형성을 구축할 수 있습니다. (추후 심화 예정)
}, cw);
```

### Q3. "코드에 `using CharacterWeapon = std::variant<Sword, Bow>;` 라고 적혀있는데, `using` 이 뭔가요?"
`using` 은 길고 복잡한 타입에 **'별명(Alias)'**을 붙여주는 모던 C++의 키워드입니다. C언어 시절의 `typedef`가 진화한 버전이라고 생각하시면 됩니다!

1. **가독성의 혁명**: 과거 `typedef`는 `typedef unsigned long long uint64;` 처럼 순서가 헷갈렸습니다. 하지만 `using`은 `using 왼쪽(별명) = 오른쪽(진짜 복잡한 타입);` 처럼 변수에 값을 대입하는 것처럼 직관적이라 훨씬 읽기 편합니다.
2. **왜 썼나요?**: 매번 저 긴 `std::variant<Sword, Bow>` 를 함수 매개변수나 변수에 타자 치기가 너무 귀찮고, 나중에 무기가 추가되어 `<Sword, Bow, Axe>` 로 바뀌면 일일이 다 고쳐야 합니다. 그래서 아예 `CharacterWeapon` 이라는 멋진 별명 하나를 지어두고 프로그램 이곳저곳에서 재사용하기 위함입니다.
3. **참고 (`using namespace std;`와의 관계)** : 완전히 다른 기능입니다! 네임스페이스를 폭격(Import)하는 용도 외에도, 최근의 C++에서는 이렇게 타입을 예쁘게 포장하는 가장 필수적인 문법으로 쓰입니다.
