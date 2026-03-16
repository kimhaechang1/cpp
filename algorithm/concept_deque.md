# 0x07 덱 (Deque) 개념 정리

## 1. 덱(Deque)이란?
**Double-Ended Queue**의 약자입니다.
Queue(큐)가 한쪽으로 넣고 반대쪽으로 빼는 파이프라면, Deque(덱)은 **양 끝(앞, 뒤) 모두에서 삽입과 삭제가 가능**한 매우 유연한 파이프입니다.

## 2. 왜 알아야 할까? (게임 개발자 관점)
실제 게임 엔진이나 클라이언트에서는 유연한 선형 버퍼가 필요할 때가 많습니다.
- **최대 보유 아이템 로그**: 최근 습득한 아이템 로그를 10개만 화면에 유지할 때 (새로운 게 뒤로 들어오면, 오래된 앞의 것을 뺌)
- **경로 탐색 (A* 등)**: 큐와 스택의 특성이 모두 필요할 때
- 물론 실무에서는 C++ STL의 `std::deque`를 사용하지만, 내부가 어떤 형태의 Array chunk들로 이루어져 캐시 히트율(Cache Hit Ratio)을 높이는지는 직접 메모리(배열)를 통해 구현해봐야 완전히 체감할 수 있습니다.

---

## 3. 핵심 아이디어: "시작점을 정중앙에 두자!" (배열 기반 구현)

일반적인 배열 기반 스택이나 큐를 만들 때 데이터의 시작점을 `index 0`으로 두었을 것입니다.

**문제 상황 발생:**
만약 덱의 시작점을 `Index 0`으로 둔다면?
- 뒤로 넣는 것(`push_back`)은 인덱스를 계속 증가시키면 되니 괜찮습니다. (1, 2, 3...)
- **하지만 앞으로 넣는 것(`push_front`)은?** (인덱스를 감소시켜야 하므로 `-1`, `-2`가 되어버려 **배열의 범위를 벗어나게(Index Out of Bounds)** 됩니다.)

**💡 데이터 지향 해결책 (Data-Oriented Solution):**
앞으로만 데이터가 N번 들어오거나, 뒤로만 데이터가 N번 들어오는 최악의 경우를 모두 커버할 수 있도록 **가운데에서부터 뻗어 나가는 방식**을 취합니다.

만약 문제에서 주어지는 최대 명령 수가 `10,000`번이라면, 최대로 늘어날 수 있는 길이는 `10,000`입니다.
따라서 배열을 `양측 + 여유분`인 넉넉한 크기(`2 * 10000 + 1`)로 만들고, **시작(head, tail)을 배열의 중간 지점에서 출발**시키는 것입니다.

```cpp
#include <iostream>
using namespace std;

// 최대 명령 수 (문제에서 10,000 주어짐)
const int MX = 10000;

// 양쪽으로 최대 MX만큼 뻗어나갈 수 있도록 2*MX + 1 크기의 배열 할당
int dat[2 * MX + 1]; 

// 초기 상태: 가운데에서 함께 시작
int head = MX, tail = MX;
```

---

## 4. 인덱스 포인터 이동 로직 (`head`, `tail`)

큐(Queue)와 마찬가지로 다음을 원칙으로 삼으세요.
*   **`head`**: 덱의 가장 아랫부분(가장 앞, 제일 먼저 들어간 녀석)의 진짜 원소가 있는 인덱스
*   **`tail`**: 가장 뒷부분(최근에 들어간 녀석)의 **다음 칸(들어갈 빈 공간)** 인덱스

### A. 앞쪽에 관련된 동작 (Front)

**1) 앞으로 넣기 (`push_front`)**
- 배열의 앞쪽 여유 공간으로 한 칸 전진(`head`를 왼쪽으로 1 감소)한 후 그 자리에 데이터를 넣습니다.
- `head--;` -> `dat[head] = 데이터;`

**2) 앞에서 빼기 (`pop_front`)**
- `head`가 오른쪽으로 한 칸 밀려나면 기존의 앞부분 데이터를 없는 취급하게 됩니다.
- `head++;`

**3) 맨 앞의 값 확인 (`front`)**
- 현재 `head`는 진짜 원소가 있는 위치이므로 `dat[head]`

### B. 뒤쪽에 관련된 동작 (Back)

**1) 뒤로 넣기 (`push_back`)**
- 현재 `tail`은 "다음에 아이템이 들어갈 빈 공간"을 가리키고 있으므로 바로 `dat[tail] = 데이터`를 대입합니다.
- 대입을 마친 후 다음 공간 확보를 위해 `tail`을 1 증가(오른쪽 이동)시킵니다.
- `dat[tail] = 데이터;` -> `tail++;`

**2) 뒤에서 빼기 (`pop_back`)**
- 현재 `tail`은 빈 칸이므로, `tail`을 왼쪽으로 1 감소시키면 가장 최근에 들어온 녀석의 위치가 됩니다.
- `tail--;` 

**3) 맨 뒤의 값 확인 (`back`)**
- 진짜 맨 뒤의 원소는 `tail - 1` 위치에 있습니다.
- `dat[tail - 1]`

---

## 5. 상태 확인 (Status)

*   **현재 원소의 개수 (크기, `size`)**
    *   **공식:** `tail - head`
*   **비어있는지 확인 (`empty`)**
    *   **조건:** `head == tail` 이면 비어있음. (원소가 없으므로 둘이 만나게 됨)

---

## 6. 완성 구현 코드 (배열 기반, 회창 ver.)

회창님이 직접 구현하신 10866번 코드입니다.
배열의 중간(`SIZE`)에서 시작하는 핵심 아이디어와 8가지 기능 모두를 완벽하게 담고 있습니다.

```cpp
#include<iostream>
#include<string>
#include<sstream>

using namespace std;

static const int SIZE = 10000;
int deq[SIZE * 2 + 5];
int head = SIZE;
int tail = SIZE;

// 현재 원소 개수
int Size() {
    return tail - head;
}

// 비어있으면 1, 아니면 0
int Empty() {
    if (Size() == 0) {
        return 1;
    }
    return 0;
}

// head를 왼쪽으로 한 칸 이동 후 그 자리에 저장
void Push_Front(int x) {
    head--;
    deq[head] = x;
}

// tail 위치에 저장 후 tail을 오른쪽으로 한 칸 이동
void Push_Back(int x) {
    deq[tail] = x;
    tail++;
}

// head 위치가 곧 맨 앞 원소
int Front() {
    if (Empty() == 1) {
        return -1;
    }
    return deq[head];
}

// 실제 맨 뒤 원소는 tail - 1 위치
int Back() {
    if (Empty() == 1) {
        return -1;
    }
    return deq[tail - 1];
}

// 맨 앞 값을 반환하고 head를 오른쪽으로 한 칸 밀어내 원소를 논리적으로 제거
int Pop_Front() {
    if (Empty() == 1) {
        return -1;
    }
    int rs = Front();
    head++;
    return rs;
}

// 맨 뒤 값을 반환하고 tail을 왼쪽으로 한 칸 밀어내 원소를 논리적으로 제거
int Pop_Back() {
    if (Empty() == 1) {
        return -1;
    }
    int rs = Back();
    tail--;
    return rs;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin >> N;
    stringstream stream;
    while (N-- > 0) {
        string op;
        cin >> op;
        int x;
        if (op == "push_back") {
            cin >> x;
            Push_Back(x);
        }
        else if (op == "push_front") {
            cin >> x;
            Push_Front(x);
        }
        else if (op == "front") {
            stream << Front() << "\n";
        }
        else if (op == "back") {
            stream << Back() << "\n";
        }
        else if (op == "pop_front") {
            stream << Pop_Front() << "\n";
        }
        else if (op == "pop_back") {
            stream << Pop_Back() << "\n";
        }
        else if (op == "size") {
            stream << Size() << "\n";
        }
        else if (op == "empty") {
            stream << Empty() << "\n";
        }
    }
    cout << stream.str();
}
```

### 핵심 패턴 요약

- `Push_Front`: `head--` 먼저, 그 다음 대입 (`head`는 항상 실제 원소의 인덱스)
- `Push_Back`: 대입 먼저, 그 다음 `tail++` (`tail`은 항상 다음에 들어올 빈 공간의 인덱스)
- `Pop_Front`: 값을 읽은 후 `head++` (실제 메모리를 지우지 않아도 논리적 제거)
- `Pop_Back`: 값을 읽은 후 `tail--`
- `Size`: `tail - head`
- `Empty`: `head == tail`
