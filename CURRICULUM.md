# C++ 알고리즘/자료구조 학습 커리큘럼

## 📚 학습 로드맵 (총 16단계)

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                          🎯 BEGINNER (입문)                                 │
├─────────────────────────────────────────────────────────────────────────────┤
│  STEP 0: C++ 기초 문법 ← 현재 단계                                          │
│  STEP 1: 조건문과 반복문                                                    │
│  STEP 2: 배열과 문자열                                                      │
│  STEP 3: 함수와 재귀 기초                                                   │
├─────────────────────────────────────────────────────────────────────────────┤
│                          📦 INTERMEDIATE (중급)                             │
├─────────────────────────────────────────────────────────────────────────────┤
│  STEP 4: 기초 STL (vector, string, pair)                                   │
│  STEP 5: 선형 자료구조 직접 구현 (스택, 큐, 덱, 연결리스트)                   │
│  STEP 6: STL 컨테이너 심화 (stack, queue, deque, list)                     │
│  STEP 7: 정렬 알고리즘 (버블, 선택, 삽입, 퀵, 머지)                          │
│  STEP 8: 탐색 알고리즘 (이분 탐색, 파라메트릭)                               │
│  STEP 9: 비트마스킹 기초                                                    │
├─────────────────────────────────────────────────────────────────────────────┤
│                          🚀 ADVANCED (고급)                                 │
├─────────────────────────────────────────────────────────────────────────────┤
│  STEP 10: 고급 STL (set, map, priority_queue, algorithm)                   │
│  STEP 11: 비선형 자료구조 직접 구현 (이진트리, 힙)                           │
│  STEP 12: 그래프 기초 (BFS, DFS)                                           │
│  STEP 13: 최단 경로 (다익스트라, 플로이드)                                   │
│  STEP 14: 동적 프로그래밍 (DP)                                              │
│  STEP 15: 비트마스킹 심화 + DP                                              │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

# 🎯 BEGINNER (입문) - 브론즈 티어

---

## STEP 0: C++ 기초 문법 ⬅️ **현재 단계**

### 학습 목표
- C++ 프로그램의 기본 구조 이해
- 표준 입출력 사용법 습득
- 기본 자료형과 변수 이해

### 핵심 개념

#### 1. 프로그램 기본 구조
```cpp
#include <iostream>  // 입출력 라이브러리
using namespace std; // std 네임스페이스 사용

int main() {
    // 코드 작성 영역
    return 0;  // 프로그램 정상 종료
}
```

#### 2. 출력 (cout)
```cpp
cout << "Hello, World!" << endl;  // 문자열 출력
cout << 123 << endl;               // 숫자 출력
cout << "숫자: " << 42 << endl;    // 혼합 출력
```

#### 3. 입력 (cin)
```cpp
int num;
cin >> num;  // 정수 입력받기

string name;
cin >> name; // 문자열 입력받기 (공백 전까지)
```

#### 4. 기본 자료형
```cpp
int a = 10;        // 정수 (-2^31 ~ 2^31-1)
long long b = 10;  // 큰 정수 (-2^63 ~ 2^63-1)
double c = 3.14;   // 실수
char d = 'A';      // 문자
string e = "Hi";   // 문자열
bool f = true;     // 참/거짓
```

### 추천 문제 (브론즈 5)

| 순서 | 문제 번호 | 문제 제목 | 태그 | 설명 |
|------|-----------|----------|------|------|
| 1 | 2557 | Hello World | 구현 | 가장 기초적인 출력 문제 |
| 2 | 1000 | A+B | 사칙연산 | 두 수 입력받아 더하기 |
| 3 | 1001 | A-B | 사칙연산 | 두 수 입력받아 빼기 |
| 4 | 10998 | A×B | 사칙연산 | 두 수 입력받아 곱하기 |
| 5 | 1008 | A/B | 사칙연산 | 두 수 입력받아 나누기 |
| 6 | 10869 | 사칙연산 | 사칙연산 | 사칙연산 종합 |
| 7 | 10926 | ??! | 문자열 | 문자열 뒤에 문자 붙이기 |
| 8 | 18108 | 1998년생인 내가 태풍이다 | 사칙연산 | 간단한 연산 |
| 9 | 10171 | 고양이 | 구현 | 여러 줄 출력 |
| 10 | 10172 | 개 | 구현 | 특수문자 출력 |

---

## STEP 1: 조건문과 반복문

### 학습 목표
- if-else, switch 조건문 마스터
- for, while 반복문 이해
- 중첩 반복문 활용

### 핵심 개념

#### 조건문
```cpp
if (조건1) {
    // 조건1이 참일 때
} else if (조건2) {
    // 조건2가 참일 때
} else {
    // 모든 조건이 거짓일 때
}
```

#### 반복문
```cpp
// for 문 - 반복 횟수가 정해진 경우
for (int i = 0; i < n; i++) {
    // 반복 내용
}

// while 문 - 조건이 참인 동안 반복
while (조건) {
    // 반복 내용
}
```

### 추천 문제 (브론즈 4~5)

| 순서 | 문제 번호 | 문제 제목 | 태그 |
|------|-----------|----------|------|
| 1 | 1330 | 두 수 비교하기 | 조건문 |
| 2 | 9498 | 시험 성적 | 조건문 |
| 3 | 2753 | 윤년 | 조건문 |
| 4 | 14681 | 사분면 고르기 | 조건문 |
| 5 | 2884 | 알람 시계 | 조건문 |
| 6 | 2739 | 구구단 | 반복문 |
| 7 | 10950 | A+B - 3 | 반복문 |
| 8 | 8393 | 합 | 반복문 |
| 9 | 2438 | 별 찍기 - 1 | 반복문 |
| 10 | 2439 | 별 찍기 - 2 | 반복문 |

---

## STEP 2: 배열과 문자열

### 학습 목표
- 1차원, 2차원 배열 활용
- 문자열 처리 기초
- 배열 순회 및 탐색

### 핵심 개념

#### 배열
```cpp
int arr[5] = {1, 2, 3, 4, 5};      // 1차원 배열
int matrix[3][3];                   // 2차원 배열

// 배열 순회
for (int i = 0; i < 5; i++) {
    cout << arr[i] << " ";
}
```

#### 문자열
```cpp
string s = "hello";
cout << s.length() << endl;  // 길이: 5
cout << s[0] << endl;        // 첫 문자: 'h'
s += " world";               // 문자열 연결
```

### 추천 문제 (브론즈 3~4)

| 순서 | 문제 번호 | 문제 제목 | 태그 |
|------|-----------|----------|------|
| 1 | 10818 | 최소, 최대 | 배열 |
| 2 | 2562 | 최댓값 | 배열 |
| 3 | 10871 | X보다 작은 수 | 배열 |
| 4 | 10807 | 개수 세기 | 배열 |
| 5 | 11720 | 숫자의 합 | 문자열 |
| 6 | 10809 | 알파벳 찾기 | 문자열 |
| 7 | 2675 | 문자열 반복 | 문자열 |
| 8 | 1152 | 단어의 개수 | 문자열 |
| 9 | 2566 | 최댓값 | 2차원 배열 |
| 10 | 2563 | 색종이 | 2차원 배열 |

---

## STEP 3: 함수와 재귀 기초

### 학습 목표
- 함수 정의와 호출
- 값 전달과 참조 전달
- 재귀 함수의 기본 원리

### 핵심 개념

#### 함수 정의
```cpp
// 반환값이 있는 함수
int add(int a, int b) {
    return a + b;
}

// 참조 전달
void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}
```

#### 재귀 함수
```cpp
// 팩토리얼 (n!)
int factorial(int n) {
    if (n <= 1) return 1;       // 기저 조건
    return n * factorial(n - 1); // 재귀 호출
}

// 피보나치
int fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}
```

### 추천 문제 (브론즈 2~3)

| 순서 | 문제 번호 | 문제 제목 | 태그 |
|------|-----------|----------|------|
| 1 | 15596 | 정수 N개의 합 | 함수 |
| 2 | 4673 | 셀프 넘버 | 함수 |
| 3 | 1065 | 한수 | 함수 |
| 4 | 10872 | 팩토리얼 | 재귀 |
| 5 | 10870 | 피보나치 수 5 | 재귀 |
| 6 | 2447 | 별 찍기 - 10 | 재귀 |
| 7 | 11729 | 하노이 탑 이동 순서 | 재귀 |
| 8 | 17478 | 재귀함수가 뭔가요? | 재귀 |

---

# 📦 INTERMEDIATE (중급) - 실버 티어

---

## STEP 4: 기초 STL

### 학습 목표
- vector 컨테이너 활용
- pair와 tuple 사용법
- 기본 algorithm 함수들

### 핵심 개념

#### vector
```cpp
#include <vector>

vector<int> v;              // 빈 벡터 생성
vector<int> v2(10, 0);      // 크기 10, 0으로 초기화

v.push_back(1);             // 끝에 추가
v.pop_back();               // 끝에서 제거
v.size();                   // 크기
v[0];                       // 인덱스 접근
v.front();                  // 첫 요소
v.back();                   // 마지막 요소
```

#### pair
```cpp
#include <utility>

pair<int, int> p = {1, 2};
p.first;   // 1
p.second;  // 2

// pair 비교: first 먼저, 같으면 second
pair<int, int> a = {1, 3};
pair<int, int> b = {1, 2};
// b < a (second 비교에서 2 < 3)
```

#### 기본 algorithm
```cpp
#include <algorithm>

vector<int> v = {3, 1, 4, 1, 5};
sort(v.begin(), v.end());            // 오름차순 정렬
sort(v.begin(), v.end(), greater<int>()); // 내림차순

int maxVal = *max_element(v.begin(), v.end());
int minVal = *min_element(v.begin(), v.end());
```

### 추천 문제 (실버 5)

| 순서 | 문제 번호 | 문제 제목 | 태그 |
|------|-----------|----------|------|
| 1 | 10828 | 스택 | STL 연습 |
| 2 | 10845 | 큐 | STL 연습 |
| 3 | 11650 | 좌표 정렬하기 | pair, sort |
| 4 | 11651 | 좌표 정렬하기 2 | pair, sort |
| 5 | 10814 | 나이순 정렬 | 안정 정렬 |

---

## STEP 5: 선형 자료구조 직접 구현 ⭐

### 학습 목표
- STL 없이 자료구조 직접 구현
- struct와 전역 배열을 활용한 구현 (대회 스타일)
- 포인터 기반 연결 리스트 이해

> 💡 **왜 struct를 사용하나요?**
> - C++에서 `struct`와 `class`의 유일한 차이는 기본 접근 제한자입니다.
> - `struct`: 기본이 `public` (멤버에 바로 접근 가능)
> - `class`: 기본이 `private` (getter/setter 필요)
> - 알고리즘 대회에서는 **간결함**이 중요하므로 `struct`를 주로 사용합니다.

### 5-1. 스택 (Stack) 직접 구현

```cpp
// 방법 1: 전역 배열 + 변수 (가장 간단, 대회에서 가장 많이 사용)
int stk[10000];
int top_idx = -1;

void push(int x) { stk[++top_idx] = x; }
int pop() { return stk[top_idx--]; }
int top() { return stk[top_idx]; }
bool empty() { return top_idx == -1; }
int size() { return top_idx + 1; }
```

```cpp
// 방법 2: struct로 캡슐화 (여러 스택이 필요할 때)
struct Stack {
    int data[10000];
    int top_idx = -1;
    
    void push(int x) { data[++top_idx] = x; }
    int pop() { return data[top_idx--]; }
    int top() { return data[top_idx]; }
    bool empty() { return top_idx == -1; }
    int size() { return top_idx + 1; }
};

// 사용법
Stack stk;
stk.push(10);
cout << stk.top();  // 10
```

### 5-2. 큐 (Queue) 직접 구현

```cpp
// 방법 1: 전역 배열 + 변수
int que[10001];
int front_idx = 0, back_idx = 0;

void push(int x) { que[back_idx++] = x; }
int pop() { return que[front_idx++]; }
int front() { return que[front_idx]; }
int back() { return que[back_idx - 1]; }
bool empty() { return front_idx == back_idx; }
int size() { return back_idx - front_idx; }
```

```cpp
// 방법 2: struct로 캡슐화
struct Queue {
    int data[10001];
    int front_idx = 0, back_idx = 0;
    
    void push(int x) { data[back_idx++] = x; }
    int pop() { return data[front_idx++]; }
    int front() { return data[front_idx]; }
    int back() { return data[back_idx - 1]; }
    bool empty() { return front_idx == back_idx; }
    int size() { return back_idx - front_idx; }
};
```

### 5-3. 덱 (Deque) 직접 구현

```cpp
// 방법 1: 전역 배열 (중간에서 시작)
int dq[20001];
int front_idx = 10000, back_idx = 10000;

void push_front(int x) { dq[--front_idx] = x; }
void push_back(int x) { dq[back_idx++] = x; }
int pop_front() { return dq[front_idx++]; }
int pop_back() { return dq[--back_idx]; }
int front() { return dq[front_idx]; }
int back() { return dq[back_idx - 1]; }
bool empty() { return front_idx == back_idx; }
int size() { return back_idx - front_idx; }
```

```cpp
// 방법 2: struct로 캡슐화
struct Deque {
    int data[20001];
    int front_idx = 10000, back_idx = 10000;
    
    void push_front(int x) { data[--front_idx] = x; }
    void push_back(int x) { data[back_idx++] = x; }
    int pop_front() { return data[front_idx++]; }
    int pop_back() { return data[--back_idx]; }
    int front() { return data[front_idx]; }
    int back() { return data[back_idx - 1]; }
    bool empty() { return front_idx == back_idx; }
    int size() { return back_idx - front_idx; }
};
```

### 5-4. 연결 리스트 (Linked List) 직접 구현

```cpp
// 노드 구조체 정의
struct Node {
    int data;
    Node* next;
};

// 전역 변수
Node* head = nullptr;
int list_size = 0;

// 맨 앞에 삽입
void push_front(int x) {
    Node* new_node = new Node();
    new_node->data = x;
    new_node->next = head;
    head = new_node;
    list_size++;
}

// 맨 앞 삭제
int pop_front() {
    if (!head) return -1;
    Node* temp = head;
    int val = temp->data;
    head = head->next;
    delete temp;
    list_size--;
    return val;
}

// 특정 위치에 삽입
void insert(int idx, int x) {
    if (idx == 0) {
        push_front(x);
        return;
    }
    Node* cur = head;
    for (int i = 0; i < idx - 1 && cur; i++) {
        cur = cur->next;
    }
    if (cur) {
        Node* new_node = new Node();
        new_node->data = x;
        new_node->next = cur->next;
        cur->next = new_node;
        list_size++;
    }
}

// 특정 위치 삭제
void erase(int idx) {
    if (idx == 0) {
        pop_front();
        return;
    }
    Node* cur = head;
    for (int i = 0; i < idx - 1 && cur; i++) {
        cur = cur->next;
    }
    if (cur && cur->next) {
        Node* temp = cur->next;
        cur->next = temp->next;
        delete temp;
        list_size--;
    }
}

int size() { return list_size; }
bool empty() { return list_size == 0; }
```

### 5-5. 이중 연결 리스트 (Doubly Linked List)

```cpp
// 노드 구조체 정의
struct DNode {
    int data;
    DNode* prev;
    DNode* next;
};

// 전역 변수
DNode* head = nullptr;
DNode* tail = nullptr;
int list_size = 0;

// 맨 앞에 삽입
void push_front(int x) {
    DNode* new_node = new DNode();
    new_node->data = x;
    new_node->prev = nullptr;
    new_node->next = head;
    
    if (head) head->prev = new_node;
    else tail = new_node;
    
    head = new_node;
    list_size++;
}

// 맨 뒤에 삽입
void push_back(int x) {
    DNode* new_node = new DNode();
    new_node->data = x;
    new_node->next = nullptr;
    new_node->prev = tail;
    
    if (tail) tail->next = new_node;
    else head = new_node;
    
    tail = new_node;
    list_size++;
}

// 맨 앞 삭제
int pop_front() {
    if (!head) return -1;
    DNode* temp = head;
    int val = temp->data;
    head = head->next;
    
    if (head) head->prev = nullptr;
    else tail = nullptr;
    
    delete temp;
    list_size--;
    return val;
}

// 맨 뒤 삭제
int pop_back() {
    if (!tail) return -1;
    DNode* temp = tail;
    int val = temp->data;
    tail = tail->prev;
    
    if (tail) tail->next = nullptr;
    else head = nullptr;
    
    delete temp;
    list_size--;
    return val;
}

int size() { return list_size; }
bool empty() { return list_size == 0; }
```

### 5-6. 배열 기반 연결 리스트 (정적 할당)

> 💡 **동적 할당 없이 연결 리스트 구현하기**
> 대회에서 `new`/`delete` 없이 배열로 연결 리스트를 구현하는 방법입니다.

```cpp
const int MAX = 100001;

struct Node {
    int data;
    int prev, next;  // 인덱스로 연결
} node[MAX];

int head = -1, tail = -1;
int node_cnt = 0;

// 새 노드 할당
int alloc(int x) {
    node[node_cnt].data = x;
    node[node_cnt].prev = -1;
    node[node_cnt].next = -1;
    return node_cnt++;
}

// 맨 뒤에 삽입
void push_back(int x) {
    int idx = alloc(x);
    if (tail == -1) {
        head = tail = idx;
    } else {
        node[tail].next = idx;
        node[idx].prev = tail;
        tail = idx;
    }
}

// 특정 노드 뒤에 삽입
void insert_after(int target, int x) {
    int idx = alloc(x);
    node[idx].prev = target;
    node[idx].next = node[target].next;
    
    if (node[target].next != -1) {
        node[node[target].next].prev = idx;
    } else {
        tail = idx;
    }
    node[target].next = idx;
}

// 특정 노드 삭제
void erase(int idx) {
    if (node[idx].prev != -1) {
        node[node[idx].prev].next = node[idx].next;
    } else {
        head = node[idx].next;
    }
    
    if (node[idx].next != -1) {
        node[node[idx].next].prev = node[idx].prev;
    } else {
        tail = node[idx].prev;
    }
}
```

### 추천 문제 (자료구조 직접 구현)

| 순서 | 문제 번호 | 문제 제목 | 구현 대상 |
|------|-----------|----------|----------|
| 1 | 10828 | 스택 | 스택 구현 |
| 2 | 10845 | 큐 | 큐 구현 |
| 3 | 10866 | 덱 | 덱 구현 |
| 4 | 1158 | 요세푸스 문제 | 연결리스트/큐 |
| 5 | 5397 | 키로거 | 연결리스트/스택 |
| 6 | 1406 | 에디터 | 연결리스트/스택 |

---

## STEP 6: STL 컨테이너 심화

### 학습 목표
- stack, queue, deque STL 활용
- list 컨테이너 이해
- 반복자(iterator) 활용

### 핵심 개념

```cpp
#include <stack>
#include <queue>
#include <deque>
#include <list>

// 스택
stack<int> st;
st.push(1); st.pop(); st.top(); st.empty(); st.size();

// 큐
queue<int> q;
q.push(1); q.pop(); q.front(); q.back(); q.empty();

// 덱
deque<int> dq;
dq.push_front(1); dq.push_back(2);
dq.pop_front(); dq.pop_back();
dq.front(); dq.back();

// 리스트 (이중 연결 리스트)
list<int> lst;
lst.push_front(1); lst.push_back(2);
auto it = lst.begin();
lst.insert(it, 3);  // 반복자 위치에 삽입
lst.erase(it);      // 반복자 위치 삭제
```

### 추천 문제 (실버 4~5)

| 순서 | 문제 번호 | 문제 제목 | 태그 |
|------|-----------|----------|------|
| 1 | 9012 | 괄호 | 스택 |
| 2 | 4949 | 균형잡힌 세상 | 스택 |
| 3 | 1874 | 스택 수열 | 스택 |
| 4 | 2164 | 카드2 | 큐 |
| 5 | 11866 | 요세푸스 문제 0 | 큐 |
| 6 | 5430 | AC | 덱 |

---

## STEP 7: 정렬 알고리즘

### 학습 목표
- O(n²) 정렬: 버블, 선택, 삽입
- O(n log n) 정렬: 퀵, 머지
- 정렬의 안정성 이해

### 핵심 개념

#### 버블 정렬 O(n²)
```cpp
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}
```

#### 퀵 정렬 O(n log n)
```cpp
void quickSort(int arr[], int left, int right) {
    if (left >= right) return;
    
    int pivot = arr[(left + right) / 2];
    int i = left, j = right;
    
    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            swap(arr[i], arr[j]);
            i++; j--;
        }
    }
    
    quickSort(arr, left, j);
    quickSort(arr, i, right);
}
```

#### 병합 정렬 O(n log n)
```cpp
void merge(int arr[], int left, int mid, int right) {
    vector<int> temp;
    int i = left, j = mid + 1;
    
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) temp.push_back(arr[i++]);
        else temp.push_back(arr[j++]);
    }
    while (i <= mid) temp.push_back(arr[i++]);
    while (j <= right) temp.push_back(arr[j++]);
    
    for (int k = left; k <= right; k++) {
        arr[k] = temp[k - left];
    }
}

void mergeSort(int arr[], int left, int right) {
    if (left >= right) return;
    
    int mid = (left + right) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}
```

### 추천 문제 (실버 3~5)

| 순서 | 문제 번호 | 문제 제목 | 태그 |
|------|-----------|----------|------|
| 1 | 2750 | 수 정렬하기 | 정렬 기초 |
| 2 | 2751 | 수 정렬하기 2 | O(n log n) 정렬 |
| 3 | 10989 | 수 정렬하기 3 | 카운팅 정렬 |
| 4 | 1427 | 소트인사이드 | 정렬 |
| 5 | 11650 | 좌표 정렬하기 | 다중 조건 정렬 |
| 6 | 10814 | 나이순 정렬 | 안정 정렬 |

---

## STEP 8: 탐색 알고리즘

### 학습 목표
- 이분 탐색 원리와 구현
- lower_bound, upper_bound
- 파라메트릭 서치

### 핵심 개념

#### 이분 탐색
```cpp
// 정렬된 배열에서 target 찾기
int binarySearch(int arr[], int n, int target) {
    int left = 0, right = n - 1;
    
    while (left <= right) {
        int mid = (left + right) / 2;
        
        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    
    return -1;  // 찾지 못함
}
```

#### STL 이분 탐색
```cpp
#include <algorithm>

vector<int> v = {1, 2, 3, 4, 5};
binary_search(v.begin(), v.end(), 3);  // true/false

auto lb = lower_bound(v.begin(), v.end(), 3);  // 3 이상 첫 위치
auto ub = upper_bound(v.begin(), v.end(), 3);  // 3 초과 첫 위치
```

### 추천 문제 (실버 2~4)

| 순서 | 문제 번호 | 문제 제목 | 태그 |
|------|-----------|----------|------|
| 1 | 1920 | 수 찾기 | 이분 탐색 기초 |
| 2 | 10816 | 숫자 카드 2 | lower/upper_bound |
| 3 | 1654 | 랜선 자르기 | 파라메트릭 |
| 4 | 2805 | 나무 자르기 | 파라메트릭 |
| 5 | 2110 | 공유기 설치 | 파라메트릭 |

---

## STEP 9: 비트마스킹 기초 ⭐

### 학습 목표
- 비트 연산자 이해
- 집합을 비트로 표현
- 비트마스크로 상태 관리

### 핵심 개념

#### 비트 연산자
```cpp
// 비트 연산자 종류
a & b   // AND: 둘 다 1이면 1
a | b   // OR: 하나라도 1이면 1
a ^ b   // XOR: 서로 다르면 1
~a      // NOT: 비트 반전
a << n  // 왼쪽 시프트: 2^n 곱하기
a >> n  // 오른쪽 시프트: 2^n 나누기
```

#### 집합 표현 (0~N-1 원소)
```cpp
int S = 0;  // 공집합

// i번째 원소 추가
S |= (1 << i);

// i번째 원소 제거
S &= ~(1 << i);

// i번째 원소 포함 여부
bool has = (S >> i) & 1;
// 또는
bool has = S & (1 << i);

// i번째 원소 토글 (있으면 제거, 없으면 추가)
S ^= (1 << i);

// 집합 크기 (원소 개수)
int count = __builtin_popcount(S);
```

#### 모든 부분집합 순회
```cpp
int n = 4;  // 원소 개수
int full = (1 << n) - 1;  // 전체 집합 {0,1,2,3}

// 모든 부분집합 순회
for (int S = 0; S <= full; S++) {
    // S는 각 부분집합을 비트로 표현
    for (int i = 0; i < n; i++) {
        if (S & (1 << i)) {
            cout << i << " ";  // S에 포함된 원소 출력
        }
    }
    cout << endl;
}

// 특정 집합 S의 모든 부분집합 순회
for (int subset = S; subset > 0; subset = (subset - 1) & S) {
    // subset 처리
}
```

### 추천 문제 (실버 3~5)

| 순서 | 문제 번호 | 문제 제목 | 태그 |
|------|-----------|----------|------|
| 1 | 11723 | 집합 | 비트마스크 기초 |
| 2 | 1182 | 부분수열의 합 | 비트마스크 완전탐색 |
| 3 | 14889 | 스타트와 링크 | 비트마스크 |
| 4 | 14391 | 종이 조각 | 비트마스크 |

---

# 🚀 ADVANCED (고급) - 골드 티어

---

## STEP 10: 고급 STL ⭐

### 학습 목표
- set, map 컨테이너 활용
- priority_queue (힙)
- algorithm 라이브러리 심화

### 핵심 개념

#### set (정렬된 집합, 중복 불가)
```cpp
#include <set>

set<int> s;
s.insert(3);          // 삽입 O(log n)
s.erase(3);           // 삭제 O(log n)
s.count(3);           // 포함 여부 (0 or 1)
s.find(3) != s.end(); // 찾기

// 정렬된 순서로 순회
for (int x : s) cout << x << " ";

// multiset - 중복 허용
multiset<int> ms;
```

#### map (키-값 쌍, 정렬됨)
```cpp
#include <map>

map<string, int> m;
m["apple"] = 5;       // 삽입/수정 O(log n)
m["banana"] = 3;
m.count("apple");     // 키 존재 여부
m.erase("apple");     // 삭제

for (auto& [key, val] : m) {
    cout << key << ": " << val << endl;
}

// unordered_map - 해시 기반 O(1)
#include <unordered_map>
unordered_map<string, int> um;
```

#### priority_queue (최대 힙)
```cpp
#include <queue>

priority_queue<int> pq;  // 최대 힙
pq.push(3);
pq.push(1);
pq.push(4);
pq.top();   // 4 (최댓값)
pq.pop();   // 4 제거

// 최소 힙
priority_queue<int, vector<int>, greater<int>> minPq;

// 커스텀 비교
struct Compare {
    bool operator()(int a, int b) {
        return a > b;  // 최소 힙
    }
};
priority_queue<int, vector<int>, Compare> customPq;
```

#### algorithm 심화
```cpp
#include <algorithm>

// next_permutation - 다음 순열
vector<int> v = {1, 2, 3};
do {
    // 현재 순열 처리
} while (next_permutation(v.begin(), v.end()));

// unique - 연속 중복 제거 (정렬 필요)
sort(v.begin(), v.end());
v.erase(unique(v.begin(), v.end()), v.end());

// nth_element - k번째 원소
nth_element(v.begin(), v.begin() + k, v.end());
// v[k]가 k번째로 작은 원소

// partial_sort - 상위 k개만 정렬
partial_sort(v.begin(), v.begin() + k, v.end());
```

### 추천 문제 (골드 5~실버 1)

| 순서 | 문제 번호 | 문제 제목 | 태그 |
|------|-----------|----------|------|
| 1 | 7785 | 회사에 있는 사람 | set/map |
| 2 | 1764 | 듣보잡 | set |
| 3 | 1927 | 최소 힙 | priority_queue |
| 4 | 11279 | 최대 힙 | priority_queue |
| 5 | 11286 | 절댓값 힙 | priority_queue |
| 6 | 10972 | 다음 순열 | permutation |
| 7 | 9375 | 패션왕 신해빈 | map |

---

## STEP 11: 비선형 자료구조 직접 구현 ⭐

### 학습 목표
- 힙 직접 구현 (배열 기반)
- 이진 트리 구현
- 트리 순회 (전위, 중위, 후위)

### 11-1. 힙 (Heap) 직접 구현

> 💡 **힙의 특성**
> - 완전 이진 트리 형태
> - 최소 힙: 부모 ≤ 자식
> - 최대 힙: 부모 ≥ 자식
> - 배열로 구현 가능 (인덱스 관계 활용)

```cpp
// 배열 기반 최소 힙
const int MAX = 100001;
int heap[MAX];
int heap_size = 0;

// 부모/자식 인덱스 계산 (1-indexed)
int parent(int i) { return i / 2; }
int left_child(int i) { return i * 2; }
int right_child(int i) { return i * 2 + 1; }

// 위로 올리기 (삽입 시)
void heapify_up(int i) {
    while (i > 1 && heap[parent(i)] > heap[i]) {
        swap(heap[parent(i)], heap[i]);
        i = parent(i);
    }
}

// 아래로 내리기 (삭제 시)
void heapify_down(int i) {
    int smallest = i;
    int left = left_child(i);
    int right = right_child(i);
    
    if (left <= heap_size && heap[left] < heap[smallest])
        smallest = left;
    if (right <= heap_size && heap[right] < heap[smallest])
        smallest = right;
    
    if (smallest != i) {
        swap(heap[i], heap[smallest]);
        heapify_down(smallest);
    }
}

// 삽입
void push(int x) {
    heap[++heap_size] = x;
    heapify_up(heap_size);
}

// 최솟값 삭제 및 반환
int pop() {
    if (heap_size == 0) return -1;
    int top = heap[1];
    heap[1] = heap[heap_size--];
    heapify_down(1);
    return top;
}

// 최솟값 확인
int top() {
    return heap_size > 0 ? heap[1] : -1;
}

bool empty() { return heap_size == 0; }
int size() { return heap_size; }
```

```cpp
// struct로 캡슐화 (여러 힙이 필요할 때)
struct MinHeap {
    int data[MAX];
    int sz = 0;
    
    int parent(int i) { return i / 2; }
    int left(int i) { return i * 2; }
    int right(int i) { return i * 2 + 1; }
    
    void up(int i) {
        while (i > 1 && data[parent(i)] > data[i]) {
            swap(data[parent(i)], data[i]);
            i = parent(i);
        }
    }
    
    void down(int i) {
        int small = i;
        if (left(i) <= sz && data[left(i)] < data[small]) small = left(i);
        if (right(i) <= sz && data[right(i)] < data[small]) small = right(i);
        if (small != i) {
            swap(data[i], data[small]);
            down(small);
        }
    }
    
    void push(int x) { data[++sz] = x; up(sz); }
    int pop() { int t = data[1]; data[1] = data[sz--]; down(1); return t; }
    int top() { return data[1]; }
    bool empty() { return sz == 0; }
};
```

### 11-2. 이진 트리 (Binary Tree) 직접 구현

```cpp
// 포인터 기반 이진 트리
struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;
};

// 노드 생성
TreeNode* create_node(int x) {
    TreeNode* node = new TreeNode();
    node->data = x;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

// 전위 순회 (Pre-order): 루트 → 왼쪽 → 오른쪽
void preorder(TreeNode* node) {
    if (!node) return;
    cout << node->data << " ";  // 루트 방문
    preorder(node->left);
    preorder(node->right);
}

// 중위 순회 (In-order): 왼쪽 → 루트 → 오른쪽
void inorder(TreeNode* node) {
    if (!node) return;
    inorder(node->left);
    cout << node->data << " ";  // 루트 방문
    inorder(node->right);
}

// 후위 순회 (Post-order): 왼쪽 → 오른쪽 → 루트
void postorder(TreeNode* node) {
    if (!node) return;
    postorder(node->left);
    postorder(node->right);
    cout << node->data << " ";  // 루트 방문
}

// 사용 예시
int main() {
    TreeNode* root = create_node(1);
    root->left = create_node(2);
    root->right = create_node(3);
    root->left->left = create_node(4);
    root->left->right = create_node(5);
    
    //       1
    //      / \
    //     2   3
    //    / \
    //   4   5
    
    preorder(root);   // 1 2 4 5 3
    inorder(root);    // 4 2 5 1 3
    postorder(root);  // 4 5 2 3 1
}
```

### 11-3. 배열 기반 이진 트리 (정적 할당)

```cpp
// 배열로 이진 트리 구현 (완전 이진 트리에 최적)
// 인덱스 관계: 부모 i → 왼쪽 자식 2i, 오른쪽 자식 2i+1

const int MAX = 100001;

struct TreeNode {
    int data;
    int left, right;  // 자식 인덱스 (-1이면 없음)
} tree[MAX];

int node_cnt = 0;

// 노드 할당
int alloc(int x) {
    tree[node_cnt].data = x;
    tree[node_cnt].left = -1;
    tree[node_cnt].right = -1;
    return node_cnt++;
}

// 전위 순회
void preorder(int idx) {
    if (idx == -1) return;
    cout << tree[idx].data << " ";
    preorder(tree[idx].left);
    preorder(tree[idx].right);
}

// 중위 순회
void inorder(int idx) {
    if (idx == -1) return;
    inorder(tree[idx].left);
    cout << tree[idx].data << " ";
    inorder(tree[idx].right);
}

// 후위 순회
void postorder(int idx) {
    if (idx == -1) return;
    postorder(tree[idx].left);
    postorder(tree[idx].right);
    cout << tree[idx].data << " ";
}
```

### 11-4. 이진 탐색 트리 (BST) 기본

```cpp
// 이진 탐색 트리: 왼쪽 < 루트 < 오른쪽
struct BSTNode {
    int data;
    BSTNode* left;
    BSTNode* right;
};

BSTNode* root = nullptr;

// 삽입
BSTNode* insert(BSTNode* node, int x) {
    if (!node) {
        BSTNode* new_node = new BSTNode();
        new_node->data = x;
        new_node->left = new_node->right = nullptr;
        return new_node;
    }
    
    if (x < node->data)
        node->left = insert(node->left, x);
    else if (x > node->data)
        node->right = insert(node->right, x);
    
    return node;
}

// 검색
bool search(BSTNode* node, int x) {
    if (!node) return false;
    if (node->data == x) return true;
    if (x < node->data) return search(node->left, x);
    return search(node->right, x);
}

// 중위 순회로 정렬된 출력
void inorder(BSTNode* node) {
    if (!node) return;
    inorder(node->left);
    cout << node->data << " ";
    inorder(node->right);
}

// 사용 예시
int main() {
    root = insert(root, 5);
    insert(root, 3);
    insert(root, 7);
    insert(root, 1);
    insert(root, 4);
    
    //       5
    //      / \
    //     3   7
    //    / \
    //   1   4
    
    inorder(root);  // 1 3 4 5 7 (정렬됨!)
}
```

### 추천 문제 (비선형 자료구조)

| 순서 | 문제 번호 | 문제 제목 | 구현 대상 |
|------|-----------|----------|----------|
| 1 | 1927 | 최소 힙 | 힙 구현 |
| 2 | 11279 | 최대 힙 | 힙 구현 |
| 3 | 11286 | 절댓값 힙 | 커스텀 힙 |
| 4 | 1991 | 트리 순회 | 이진 트리 |
| 5 | 5639 | 이진 검색 트리 | BST |

---

## STEP 12: 그래프 기초

### 학습 목표
- 그래프 표현 (인접 리스트, 인접 행렬)
- BFS (너비 우선 탐색)
- DFS (깊이 우선 탐색)

### 핵심 개념

#### 그래프 표현
```cpp
int n, m;  // 정점 수, 간선 수

// 인접 리스트 (추천)
vector<int> adj[100001];
for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);  // 무방향
}

// 인접 행렬
int graph[1001][1001];
for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    graph[u][v] = graph[v][u] = 1;
}
```

#### BFS
```cpp
void bfs(int start) {
    queue<int> q;
    bool visited[100001] = {false};
    
    q.push(start);
    visited[start] = true;
    
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        
        cout << cur << " ";
        
        for (int next : adj[cur]) {
            if (!visited[next]) {
                visited[next] = true;
                q.push(next);
            }
        }
    }
}
```

#### DFS
```cpp
bool visited[100001];

void dfs(int cur) {
    visited[cur] = true;
    cout << cur << " ";
    
    for (int next : adj[cur]) {
        if (!visited[next]) {
            dfs(next);
        }
    }
}
```

### 추천 문제 (골드 5~실버 1)

| 순서 | 문제 번호 | 문제 제목 | 태그 |
|------|-----------|----------|------|
| 1 | 1260 | DFS와 BFS | 기초 |
| 2 | 2606 | 바이러스 | DFS/BFS |
| 3 | 1012 | 유기농 배추 | 연결 요소 |
| 4 | 2178 | 미로 탐색 | BFS 최단거리 |
| 5 | 7576 | 토마토 | BFS |
| 6 | 2667 | 단지번호붙이기 | DFS/BFS |

---

## STEP 13: 최단 경로

### 학습 목표
- 다익스트라 알고리즘
- 플로이드-워셜 알고리즘
- 벨만-포드 알고리즘

### 추천 문제 (골드 4~5)

| 순서 | 문제 번호 | 문제 제목 | 태그 |
|------|-----------|----------|------|
| 1 | 1753 | 최단경로 | 다익스트라 |
| 2 | 1916 | 최소비용 구하기 | 다익스트라 |
| 3 | 11404 | 플로이드 | 플로이드 |
| 4 | 11403 | 경로 찾기 | 플로이드 |

---

## STEP 14: 동적 프로그래밍 (DP)

### 학습 목표
- DP의 기본 원리 (메모이제이션)
- 점화식 세우기
- Top-down vs Bottom-up

### 추천 문제 (실버 3~골드 5)

| 순서 | 문제 번호 | 문제 제목 | 태그 |
|------|-----------|----------|------|
| 1 | 1003 | 피보나치 함수 | DP 기초 |
| 2 | 9184 | 신나는 함수 실행 | 메모이제이션 |
| 3 | 1463 | 1로 만들기 | DP |
| 4 | 11726 | 2×n 타일링 | DP |
| 5 | 11727 | 2×n 타일링 2 | DP |
| 6 | 9095 | 1, 2, 3 더하기 | DP |
| 7 | 12865 | 평범한 배낭 | 배낭 DP |

---

## STEP 15: 비트마스킹 심화 + DP ⭐

### 학습 목표
- 비트마스크 DP
- TSP 문제 이해
- 상태 압축 DP

### 핵심 개념

#### 비트마스크 DP
```cpp
// TSP (외판원 순회) - O(n² × 2^n)
int n;
int dist[20][20];
int dp[20][1 << 20];  // dp[현재위치][방문상태]

int tsp(int cur, int visited) {
    // 모든 도시 방문 완료
    if (visited == (1 << n) - 1) {
        return dist[cur][0];  // 시작점으로 복귀
    }
    
    // 이미 계산됨
    if (dp[cur][visited] != -1) {
        return dp[cur][visited];
    }
    
    int result = INF;
    for (int next = 0; next < n; next++) {
        // 아직 방문하지 않은 도시
        if (!(visited & (1 << next))) {
            result = min(result, 
                dist[cur][next] + tsp(next, visited | (1 << next)));
        }
    }
    
    return dp[cur][visited] = result;
}
```

### 추천 문제 (골드 1~3)

| 순서 | 문제 번호 | 문제 제목 | 태그 |
|------|-----------|----------|------|
| 1 | 11723 | 집합 | 비트마스크 기초 |
| 2 | 2098 | 외판원 순회 | 비트마스크 DP |
| 3 | 1562 | 계단 수 | 비트마스크 DP |
| 4 | 2533 | 사회망 서비스(SNS) | 트리 DP |

---

## 📌 학습 팁

1. **문제를 풀기 전에 개념을 먼저 이해하세요**
2. **직접 코드를 작성하세요** - 복사/붙여넣기는 학습 효과가 낮습니다
3. **에러 메시지를 읽는 습관을 기르세요**
4. **한 문제에 30분 이상 막히면 힌트를 요청하세요**
5. **풀이 후 다른 사람의 코드도 참고하세요**
6. **STL 사용 전에 직접 구현해보면 이해도가 높아집니다**
7. **시간 복잡도를 항상 먼저 계산하세요**

---

## 📊 난이도 가이드

| 티어 | 예상 학습 기간 | 해당 STEP |
|------|---------------|----------|
| 브론즈 | 1~2주 | STEP 0~3 |
| 실버 | 3~6주 | STEP 4~9 |
| 골드 | 2~4개월 | STEP 10~15 |
