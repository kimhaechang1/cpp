# 0x07 덱 (Deque) STL 사용법: std::deque

## 1. 왜 std::deque인가?
자료구조 원리(배열/노드 직접 구현)를 완전히 이해했다면, 이제 실무에서 사용하는 **C++ STL의 `std::deque`** 를 편하게 쓸 자격이 생겼습니다!

`std::deque`는 내부적으로 고정 크기의 `chunk(청크)` 단위 배열들을 관리하는 방식으로 구현되어 있습니다.
완전한 연속 메모리(Array)와 단순 연결 리스트(Linked List)의 중간 어딘가에 위치한 자료구조입니다.

- **`std::vector`와의 차이**: 벡터는 앞(front)에 원소를 삽입/삭제할 때 O(N)이 걸리지만, `deque`는 O(1)입니다.
- **`std::list`와의 차이**: 리스트는 완전 불연속 메모리라 캐시 미스가 많지만, `deque`는 청크 단위로 연속이라 일반적으로 더 빠릅니다.

---

## 2. 헤더 파일 및 선언

```cpp
#include<deque>
using namespace std;

deque<int> dq; // 정수형 덱 선언
```

---

## 3. STL deque의 8가지 핵심 함수

### 삽입 (Push)

```cpp
dq.push_front(10); // 맨 앞에 10 삽입 -> [10]
dq.push_back(20);  // 맨 뒤에 20 삽입 -> [10, 20]
dq.push_front(5);  // 맨 앞에 5 삽입  -> [5, 10, 20]
```

### 삭제 (Pop / Erase)

`pop_front`와 `pop_back`은 **값을 반환하지 않고** 그냥 원소를 지운다는 점에 주의하세요.
값을 먼저 꺼낸 뒤 delete하는 우리의 직접 구현과는 순서가 반대입니다!

```cpp
// 값을 꺼내고 싶다면 front() / back()으로 먼저 읽은 후 pop 해야 합니다.
int frontVal = dq.front(); // 5 (읽기만 함, 제거 안 됨)
dq.pop_front();            // 5 제거 -> [10, 20]

int backVal = dq.back();   // 20 (읽기만 함)
dq.pop_back();             // 20 제거 -> [10]
```

### 조회 (Front / Back)

```cpp
dq.front(); // 맨 앞 원소 반환 (덱이 비어있으면 undefined behavior!)
dq.back();  // 맨 뒤 원소 반환 (덱이 비어있으면 undefined behavior!)
```

### 상태 확인 (Size / Empty)

```cpp
dq.size();  // 현재 원소 개수 (반환 타입: size_t, 즉 unsigned)
dq.empty(); // 비어있으면 true, 아니면 false (int 1/0이 아님에 주의)
```

---

## 4. 직접 구현과의 비교

| 직접 구현 함수 | STL 함수 | 반환값의 차이 |
|:---|:---|:---|
| `Push_Front(x)` | `dq.push_front(x)` | 없음 |
| `Push_Back(x)` | `dq.push_back(x)` | 없음 |
| `Pop_Front()` | `dq.pop_front()` | STL은 void (값 반환 없음) |
| `Pop_Back()` | `dq.pop_back()` | STL은 void (값 반환 없음) |
| `Front()` | `dq.front()` | 같음 |
| `Back()` | `dq.back()` | 같음 |
| `Size()` | `dq.size()` | STL은 size_t (부호 없는 정수) |
| `Empty()` (1/0) | `dq.empty()` | STL은 bool (true/false) |

**가장 중요한 차이**: STL의 `pop_front()` / `pop_back()`은 아무 값도 리턴하지 않습니다.
값을 꺼내려면 반드시 `front()` / `back()`을 먼저 호출해서 저장해 두어야 합니다.

```cpp
// 올바른 패턴
int val = dq.front();
dq.pop_front();

// 틀린 패턴 (컴파일 오류)
// int val = dq.pop_front(); // pop_front()는 void!
```

---

## 5. 반복자(Iterator)와 인덱스 접근

`std::deque`는 `std::vector`와 마찬가지로 `[]` 연산자를 통한 인덱스 접근을 O(1)로 지원합니다.
(단순 연결 리스트인 `std::list`는 `[]`가 불가능합니다.)

```cpp
deque<int> dq = {1, 2, 3, 4, 5};
cout << dq[2]; // 3 출력

// 범위 기반 for문도 사용 가능
for (int val : dq) {
    cout << val << " "; // 1 2 3 4 5
}
```

---

## 6. 10866번 문제에 std::deque를 쓰면?

STL을 쓸 수 있다면 코드가 훨씬 짧아집니다. (직접 구현과의 차이를 비교해 보세요!)

```cpp
#include<iostream>
#include<deque>
#include<string>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int N;
    cin >> N;
    deque<int> dq;
    while (N-- > 0) {
        string op;
        cin >> op;
        if (op == "push_back") {
            int x; cin >> x;
            dq.push_back(x);
        }
        else if (op == "push_front") {
            int x; cin >> x;
            dq.push_front(x);
        }
        else if (op == "front") {
            cout << (dq.empty() ? -1 : (int)dq.front()) << "\n";
        }
        else if (op == "back") {
            cout << (dq.empty() ? -1 : (int)dq.back()) << "\n";
        }
        else if (op == "pop_front") {
            if (dq.empty()) { cout << -1 << "\n"; }
            else { cout << dq.front() << "\n"; dq.pop_front(); }
        }
        else if (op == "pop_back") {
            if (dq.empty()) { cout << -1 << "\n"; }
            else { cout << dq.back() << "\n"; dq.pop_back(); }
        }
        else if (op == "size") {
            cout << dq.size() << "\n";
        }
        else if (op == "empty") {
            cout << (dq.empty() ? 1 : 0) << "\n";
        }
    }
}
```
