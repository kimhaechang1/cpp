# 복사 생성자 (Copy Constructor)

## 1. 🔍 정의 (Definition)
객체를 생성할 때, **다른 객체(원본)를 인자로 받아** 그 내용을 복사하여 생성하는 생성자입니다.
```cpp
ClassName(const ClassName& other);
```

## 2. 🚨 문제 상황: 포인터와 얕은 복사 (Shallow Copy)
C++ 컴파일러가 기본으로 제공하는 복사 생성자는 **멤버 변수의 값을 그대로 복사**만 합니다.
이때 `Item*`, `char*` 같은 포인터 멤버가 있다면, **주소값만 복사**되어 두 객체가 **같은 메모리**를 가리키게 됩니다.

### 예시: Inventory의 비극 (The Tragedy of Inventory)
```cpp
{
    Inventory A(100);
    A.BuyItem("Sword", 50); // A가 Sword(Item 객체)를 가리킴 (주소: 0x100)
    
    Inventory B = A; // 복사! B도 Sword(Item 객체)를 가리킴 (주소: 0x100)
    
} // 블록이 끝남 -> 둘 다 죽음!
```
1. **B 소멸**: `0x100` 주소의 `Item`을 `delete`함. (성공)
2. **A 소멸**: `0x100` 주소의 `Item`을 또 `delete`하려고 함. (이미 해제된 메모리!)
3. **Double Free Error**: 프로그램 강제 종료 (Crash) 💥

## 3. 🛡️ 해결책: 깊은 복사 (Deep Copy)
복사 생성자를 직접 정의하여, **새로운 메모리를 할당하고 내용물만 복사**해야 합니다.

### 구현 예시
```cpp
class Inventory {
    vector<Item*> items;
public:
    // [복사 생성자]
    Inventory(const Inventory& other) {
        // 1. 일반 멤버 복사
        this->gold = other.gold;
        
        // 2. 깊은 복사 (Deep Copy)
        for (const Item* pItem : other.items) {
            // 원본(other)의 아이템 내용을 복사해서 새 Item을 만듦
            Item* newItem = new Item(pItem->GetName(), pItem->GetCost());
            this->items.push_back(newItem);
        }
    }
    // ... (기존 생성자/소멸자) ...
};
```

## 4. 📝 Rule of Three (3의 규칙)
클래스 내에서 동적 할당(`new`)을 사용해 리소스를 관리한다면, 다음 3가지를 반드시 직접 구현해야 합니다.
1. **소멸자 (Destructor)**: 리소스 해제 (`delete`)
2. **복사 생성자 (Copy Constructor)**: 깊은 복사 구현
3. **복사 대입 연산자 (Copy Assignment Operator)**: `A = B;` 형태의 깊은 복사 구현

> **Modern C++ Tip**: `std::unique_ptr`를 쓰면(Rule of Zero), 컴파일러가 알아서 복사를 금지해주거나 이동(Move)만 허용하여 훨씬 안전합니다. (Module 9에서 학습)
