# TCP 프로토콜 기초: 신뢰성과 연결 (Reliability & Connection)

## 🏢 실무 상황 (The Scenario)
당신은 방금 전 세션에서 윈도우 네트워크 환경(Winsock2)을 성공적으로 초기화했습니다. 이제 실제로 데이터를 주고받아야 합니다. 하지만 인터넷은 생각보다 훨씬 위험하고 불친절한 곳입니다.
- 보낸 패키지가 순서가 뒤바뀌어 도착할 수도 있습니다.
- 중간에 패키지가 유실(Loss)될 수도 있습니다.
- 상대방이 데이터를 받을 준비가 안 되었는데 무작정 들이부을 수도 있습니다.

이 모든 아수라장을 해결하고, **"마치 직접 선을 연결한 것처럼"** 안전하게 데이터를 전달해주는 마법의 규칙이 바로 **TCP(Transmission Control Protocol)**입니다.

---

## 🤝 1. TCP의 핵심: 3-Way Handshake

TCP는 데이터를 보내기 전, 반드시 상대방과 "안면"을 트는 과정이 필요합니다. 이를 **3-Way Handshake**라고 부릅니다.

1.  **SYN (Synchronize)**: 클라이언트가 서버에게 "안녕? 나랑 연결할래? 내 번호(Sequence Number)는 X야."라고 물어봅니다.
2.  **SYN-ACK (Synchronize-Acknowledgment)**: 서버가 응답합니다. "안녕! 나도 좋아. 네 번호 X는 잘 확인했어(ACK). 내 번호는 Y야(SYN)."
3.  **ACK (Acknowledgment)**: 클라이언트가 최종 확인합니다. "오케이! 네 번호 Y도 확인했어. 이제 통신 시작하자!"

> [!IMPORTANT]
> 이 과정이 끝나야만 비로소 **`ESTABLISHED`** 상태가 되며, 데이터 전송이 가능해집니다. 게임에서 "서버 연결 중..."이라는 메시지가 뜨는 구간이 바로 여기입니다.

---

## 📞 2. 통신 창구: 소켓(Socket)의 생성

이제 통신을 위한 "전화기" 기기가 필요합니다. 이를 **소켓(Socket)**이라고 부릅니다. C++(Winsock)에서 소켓은 단순한 숫자가 아니라, 운영체제가 관리하는 **핸들(Handle)** 형식으로 취급됩니다.

### `socket()` 함수 파라미터 이해
```cpp
SOCKET handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
```

1.  **`AF_INET` (Address Family)**: 어떤 주소 체계를 쓸 것인가? (IPv4를 의미)
2.  **`SOCK_STREAM` (Socket Type)**: 어떤 방식으로 데이터를 보낼 것인가? (TCP의 특징인 '흐름/스트림' 방식)
3.  **`IPPROTO_TCP` (Protocol)**: 구체적으로 어떤 프로토콜인가? (TCP)

---

## 🌊 3. 소켓의 "타입"과 "스트림"의 비밀

사용자가 질문해주신 **스트림(Stream)**과 **타입(Type)**의 구체적인 차이를 정리합니다.

### 왜 `SOCK_STREAM`이라고 부를까?
"스트림"은 수도꼭지에서 나오는 **물줄기**를 생각하면 쉽습니다.
- **경계가 없음 (Boundary-less)**: 보내는 쪽에서 "Hello"라고 보내도, 받는 쪽에서는 "He"를 먼저 받고 나중에 "llo"를 받을 수 있습니다. 즉, 데이터의 '조각'들이 하나의 큰 흐름으로 이어집니다.
- **순서와 신뢰성**: 물줄기가 중간에 새지 않고 순서대로 도착하는 것처럼, TCP는 운영체제가 데이터의 순서와 도착 여부를 끝까지 책임집니다.

### 타입의 종류: `SOCK_STREAM` vs `SOCK_DGRAM`
운영체제에게 "어떤 방식의 포장지를 쓸까?"를 물어보는 것입니다.

1.  **`SOCK_STREAM` (TCP)**: 
    - 연결 필수, 순서 보장, 안전함.
    - 게임 내 **아이템 결제, 채팅, 퀘스트 수락** 등 정확도가 생명인 곳에 사용.
2.  **`SOCK_DGRAM` (UDP)**: 
    - 연결 불필요, 빠름, 유실 가능성 있음.
    - 게임 내 **플레이어 위치 이동, 동기화** 등 속도가 생명인 곳에 사용.

---

## 🔍 4. 하드웨어 시각화 (Hardware Verification)

소켓은 실제로 메모리 상에 어떤 모습으로 존재할까요? 소켓 핸들은 단순히 메모리 주소가 아니라 운영체제가 부여한 **식별 번호**입니다.

```cpp
#include <iostream>
#include <WinSock2.h>

void VerifySocket() {
    // 1. 소켓 생성
    SOCKET testSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    // 2. 소켓 핸들의 크기 확인 (64비트 환경에서는?)
    std::cout << "Socket Handle Size: " << sizeof(testSocket) << " bytes\n";
    
    // 3. 소켓 핸들 값 확인 (운영체제가 부여한 번호)
    std::cout << "Socket ID (Handle): " << testSocket << "\n";
    
    if (testSocket == INVALID_SOCKET) {
        std::cout << "Failed to create socket. Error: " << WSAGetLastError() << "\n";
    }

    // 4. 리소스 반납
    closesocket(testSocket);
}
```

### 🧠 Deep Dive: `INVALID_SOCKET`과 `~0`의 비밀
헤더 파일을 보면 `INVALID_SOCKET`이 `(SOCKET)(~0)`으로 정의된 것을 볼 수 있습니다. 이게 무엇을 의미할까요?

1.  **비트 반전 (`~`)**: `0`의 모든 비트를 뒤집는다는 뜻입니다.
    - `0` (32bit): `00000000...`
    - `~0` (32bit): `11111111...` (16진수로 `0xFFFFFFFF`)
2.  **왜 이렇게 쓸까?**:
    - 리눅스는 소켓을 `int`로 다루지만, 윈도우는 `unsigned int` 계열인 `UINT_PTR`로 다룹니다.
    - 부호 없는 정수에서 **"가장 큰 값"**을 에러 메시지로 쓰기 위해 비트를 꽉 채운 형태를 사용하는 것입니다. 사실상 부호 있는 정수의 `-1`과 비트 패턴이 동일합니다.
3.  **핸들(Handle) vs 포인터**: 소켓 핸들은 메모리 주소가 아니라, 운영체제가 관리하는 **장부의 번호**입니다. 사용자 모드에서 이 번호를 가지고 직접 커널 메모리를 건드릴 수는 없습니다.

---

## ❓ Q&A (자가 진단)

1.  **왜 게임에서 UDP보다 TCP가 "느리다"고 평가받을까요?**
    - 정답: 위에서 배운 3-Way Handshake 과정과, 데이터가 유실되었을 때 다시 받는 재전송 로직 때문입니다. 하지만 신뢰성이 필수인 게임 로비, 경매장, 채팅 등에는 TCP가 필수적입니다.
2.  **소켓 핸들이 `INVALID_SOCKET`(-1)을 반환했다면 무엇이 문제일까요?**
    - 정답: 가장 흔한 원인은 `WSAStartup`을 호출하지 않았거나, 시스템의 소켓 자원이 고갈된 경우입니다.

---

## 🔗 다음 단계
이 문서를 읽고 궁금한 점이 해결되었다면, `practice_tcp_socket.cpp`를 통해 직접 소켓을 생성하고 운영체제로부터 할당받는 실습을 진행하겠습니다.
