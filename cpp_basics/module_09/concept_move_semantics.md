# [Module 09] 이동 시맨틱 (Move Semantics): 자원을 '복사'하지 말고 '강탈'하라

## 1. 왜 이동(Move)이 필요한가?

지금까지 우리는 데이터를 넘길 때 **복사(Copy)**를 주로 썼습니다. 하지만 엄청나게 큰 데이터를 가진 객체라면 어떨까요?

```cpp
std::vector<int> bigData(100'000'000, 7); // 400MB 정도의 데이터
std::vector<int> otherData = bigData;     // 400MB를 새로 할당하고 복사! CPU/메모리 비명!
```

만약 `bigData`를 더 이상 쓰지 않을 예정이라면, 굳이 새로 복사할 필요가 있을까요? 그냥 `bigData`가 가진 메모리 주소(포인터)만 쏙 빼앗아 오면 되지 않을까요?

이것이 바로 **이동 시맨틱(Move Semantics)**의 핵심입니다.

---

## 2. L-value vs R-value

이동을 이해하려면 먼저 두 가지 타입을 구분해야 합니다.

| 타입 | 의미 | 특징 |
|------|------|------|
| **L-value** | 이름을 가진 실체 (위치 있음) | 주소(`&`)를 취할 수 있음. 대입 연산자의 왼쪽 가능. |
| **R-value** | 이름 없는 임시 값 (우측 값) | 사용 후 즉시 소멸됨. 주소를 취할 수 없음. |

```cpp
int a = 10;      // a는 L-value, 10은 R-value
int b = a + 5;   // b는 L-value, (a+5)는 R-value (임시 계산 결과)
```

C++11부터는 이 **R-value**를 가리키는 특별한 참조자인 **R-value 참조(`&&`)**가 등장했습니다.

---

## 3. `std::move` — "나 이제 이거 안 쓸 거야, 가져가!"

`std::move`는 이름은 거창하지만, 실제로는 아주 단순한 일을 합니다.
> "L-value를 **R-value로 캐스팅**해준다."

즉, 컴파일러에게 "이 변수는 이제 곧 죽을 녀석(임시 객체)처럼 취급해도 돼!"라고 알려주는 신호입니다.

```cpp
std::string str1 = "Large String Data...";
std::string str2 = std::move(str1); // str1을 임시 객체처럼 취급해서 str2로 '이동'

// 결과:
// str2는 데이터를 안전하게 가로챔.
// str1은 '빈 상태' (유효하지만 정의되지 않은 상태)가 됨.
```

---

## 4. 이동 생성자 (Move Constructor)

클래스에서 이동 시맨틱을 지원하려면 **이동 생성자**가 필요합니다.

```cpp
class Buffer {
    int* data;
    size_t size;

public:
    // 1. 복사 생성자 (깊은 복사 - 느림)
    Buffer(const Buffer& other) : size(other.size) {
        data = new int[size];
        std::copy(other.data, other.data + size, data);
    }

    // 2. 이동 생성자 (자원 가로채기 - 매우 빠름!)
    Buffer(Buffer&& other) noexcept 
        : data(other.data), size(other.size) { // 주소만 쏙 빼옴
        other.data = nullptr; // 원본의 연결고리를 끊음 (중요! 안 하면 Double Free)
        other.size = 0;
    }

    ~Buffer() { delete[] data; }
};
```

### 💡 이동의 3단계 공식
1.  상대방(`other`)의 자원을 내 것으로 **가져온다** (포인터 복사).
2.  상대방의 자원을 **null** 등으로 초기화한다 (파괴자 방지).
3.  이 작업은 예외가 없음을 보장한다 (`noexcept`).

---

## 5. 이동 대입 연산자 (Move Assignment Operator)

이미 존재하는 객체에 다른 객체의 자원을 뺏어다 넣을 때 필요한 연산자입니다.

```cpp
Buffer& operator=(Buffer&& other) noexcept {
    // 1. 자가 대입 방지 (Self-assignment check)
    // - 나 자신을 나에게 이동시키는 뻘짓(?)을 막습니다. 
    if (this != &other) {
        // 2. 기존 자원 해제 (Cleanup)
        // - 대입은 '기존 것을 버리고 새 것을 받는' 과정입니다. 
        delete[] data; 

        // 3. 자원 가로채기 (Steal)
        data = other.data;
        size = other.size;

        // 4. 상대방 무력화 (Disarm)
        other.data = nullptr;
        other.size = 0;
    }
    return *this;
}
```

### ❓ 이동 생성자와의 결정적인 차이점
*   **이동 생성자**: 빈 땅에 집을 짓는 것. (기존 것이 없으므로 해제가 필요 없음)
*   **이동 대입 연산자**: 헌 집을 부수고 새 집을 짓는 것. (**`delete`** 과정이 반드시 필요!)

---

## 6. 요약 및 주의사항

1.  **속도 향상**: 복사(Heap 할당 + 반복문 복사)보다 이동(포인터 대입)이 압도적으로 빠릅니다.
2.  **unique_ptr**: 복사가 불가능한 `unique_ptr`은 `std::move`를 통해서만 소유권을 바꿀 수 있습니다.
3.  **이동 후 상태**: `std::move` 당한 변수는 이제 "껍데기"만 남은 상태이므로 다시 사용하지 않는 것이 원칙입니다.

---

---

## 6. Q&A: 심화 질문과 답변

### Q1. "결국 주소 뺏고 nullptr 넣는 게 다인데, 왜 굳이 &&를 쓰나요?"
매우 날카로운 질문입니다! 실제로 이동의 **내부 동작(Mechanism)**은 주소 복사와 원본 초기화가 전부입니다. 하지만 **&&**는 이를 실현하기 위한 **표준화된 통로** 역할을 합니다.

1.  **바인딩 규칙 (Binding Rule)**: 일반 참조(`&`)는 임시 객체(R-value)를 아예 받을 수 없도록 문법적으로 막혀 있습니다.
2.  **수정 권한 (Modifiability)**: 상수 참조(`const &`)는 임시 객체를 받을 수는 있지만, 그 안의 데이터를 `nullptr`로 수정(강탈)할 수 없습니다.
3.  **안전한 강탈**: 결국 `&&`는 **"임시 객체를 받을 수 있으면서, 동시에 내용을 수정할 수 있는 유일한 대안"**입니다.

### Q2. "이동하고 나면 원본(other)은 즉시 소멸되나요?"
아닙니다! 원본 객체는 자기가 속한 스코프(중괄호)가 끝날 때까지 스택에 버팁니다. 다만 그 안의 **'알맹이(자원)'**만 쏙 빠져나간 **'빈 껍데기'** 상태가 될 뿐입니다. 
우리가 `nullptr`을 넣어주는 이유는, 나중에 이 껍데기가 죽을 때(소멸자 호출 시) 우리가 이미 뺏어온 자원을 함께 파괴하지 못하도록 **무기를 해제(Disarm)**하는 작업입니다.

### Q3. "이동 생성자와 이동 대입 연산자는 언제 각각 호출되나요?"
*   **초기화 (Initialization)**: 새로운 변수가 탄생할 때! (`=`) 기호를 쓰더라도 실체는 **이동 생성자**입니다.
    `HugeBuffer b2 = std::move(b1);` ➞ `HugeBuffer b2(std::move(b1));`와 동일
*   **대입 (Assignment)**: 이미 존재하던 객체에 값을 다시 넣을 때 호출됩니다.
    `b2 = std::move(b3);` ➞ `b2.operator=(std::move(b3));` 호출

### Q4. "noexcept 키워드는 왜 그렇게 중요한가요?"
`std::vector` 같은 STL 컨테이너들은 **'강한 예외 보증(Strong Exception Guarantee)'**을 철저히 지킵니다.

1.  벡터의 크기가 늘어나서 자리를 옮길 때, 만약 **이동(Move)** 도중 예외가 터지면 원본 데이터가 유실될 위험이 있습니다.
2.  이런 위험을 방지하기 위해 벡터는 이동 생성자에 **`noexcept`** 가 붙어있는지 꼭 확인합니다.
3.  만약 `noexcept`가 없다면? 벡터는 성능을 포기하더라도 안전을 위해 **'느린 복사(Copy)'**를 선택해 버립니다.
4.  결론적으로, 이동 시맨틱을 제대로 활용하려면 **`noexcept`** 는 선택이 아닌 필수입니다.

---

## 7. "떼어내는(Severing)" 철학

이동 시맨틱은 자원을 복제(Clone)하는 것이 아니라, 원본으로부터 자원을 **툭 떼어내서(Tearing away)** 나에게 옮겨 붙이는 과정입니다. 
가장 빠르고 효율적인 이 "자원 강탈"의 논리를 직접 실험 코드로 확인해 보세요.

👉 **[실험 파일]** `cpp_basics/module_09/experiment_move_logic.cpp`

---

## 다음 단계

개론과 실험을 통해 논리를 이해했다면, 이제 직접 손으로 구현해 볼 차례입니다!  
👉 **[실습]** `practice_move_semantics.cpp`

---

## 진행 상황
- [x] `concept_move_semantics.md` 작성 (이론 학습용)
- [x] `&&` (R-value 참조)의 필요성 논리적 설명 및 문서화 완료
- [x] 이동 생성자 vs 대입 연산자 차이점 정리 완료
- [ ] `practice_move_semantics.cpp` 작성 (실습 문제 출제)
- [ ] `session_checkpoint.md` 최종 업데이트 및 마무리
