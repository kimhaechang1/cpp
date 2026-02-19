# 0x05 스택 (Stack)

## 1. 📚 정의
- **LIFO (Last-In, First-Out)**: 나중에 들어간 원소가 가장 먼저 나옵니다.
- **비유**: 프링글스 통, 접시 쌓기, 웹 브라우저 뒤로 가기.

## 2. ⚡ 시간 복잡도 (Time Complexity)
스택의 핵심은 **모든 연산이 O(1)** 이라는 점입니다.
- **push (삽입)**: $O(1)$
- **pop (삭제)**: $O(1)$
- **top (확인)**: $O(1)$

> **주의**: 특정 원소를 찾거나(Items[i]), 중간에 삽입하는 건 스택의 역할이 아닙니다. (굳이 하려면 $O(N)$)

## 3. 🛠️ 구현 방법: 배열 vs 연결 리스트
우리는 **배열(Array)** 기반 스택을 먼저 구현합니다.

### 🏆 배열 기반 스택 (Why Recommended?)
- **Cache Locality (캐시 지역성)**: 데이터가 메모리에 연속적으로 배치되므로 CPU 캐시 적중률이 높습니다.
- **Overhead**: 연결 리스트는 노드마다 포인터(`next`)를 저장해야 하므로 추가 메모리(Overhead)가 큽니다.
- **구현 난이도**: 배열과 인덱스(`pos`) 하나만 있으면 되어 매우 간단합니다.

### 구조
```cpp
const int MX = 1000005; 
int dat[MX]; // 데이터를 담을 배열
int pos = 0; // 다음 원소가 들어갈 인덱스 (현재 원소 개수와 동일)
```

## 4. 📝 핵심 연산 로직

1.  **push(x)**: `dat[pos]`에 x를 넣고, `pos`를 1 증가시킵니다.
    ```cpp
    void push(int x) { dat[pos++] = x; }
    ```

2.  **pop()**: `pos`를 1 감소시킵니다. (데이터를 굳이 지울 필요 없음, 덮어씌워짐)
    ```cpp
    void pop() { pos--; }
    ```

3.  **top()**: `dat[pos-1]`을 반환합니다.
    ```cpp
    int top() { return dat[pos-1]; }
    ```

## 5. 🔍 STL Stack (`<stack>`)
실전(코딩테스트)에서는 직접 구현보다 `std::stack`을 주로 씁니다.

### 5.1 기본 메서드
```cpp
#include <stack>

std::stack<int> s;

// 1. 삽입 (Push)
s.push(10); // [10]
s.push(20); // [10, 20]

// 2. 삭제 (Pop) - 반환값 없음!
s.pop();    // [10] (20 삭제)

// 3. 최상단 원소 확인 (Top)
int val = s.top(); // 10

// 4. 상태 확인
if(s.empty()) cout << "비어있음";
cout << s.size(); // 1
```

### 5.2 ⚠️ 주의사항 (Runtime Error)
C++ STL Stack은 **비어있을 때 `top()`이나 `pop()`을 호출하면 프로그램이 뻗습니다(Crash).**
항상 `!s.empty()`를 먼저 확인해야 합니다.

```cpp
if(!s.empty()) {
    cout << s.top();
    s.pop();
}
```
