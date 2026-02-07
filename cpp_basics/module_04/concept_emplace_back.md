# std::vector::emplace_back 완벽 분석

## 1. ❓ 무엇이 다른가요? (The "Bag" Analogy)

질문하신 **"가방(Vector) 안에서 어떻게 가능한가?"**에 대한 답은 C++의 **Placement New**라는 기술 덕분입니다.

### 📦 push_back: "포장해서 넣기"
1. **외부 생성**: 데이터를 먼저 임시 공간에 만듭니다. (생성자 호출)
2. **복사/이동**: 만든 데이터를 가방(Vector) 안으로 옮김니다. (복사/이동 생성자 호출)
3. **제거**: 임시 공간에 있던 데이터를 부숩니다. (소멸자 호출)

### ✨ emplace_back: "재료만 던져주기"
1. **전달**: 데이터 자체가 아니라, **생성자에 필요한 재료(인자)**만 가방에게 던져줍니다.
2. **직접 생성**: 가방(Vector)이 미리 확보해둔 메모리 공간(**Reserved Memory**)에서, 받은 재료로 **즉석에서 조립**합니다.
   - 중간 복사나 이동 과정이 아예 사라집니다!

---

## 2. ⚡ 기술적 원리 (Under the Hood)

어떻게 **"이미 할당된 메모리"**에 객체를 생성할까요?

### Placement New (`new (ptr) Type`)
보통 `new`는 메모리를 할당(`malloc`)하고 생성자를 호출하지만,
**Placement New**는 "이미 있는 메모리 주소(`ptr`)" 위에 생성자만 호출하는 문법입니다.

```cpp
// Vector 내부 동작 수도코드 (Simplified)
template<typename... Args>
void emplace_back(Args&&... args) {
    if (size == capacity) expand();
    
    // 1. 들어갈 주소 계산
    Item* address = &data[size]; 
    
    // 2. [핵심!] 그 주소에 바로 생성 (Placement New)
    // args를 그대로 전달(Perfect Forwarding)하여 생성자 호출
    new (address) Item(std::forward<Args>(args)...); 
    
    size++;
}
```

이 덕분에 외부에서 객체를 만들어서 가져오는 과정이 생략됩니다.

---

## 3. 🧪 성능 검증 (직접 눈으로 확인하기)

`Monster` 클래스를 만들어 생성자가 몇 번 호출되는지 세어봅시다.

```cpp
#include <iostream>
#include <vector>
using namespace std;

struct Monster {
    int hp;
    Monster(int h) : hp(h) { 
        cout << "🔨 생성 (Construction)" << endl; 
    }
    Monster(const Monster&) { 
        cout << "📦 복사 (Copy)" << endl; 
    }
    Monster(Monster&&) { 
        cout << "🚚 이동 (Move)" << endl; 
    }
    ~Monster() { 
        cout << "💀 소멸 (Destruction)" << endl; 
    }
};

int main() {
    vector<Monster> v;
    v.reserve(10); // 메모리 재할당 노이즈 제거

    cout << "=== [1] push_back ===" << endl;
    v.push_back(Monster(100)); 
    // 생성(임시) -> 이동(벡터로) -> 소멸(임시)

    cout << "\n=== [2] emplace_back ===" << endl;
    v.emplace_back(200); 
    // 생성(벡터 내부에서 한 번만!)

    return 0;
}
```

### 실행 결과 예측
```text
=== [1] push_back ===
🔨 생성 (Construction)  <-- 임시 객체
🚚 이동 (Move)          <-- 벡터로 이사
💀 소멸 (Destruction)   <-- 임시 객체 삭제

=== [2] emplace_back ===
🔨 생성 (Construction)  <-- 벡터 안에서 깔끔하게 한 번!
```

---

## 4. 🎯 언제 써야 하나요?

| 상황 | 추천 | 이유 |
| :--- | :---: | :--- |
| **이미 객체가 있을 때** | `push_back` | 이미 만들어진 건 다시 만들 수 없으므로 `emplace`의 이점이 적음 |
| **새로 만들어서 넣을 때** | `emplace_back` | **무조건 유리함.** 복사/이동 비용 0. |
| **int, char 등 기본 타입** | 상관 없음 | 복사 비용이 워낙 작아서 차이 없음 |

> **꿀팁**: `vector<vector<int>>` 같은 중첩 벡터를 다룰 때 `emplace_back` 효과가 엄청납니다!
