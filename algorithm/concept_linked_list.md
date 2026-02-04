# 0x04 연결 리스트 (Linked List)

## 1. 연결 리스트의 정의
연결 리스트는 원소들을 **줄줄이 엮어서** 관리하는 자료구조입니다.
배열(Array)이 원소들을 메모리상에 **연속적**으로 둔다면, 연결 리스트는 여기저기 흩어진 원소들을 **주소(Reference)**로 연결합니다.

### 🆚 배열 vs 연결 리스트

| 특징 | 배열 (Array) | 연결 리스트 (Linked List) |
| :-- | :-- | :-- |
| **메모리 구조** | 연속적 (Contiguous) | 비연속적 (Scattered) |
| **k번째 원소 접근** | **O(1)** (Random Access) | **O(N)** (하나씩 따라가야 함) |
| **임의 위치 추가/제거** | **O(N)** (뒤의 원소들을 밀어야 함) | **O(1)** (주소만 바꾸면 됨) * |
| **메모리 오버헤드** | 적음 (데이터만 저장) | 큼 (데이터 + 다음 주소 저장) |
| **Cache Hit Rate** | 높음 (공간 지역성 좋음) | 낮음 (뚝뚝 끊겨 있음) |

> *: 단, 추가/제거할 위치의 주소(Address)를 알고 있을 때만 O(1)입니다.

---

## 2. 구현 방식 (PS vs 개발)

실제 개발(Prodution)에서는 `struct Node`를 만들고 `new`로 동적 할당하여 노드를 연결하지만,
**알고리즘 문제 풀이(PS)**에서는 **시간 제한**과 **구현 편의**를 위해 **배열 기반 연결 리스트(Array Implementaion)**를 주로 사용합니다.

### 💾 1. 야매 연결 리스트 (Array Implementation for PS)

**특징**:
- `new` 동적 할당 오버헤드가 없음 (빠름)
- 인덱스(int)로 접근하므로 디버깅이 편함
- **단점**: 사용하지 않아도 미리 `MX` 만큼 배열을 잡아야 해서 메모리 낭비가 있을 수 있음.

`new` 할당 시간(Overhead)을 줄이고, 인덱스로 빠르게 접근하기 위해 3개의 배열을 사용합니다.

```cpp
const int MX = 1000005; // 최대 원소 수
int dat[MX]; // Data: 값을 저장
int pre[MX]; // Previous: 이전 노드의 인덱스
int nxt[MX]; // Next: 다음 노드의 인덱스
int unused = 1; // 새로운 노드를 생성할 때 사용할 인덱스
```

---

### 🏗️ 2. 정석 연결 리스트 (Struct Node Implementation)

문법 교과서나 실제 시스템 프로그래밍에서 사용하는 방식입니다.
필요할 때마다 `new`로 메모리를 할당하므로 **메모리를 효율적**으로 씁니다.
하지만 `new` 자체가 시스템 콜을 동반하여 느리고, 메모리가 여기저기 흩어져(Fragmentation) **Cache Miss**가 발생할 확률이 높습니다.

```cpp
struct Node {
    int data;
    Node* next; // 다음 노드의 주소를 가리킴 (없으면 nullptr)
};
```

---

## 3. 비교: PS(배열) vs 개발(Struct)

| 비교 항목 | 배열 기반 (Array) | 구조체 기반 (Struct Pointer) |
| :--- | :--- | :--- |
| **메모리 사용** | 미리 최대치(MX) 할당 (낭비 가능) | 필요한 만큼만 할당 (효율적) |
| **할당 속도** | `idx++` (매우 빠름) | `new Node` (느림) |
| **캐시 효율 (Locality)** | 좋음 (배열은 연속적) | 나쁨 (힙 메모리에 산재) |
| **구현 난이도** | 쉬움 (인덱스 조작) | 보통 (포인터, nullptr 체크 필요) |
| **주 사용처** | **알고리즘 문제 풀이 (Time Limit)** | **운영체제 커널, 일반 SW 개발** |

---

## 4. 핵심 기능 설명 (공통 개념)

### 1) traverse (순회)
0번지(Head)에서 출발하여 `nxt`를 타고 끝(`-1`)까지 이동합니다.

### 2) insert(addr, val)
`addr` 번지 **뒤에** `val` 값을 가진 새로운 노드를 추가합니다.
1. `unused`에서 새 번지를 가져옵니다 (`new_node`).
2. `dat[new_node]`에 값을 넣습니다.
3. **Link 연결 (4 steps)**:
   - 새 노드의 pre/nxt 설정
   - `addr`의 nxt가 가리키던 놈의 pre를 새 노드로 변경
   - `addr`의 nxt를 새 노드로 변경

### 3) erase(addr)
`addr` 번지의 노드를 삭제합니다. 논리적으로 연결만 끊으면 됩니다.
1. `addr`의 이전 노드(`pre[addr]`)의 `nxt`를 `addr`의 다음 노드(`nxt[addr]`)로 바꿉니다.
2. `addr`의 다음 노드(`nxt[addr]`)의 `pre`를 `addr`의 이전 노드(`pre[addr]`)로 바꿉니다.

---


---

### 🚀 3. 하이브리드: 메모리 풀 (Memory Pool / Object Pool)

> **User's Insight**: "Struct 배열을 활용하여 미리 메모리 풀을 만들어서 연결 리스트를 하는 방법도 있음"

정확합니다! 🔥 **게임 서버나 엔진**에서 가장 사랑받는 패턴입니다.
`Struct`의 편리함과 `Array`의 성능(Cache Locality + No Alloc Overhead)을 모두 잡는 **최강의 기법**입니다.

```cpp
struct Node {
    int data;
    Node* next;
};

// 1. 미리 거대한 메모리(Pool)를 잡아둡니다. (Heap 할당 X)
Node pool[10000]; 
int pool_idx = 0; // 다음에 사용할 노드의 인덱스

// 2. new 대신 사용할 함수 (O(1))
Node* my_alloc(int val) {
    if (pool_idx >= 10000) return nullptr; // Pool Full
    Node* node = &pool[pool_idx++];
    node->data = val;
    node->next = nullptr;
    return node;
}
```

이 방식은:
1.  **No `new`/`delete`**: 할당/해제 오버헤드가 **0**에 가깝습니다.
2.  **Cache Friendly**: 모든 노드가 `pool` 배열 안에 모여 있으므로 캐시 적중률이 높습니다.
3.  **Safety**: 댕글링 포인터가 발생해도(잘못 참조해도) 적어도 우리 프로그램 메모리 안입니다.

---

## 4. 비교 요약 (Ultimate)

| 방식 | Array (PS) | Pointer (Classic) | Memory Pool (Engine) |
| :--- | :--- | :--- | :--- |
| **개념** | `data[i], nxt[i]` | `new Node` | `pool[i]` |
| **속도** | 🚀 최상 | 🐢 느림 (`new`) | 🚀 최상 |
| **메모리 효율** | 📉 낭비 (Sparse) | 📈 최적 (Compact) | 📉 낭비 (미리 할당) |
| **캐시 효율** | ⭐️ 좋음 | ⛈ 나쁨 (파편화) | ⭐️ 좋음 |
| **사용처** | 알고리즘 대회 | 일반 애플리케이션 | **게임 엔진, 서버, 임베디드** |

---

## 5. 실습 안내

### STEP 1: 배열 기반 (PS 스타일)
`impl/linked_list_array_practice.cpp`
- 인덱스를 이용한 빠른 구현을 연습합니다.

### STEP 2: 포인터 기반 (정석 스타일)
`impl/linked_list_pointer_practice.cpp`
- `new`와 `delete`를 신경 써야 합니다.

### STEP 3: 메모리 풀 (Advanced)
`impl/linked_list_pool_practice.cpp`
- 직접 메모리 관리자(Allocator)를 흉내 내봅니다.

---

## ❓ FAQ: "보통은 값을 찾아서 넣지 않나요?"

> "보통 `search` 함수로 특정 값이 있는 노드를 찾고, 그 노드 기준으로 넣지 않나요?"

맞습니다! 일반적인 비즈니스 로직에서는 "값"을 찾는 게 우선입니다.

하지만 **Linked List의 존재 이유**를 생각해보면 조금 다릅니다.
- **Search (값 찾기)**: **O(N)**. 처음부터 끝까지 뒤져야 함. (배열과 다를 바 없음 [인덱스 X])
- **Insert (위치 알 때 삽입)**: **O(1)**. 연결만 툭 바꾸면 됨. (배열은 O(N) [밀어내기])

만약 매번 `Search`(`O(N)`)를 하고 `Insert`를 한다면, 전체 성능은 결국 **O(N)**이 되어 연결 리스트를 쓰는 장점이 사라집니다.

그래서 Linked List가 빛을 발하는 순간은 **"이미 위치를 알고 있을 때"**입니다.
예를 들어:
1.  **에디터의 커서**: 사용자가 커서를 이동시키며 타이핑할 때 (위치는 커서가 알고 있음)
2.  **LRU 캐시**: 맨 앞/맨 뒤에만 넣고 뺄 때
3.  **웹 브라우저 뒤로 가기**: 현재 페이지(Current) 기준으로 앞/뒤 이동

따라서 우리가 구현하는 `insert`는 "위치를 아는 상태"를 가정한 **Primitive(기초) 연산**이고,
필요하다면 `find(val)` 함수를 따로 만들어 조합해서 씁니다.


