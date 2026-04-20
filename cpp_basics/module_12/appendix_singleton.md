# 부록: C++ 싱글톤(Singleton) 패턴의 구현 방식 비교

싱글톤 패턴은 클래스의 인스턴스를 오직 하나만 생성하도록 보장하고, 어디서든 그 인스턴스에 접근할 수 있게 하는 디자인 패턴입니다.

---

## 1. 전통적인 방식 (Raw Pointer)

C++11 이전이나 아주 오래된 레거시 코드에서 흔히 볼 수 있는 방식입니다.

```cpp
class NetworkManager {
private:
    static NetworkManager* instance;
    NetworkManager() {} // 생성자 은닉

public:
    static NetworkManager* GetInstance() {
        if (instance == nullptr) {
            instance = new NetworkManager();
        }
        return instance;
    }
    // 인스턴스를 직접 delete 해주는 별도의 함수가 필요하거나, 프로그램 종료 시까지 방치됨
};
NetworkManager* NetworkManager::instance = nullptr;
```

- **장점**: 구현이 단순하고 직관적입니다.
- **단점**: 
    - **메모리 누수**: 누군가 명시적으로 `delete` 해주지 않으면 프로그램 종료 시까지 힙 메모리에 남습니다.
    - **멀티스레드 위험**: 여러 스레드가 동시에 `GetInstance()`를 호출하면 인스턴스가 두 개 생길 수 있습니다.

---

## 2. 스마트 포인터 방식 (Modern C++ - 현재 적용 방식)

훈련생님이 오늘 사용하신 방식입니다. `shared_ptr`나 `unique_ptr`를 사용하여 수명 주기를 관리합니다.

```cpp
class NetworkManager {
private:
    static inline shared_ptr<NetworkManager> manager = nullptr;
    NetworkManager() {}

public:
    static shared_ptr<NetworkManager> GetInstance() {
        if (manager == nullptr) {
            // private 생성자 접근을 위한 Inner Class 기법 활용 가능
            struct _Inner : public NetworkManager {};
            manager = make_shared<_Inner>();
        }
        return manager;
    }
};
```

- **장점**: 
    - **자동 해제**: 참조 횟수가 0이 되거나 프로그램 종료 시 자동으로 메모리가 해제됩니다 (RAII).
    - **소유권 명확**: `shared_ptr`를 통해 관리 주체를 명확히 할 수 있습니다.
- **단점**: 구현이 다소 복잡하고(Inner class 트릭 등), 스마트 포인터 자체의 오버헤드가 미세하게 존재합니다.

---

## 3. 가장 권장되는 방식 (Meyers' Singleton)

C++11 표준 이후, 가장 깔끔하고 안전한 방식으로 통용되는 기법입니다.

```cpp
class NetworkManager {
private:
    NetworkManager() {} // 생성자 은닉
    ~NetworkManager() {} // 소멸자 은닉

public:
    // 복사와 대입을 원천 봉쇄합니다.
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;

    static NetworkManager& GetInstance() {
        // 지역 정적 변수는 프로그램 생애 주기에 단 한 번만 초기화됩니다.
        static NetworkManager instance; 
        return instance;
    }
};
```

- **장점**:
    - **Thread-Safe**: C++11 표준에 따라, 정적 지역 변수의 초기화는 멀티스레드 환경에서도 단 한 번만 일어남이 보장됩니다. (동기화 처리가 필요 없음)
    - **가장 단순함**: 별도의 포인터 관리나 동적 할당 없이 스코프가 끝나면(프로그램 종료 시) 자동으로 파괴됩니다.
    - **지연 초기화 (Lazy Initialization)**: `GetInstance()`가 처음 호출되는 순간에만 객체가 생성됩니다.

---

## 💡 결론: 어떤 것을 써야 할까요?

1.  **단순하고 안전한 전역 관리자**가 필요하다면? -> **Meyers' Singleton** (방식 3)
2.  **객체의 생성/소멸 시점을 정밀하게 제어**하고 싶거나, 여러 곳에서 소유권을 공유해야 한다면? -> **스마트 포인터 방식** (방식 2)
3.  **Raw Pointer(방식 1)**는 현대적인 C++ 프로젝트에서는 가급적 피하는 것이 좋습니다.

오늘 훈련생님이 구현하신 **방식 2**는 싱글톤을 `shared_ptr`로 관리함으로써 수동 삭제(`Destory_Instance`)의 제어권을 가지면서도 누수를 방지한 아주 훌륭한 중간 단계의 선택이었습니다!
