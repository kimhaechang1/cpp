# Module 04 보충: `std::span` (C++20) - 포인터의 구원자 🦸‍♂️

## 1. 문제점: "포인터와 길이는 왜 항상 따로 다닐까?"
C++에서 배열을 함수로 넘길 때, 우리는 항상 **두 가지**를 넘겨야 했습니다.

```cpp
void ProcessArray(int* arr, int size) { // 으악, 귀찮아!
    for(int i=0; i<size; ++i) {
        arr[i] *= 2;
    }
}

int main() {
    int arr[5] = {1, 2, 3, 4, 5};
    ProcessArray(arr, 5); // 실수로 4나 6을 넣으면? 💥 버그 발생!
}
```
*   **귀찮음**: 매개변수가 2개씩 필요함.
*   **위험함**: `size`를 잘못 넘기면 바로 메모리 오염.
*   **불유연함**: `std::vector`를 넘기려니 `vector` 헤더가 필요하고, 배열은 또 따로 짜야 하고...

---

## 2. 해결책: `std::span` (Memory View)
C++20에서 도입된 `std::span`은 **"메모리의 특정 구간을 바라보는 창문"**입니다.
데이터를 **소유(Own)**하지 않고, 단지 **바라보기(View)**만 합니다. (복사 비용 0)

### 2.1 특징
1.  **가볍다**: 내부적으로 `(포인터, 길이)` 딱 2개만 들고 다닙니다.
2.  **유연하다**: 배열(`[]`), `std::vector`, `std::array` 등 **연속된 메모리**라면 무엇이든 받을 수 있습니다.
3.  **안전하다**: 길이를 알고 있으므로 범위 기반 for문(`for(auto n : span)`)을 쓸 수 있습니다.

### 2.2 사용법

```cpp
#include <span> // C++20 필수

// 배열이든 벡터든 다 들어와! (단, 연속된 메모리여야 함)
void PrintData(std::span<int> data) {
    // size()를 내장하고 있음!
    std::cout << "Size: " << data.size() << "\n";
    
    // 범위 기반 for문 가능!
    for (int n : data) {
        std::cout << n << " ";
    }
    std::cout << "\n";
}

int main() {
    int arr[] = {1, 2, 3};
    std::vector<int> vec = {4, 5, 6, 7};
    
    PrintData(arr); // 포인터+길이 자동 변환 ✨
    PrintData(vec); // 벡터도 자동 변환 ✨
}
```

---

## 3. 핵심 기능: 부분 창문 (Subspan)
기존에는 배열의 중간부터 3개를 자르려면 포인터 연산을 머리 터지게 해야 했습니다.
`span`은 `subspan`으로 아주 쉽게 자를 수 있습니다.

```cpp
std::span<int> all = arr;
std::span<int> part = all.subspan(1, 2); // 인덱스 1부터 2개만 봐라
// part는 {2, 3}을 가리킴. (복사 아님! 원본을 가리킴)
```

> **주의**: `std::span`은 **데이터를 소유하지 않습니다.**
> 만약 `span`이 가리키는 원본 데이터(`vector` 등)가 삭제되면, `span`은 **댕글링 상태**가 됩니다.
> (따라서 `span`은 주로 **함수 매개변수**로 사용할 때 가장 빛을 발합니다.)
