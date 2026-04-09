# Module 6 보충: 스트림 추출/삽입 연산자(<<, >>) 오버로딩

## 📌 왜 `operator<<`는 전역(Global)에 `friend`로 구현할까?

`Player`나 `Monster` 같은 사용자가 직접 만든 클래스를 `std::cout << player;` 처럼 우아하게 콘솔에 출력하려면 어떻게 해야 할까요?

### 1. 연산자 오버로딩의 규칙 
`A + B`를 연산자 오버로딩으로 만든다면, 이는 내부적으로 `A.operator+(B)` 와 같이 치환됩니다. 즉, **연산자의 왼쪽 요소(A)가 그 함수의 주체**가 됩니다.

### 2. `std::cout << player` 의 딜레마
출력 연산자 `<<`를 `Player` 클래스의 내부 멤버 함수로 구현하려고 하면 아주 골치 아픈 딜레마에 빠집니다.
*   만약 멤버 함수로 구현한다면: `player << std::cout;` 처럼 써야 합니다. (아주 어색하고 거꾸로 된 문법이 되죠!)
*   우리가 원하는 자연스러운 문법: `std::cout << player;` 

하지만 연산자의 왼쪽(주체)인 `std::cout`(또는 `std::ostringstream`)은 우리가 수정할 수 없는 C++ 표준 라이브러리 안에 있습니다. 따라서 그곳에 `operator<<` 멤버 함수를 마음대로 추가해 줄 방법이 없습니다.

### 3. 해결책: 전역 함수와 `friend`
이를 해결하기 위해 클래스 내부가 아닌 **전역(Global) 공용 공간**에 글로벌 함수를 만듭니다. (왼쪽에는 스트림, 오른쪽에는 객체)

```cpp
// 1. 전역 함수로 작성
inline std::ostream& operator<<(std::ostream& os, const Player& p);
```

이때 전역 함수는 `Player` 클래스 울타리 바깥에 있으므로, 정보 은닉(Encapsulation) 원칙에 의해 내부의 `private/protected` 변수(예: `p.m_hp`)에 접근할 수 없습니다. 그래서 `Player` 클래스 설계도 속에 **`friend` (친구 명단록)**를 달아서 예외적으로 내부 접근을 허락해주는 것입니다.

```cpp
class Player {
private:
    int m_hp {100};

    // "이 전역 함수는 내 private 에 접근해도 좋아"
    friend std::ostream& operator<<(std::ostream& os, const Player& p);
};

// 구현부 (보통 .cpp로 분리하거나 inline 사용)
inline std::ostream& operator<<(std::ostream& os, const Player& p) {
    os << "Player HP: " << p.m_hp; // private 멤버 접근 가능
    return os;
}
```

> 🛠️ **중복 정의(Multiple Definition) 주의**: 전역 함수를 헤더(`.h`)에 작성할 때 몸체(`{...}`)까지 모두 구현해버리면, 그 헤더를 `#include` 하는 여러 `.cpp` 파일에 함수가 중복 생성되어 링커 에러가 발생합니다. **따라서 헤더에는 껍데기만 선언해 두고 `.cpp` 파일로 몸체를 옮기거나, 헤더에 두고 싶다면 위 예시처럼 `inline` 키워드를 반드시 붙이셔야 합니다.**
