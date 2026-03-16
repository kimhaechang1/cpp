# 0x07 덱 (Deque) 심화: Node 기반 시각화 (Doubly Linked List)

정적 배열(Array) 방식을 이해하셨다면, 이번에는 **메모리가 불연속적으로 할당되는 연결 리스트(Linked List)** 방식으로 덱을 바라봅시다.
자료구조를 깊이 이해하기 위해 두 가지 방식을 모두 알아두는 것은 매우 훌륭한 접근입니다! 🔥

## 1. 정적 배열 vs 연결 리스트 (Data-Oriented 관점)

- **정적 배열 (Array)**
  - **장점**: 캐시 히트율(Cache Hit Ratio)이 매우 높습니다. 데이터가 메모리 상에 연달아 붙어있으므로 CPU가 미리 데이터를 캐시로 가져오기(Prefetch) 매우 좋습니다. 배열 중간에서 시작하는 꼼수만 쓰면 최고로 빠릅니다.
  - **단점**: 배열의 최대 크기를 미리 정해야 합니다. (메모리 낭비 발생)
- **Node 기반 (Doubly Linked List)**
  - **장점**: 최대 크기를 정할 필요가 없습니다. 필요할 때마다 동적으로 할당(new)하면 됩니다.
  - **단점**: 메모리 상에 파편화(Fragmentation)되어 흩어져 있으므로 캐시 공간 참조 지역성(Locality)이 떨어집니다. 또한 빈번한 `new`/`delete`는 심각한 성능 저하를 일으킬 수 있습니다.

---

## 2. 노드(Node)의 구조 설계

덱은 앞/뒤 양방향으로 움직여야 하므로, **이전 노드를 가리키는 포인터(`prev`)**와 **다음 노드를 가리키는 포인터(`next`)**가 모두 필요합니다. 이를 **이중 연결 리스트 (Doubly Linked List)**라고 부릅니다.

```cpp
struct Node {
    int data;
    Node* prev; // 내 앞쪽(머리 쪽)에 있는 노드
    Node* next; // 내 뒤쪽(꼬리 쪽)에 있는 노드
    
    // 생성자 (Module 6에서 배운 Uniform Initialization 활용에 유용)
    Node(int d) : data(d), prev(nullptr), next(nullptr) {}
};
```

---

## 3. 포인터(Pointer)의 역할: `head` 와 `tail`

정적 배열에서의 인덱스와는 달리, 여기서는 실제 메모리 주소를 가리키는 **포인터**가 필요합니다.

```cpp
Node* head = nullptr; // 덱의 가장 앞쪽(첫 번째) 노드를 가리킴
Node* tail = nullptr; // 덱의 가장 뒤쪽(마지막) 노드를 가리킴
int dequeSize = 0;    // 원소의 개수를 빠르게 추적하기 위한 변수
```

*   **비어있을 때 (Empty)**: `head`와 `tail`이 모두 `nullptr` 인 상태입니다.
*   **원소가 1개일 때**: `head`와 `tail`이 **같은 노드 1개**를 동시에 가리킵니다.

---

## 4. 삽입 동작 원리 (Push)

새로운 노드를 동적 할당(`new`)한 뒤, 기존 포인터들의 '연결 고리'를 다시 이어주는 과정입니다.
가장 중요한 엣지 케이스는 **"덱이 비어있을 때 삽입하는 경우"** 입니다. 이때는 `head`와 `tail` 모두가 갱신되어야 합니다.

### A. 앞으로 삽입 (`push_front`)

1. 새로운 노드 `newNode`를 만듭니다. (`newNode->data = X`)
2. 텅 빈 상태(`head == nullptr`)라면?
   - 이 노드가 유일한 노드이므로 `head = newNode; tail = newNode;`가 됩니다.
3. 기존 노드가 있다면?
   - `newNode->next = head;` (새 노드의 다음을 기존 머리로 연결)
   - `head->prev = newNode;` (기존 머리의 이전을 새 노드로 연결)
   - `head = newNode;` (이제 머리를 새 노드로 갱신)
4. `dequeSize++`

### B. 뒤로 삽입 (`push_back`)

1. 새로운 노드 `newNode`를 만듭니다.
2. 텅 빈 상태라면? `push_front`와 동일하게 `head`와 `tail`이 이 노드를 가리킵니다.
3. 기존 노드가 있다면?
   - `tail->next = newNode;` (기존 꼬리의 다음을 새 노드로 연결)
   - `newNode->prev = tail;` (새 노드의 이전을 기존 꼬리로 연결)
   - `tail = newNode;` (이제 꼬리를 새 노드로 갱신)

---

## 5. 삭제 동작 원리 (Pop)

삭제는 더 이상 필요 없어진 노드를 해제(`delete`)하고, 끊어진 '연결 고리'를 이어주는 과정입니다.
가장 중요한 엣지 케이스는 **"노드가 1개 남았을 때 삭제하는 경우"** 입니다. 이때 삭제하고 나면 덱은 텅 빈 상태(nullptr)가 되어야 합니다.

### A. 앞에서 삭제 (`pop_front`)

1. 뺄 값이 없으면 무시하거나 -1을 리턴합니다.
2. 현재 `head`가 가리키는 노드를 임시 포인터 `temp`에 저장합니다. (나중에 `delete` 하기 위함)
3. 만약 노드가 딱 1개뿐이었다면 (`head == tail`)?
   - 지우고 나면 비어있어야 하므로 `head = nullptr; tail = nullptr;`
4. 노드가 여러 개라면?
   - `head = temp->next;` (머리를 다음 노드로 옮김)
   - `head->prev = nullptr;` (새 머리의 앞은 이제 없으므로 끊어줌)
5. `delete temp; dequeSize--;`

### B. 뒤에서 삭제 (`pop_back`)

1. 뺄 값이 없으면 무시합니다.
2. 현재 `tail`이 가리키는 노드를 임시 포인터 `temp`에 백업합니다.
3. 원소가 1개라면? 앞과 마찬가지로 둘 다 `nullptr`로 만듭니다.
4. 원소가 여러 개라면?
   - `tail = temp->prev;` (꼬리를 하나 앞 노드로 옮김)
   - `tail->next = nullptr;` (새 꼬리의 뒤는 이제 없으므로 끊어줌)
5. `delete temp; dequeSize--;`

---

## 6. 완성 구현 코드 (Node/Doubly Linked List 기반)

```cpp
#include<iostream>
#include<string>
#include<sstream>

using namespace std;

struct Node {
    int data;
    Node* prev;
    Node* next;
    Node(int d) : data(d), prev(nullptr), next(nullptr) {}
};

Node* head = nullptr;
Node* tail = nullptr;
int dequeSize = 0;

// 현재 원소 개수
int Size() {
    return dequeSize;
}

// 비어있으면 1, 아니면 0
int Empty() {
    return (head == nullptr) ? 1 : 0;
}

// 맨 앞 원소 조회 (없으면 -1)
int Front() {
    if (Empty() == 1) return -1;
    return head->data;
}

// 맨 뒤 원소 조회 (없으면 -1)
int Back() {
    if (Empty() == 1) return -1;
    return tail->data;
}

// 새 노드를 덱의 맨 앞에 삽입
void Push_Front(int x) {
    Node* newNode = new Node(x);
    if (Empty() == 1) {
        // 첫 번째 원소: head와 tail 모두 이 노드를 가리킴
        head = newNode;
        tail = newNode;
    }
    else {
        newNode->next = head;  // 새 노드의 다음 -> 기존 머리
        head->prev = newNode;  // 기존 머리의 이전 -> 새 노드
        head = newNode;        // 머리를 새 노드로 갱신
    }
    dequeSize++;
}

// 새 노드를 덱의 맨 뒤에 삽입
void Push_Back(int x) {
    Node* newNode = new Node(x);
    if (Empty() == 1) {
        head = newNode;
        tail = newNode;
    }
    else {
        tail->next = newNode;  // 기존 꼬리의 다음 -> 새 노드
        newNode->prev = tail;  // 새 노드의 이전 -> 기존 꼬리
        tail = newNode;        // 꼬리를 새 노드로 갱신
    }
    dequeSize++;
}

// 맨 앞 원소를 제거하고 그 값을 반환
int Pop_Front() {
    if (Empty() == 1) return -1;
    int rs = head->data;
    Node* temp = head;
    if (head == tail) {
        // 원소가 딱 1개였다면 지우고 나면 완전히 비어야 함
        head = nullptr;
        tail = nullptr;
    }
    else {
        head = temp->next;    // 머리를 다음 노드로 이동
        head->prev = nullptr; // 새 머리의 이전 링크 끊기
    }
    delete temp;
    dequeSize--;
    return rs;
}

// 맨 뒤 원소를 제거하고 그 값을 반환
int Pop_Back() {
    if (Empty() == 1) return -1;
    int rs = tail->data;
    Node* temp = tail;
    if (head == tail) {
        head = nullptr;
        tail = nullptr;
    }
    else {
        tail = temp->prev;    // 꼬리를 이전 노드로 이동
        tail->next = nullptr; // 새 꼬리의 다음 링크 끊기
    }
    delete temp;
    dequeSize--;
    return rs;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int N;
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

### 엣지 케이스 핵심 요약

- **`Push` 시**: 덱이 비어있으면(`head == nullptr`) `head`와 `tail`을 **모두** 새 노드로 갱신
- **`Pop` 시**: 노드가 1개만 남았으면(`head == tail`) 제거 후 `head`와 `tail`을 **모두** `nullptr`로

---

## 💡 ALGO COACH 생각 정리
배열과 노드 기반, 양쪽 원리를 모두 살펴보았습니다!
알고리즘 코딩 테스트 시간 제한(Time Limit)의 엄격함을 고려한다면 `new`와 `delete` 연산 비용이 없는 **정적 배열** 방식이 매우 유리합니다. 하지만 메모리 풀(Memory Pool) 같은 시스템을 짤 때는 이 **노드 연결 조작** 개념이 뼈대가 됩니다.
