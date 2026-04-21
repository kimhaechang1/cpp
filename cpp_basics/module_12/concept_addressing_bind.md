# 소켓 주소 설정과 서버의 대기: bind & listen

## 🏢 실무 상황 (The Scenario)
당신은 이제 게임 서버의 기초를 만들고 있습니다. 방금 생성한 소켓에게 다음과 같은 명령을 내려야 합니다.
- "너는 이제부터 **`127.0.0.1`** 주소의 **`7777`**번 포트에서 걸려오는 전화를 받아야 해."
- "그리고 혼자 놀지 말고, 손님이 올 때까지 **대기 상태**로 기다리고 있어."

이 과정을 위해 우리는 **주소 구조체**를 채우고, **`bind`**와 **`listen`**이라는 함수를 호출하게 됩니다.

---

## 📍 1. 주소 구조체: `sockaddr_in`

현대적인 IPv4 주소를 표현하기 위해 `sockaddr_in` 구조체를 사용합니다.

```cpp
sockaddr_in serverAddr;
serverAddr.sin_family = AF_INET;           // IPv4 주소 체계
serverAddr.sin_port = htons(7777);         // 포트 번호 (Big-Endian 변환 필수!)
serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 아무 랜카드로 들어오는 주소 수용
```

### 왜 `htons` (Host to Network Short)를 쓰나요?
이게 바로 네트워크 프로그래밍의 함정인 **엔디언(Endianness)** 때문입니다.
- **Intel CPU**: 숫자를 거꾸로 저장합니다. (Little Endian)
- **Network (Internet)**: 숫자를 똑바로 저장합니다. (Big Endian)

네트워크 상에서 숫자가 뒤바뀌면 포트 번호가 완전히 꼬여버리기 때문에, 반드시 **"네트워크용 정렬"**로 바꿔주는 `htons` 함수를 거쳐야 합니다.

---

## 🔗 2. 서버 개통 절차: `bind`와 `listen`

### 1) `bind()`: 소켓에 주소 입히기
식당으로 비치면 점포(Socket)에 주소지(IP/Port)를 등록하는 과정입니다.
```cpp
bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
```

### 2) `listen()`: 손님 맞이 대기
식당 문을 열고 손님이 줄을 설 수 있게 통로(Backlog)를 확보하는 과정입니다. 이제 소켓은 능동적으로 전화를 거는 입장이 아니라, 전화를 **기다리는 입장**이 됩니다.
```cpp
listen(listenSocket, SOMAXCONN); // 최대한 많은 손님을 줄 세우겠다!
```

---

## 🏛️ 3. 역사적 배경: 왜 `sockaddr*`로 형변환을 할까?

네트워크 API를 쓰다 보면 `(sockaddr*)&addr`와 같은 괴상한 형변환을 자주 보게 됩니다.

- **범용 인터페이스**: `bind`나 `connect` 함수는 IPv4뿐만 아니라 IPv6, 블루투스 등 **미래의 모든 주소 체계**를 받아낼 수 있어야 했습니다.
- **C언어식 다형성**: C++의 상속 개념이 없던 시절, 모든 주소 구조체의 **첫 2바이트(Address Family)**를 공통으로 맞추고, 나머지는 각자 용도에 맞게 다르게 쓰는 방식을 택했습니다.
- **결론**: 함수는 일단 범용 도안인 `sockaddr*`로 받고, 내부에서 첫 2바이트를 확인해 실제로는 어떤 주소인지 역추적하는 **C언어식 객체지향**의 산물입니다.

---

## 🔍 4. 하드웨어 시각화 (Endianness Verification)

내 컴퓨터와 네트워크가 숫자를 어떻게 다르게 보는지 코드로 직접 확인해 봅시다.

```cpp
#include <iostream>
#include <WinSock2.h>
#include <iomanip>

void VerifyEndianness() {
    uint16_t port = 7777; // 0x1E61
    uint16_t networkPort = htons(port); // 0x611E

    std::cout << "--- Endianness Check ---\n";
    std::cout << "Original Port: " << port << " (Hex: 0x" << std::hex << port << ")\n";
    std::cout << "Network  Port: " << networkPort << " (Hex: 0x" << std::hex << networkPort << ")\n";
    
    if (port != networkPort) {
        std::cout << "Result: Your CPU is Little-Endian. Swap occurred!\n";
    }
}
```

---

## ❓ Q&A (자가 진단)

1.  **`INADDR_ANY`는 무엇을 의미하나요?**
    - 정답: 서버 컴퓨터에 랜카드가 여러 개(유선, 무선, 가상랜 등) 있을 때, "어떤 IP로 들어오든 내 컴퓨터가 타겟이라면 다 받겠다"는 관대한 설정입니다. (값은 실제 0입니다.)
2.  **왜 서버 코드에서는 `inet_pton`을 사용하지 않았나요?**
    - 정답: 서버는 특정 목적지를 찾아가는 입장이 아니라, **"내 컴퓨터의 모든 문"**을 열어두는 입장이기 때문입니다. `INADDR_ANY`라는 0번 상수를 쓰는 것만으로도 모든 인터페이스 개방이 가능하므로, 굳이 문자열 IP를 숫자로 변환하는 `inet_pton` 과정을 거칠 필요가 없습니다.
3.  **그럼 `inet_pton`은 언제, 왜 쓰나요?**
    - 정답: 클라이언트가 특정 서버(예: "127.0.0.1")를 찾아갈 때 사용합니다. 사람은 "127.0.0.1"이라는 글자로 이해하지만, 컴퓨터는 이를 **4바이트 정수**로 이해해야 합니다. `inet_pton`은 사람이 보는 문자열(Presentation)을 네트워크용 숫자(Network)로 안전하게 변환해주는 역할을 합니다.
4.  **`bind` 단계에서 에러가 나는 가장 흔한 이유는?**
    - 정답: 이미 다른 프로그램(예: 웹 서버, 다른 게임 등)이 해당 포트 번호를 선점하고 있을 때 발생합니다. (`Address already in use`)

---

## 🔗 다음 단계
주소 설정의 원리를 이해하셨다면, `practice_server_setup.cpp`를 통해 직접 소켓에 번호를 부여하고 대기 상태로 만들어 보겠습니다.
