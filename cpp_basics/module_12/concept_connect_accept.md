# 서버의 응답과 클라이언트의 접속: accept & connect

## 🏢 실무 상황 (The Scenario)
서버는 이제 7777번 포트에서 손님을 기다리고 있습니다. 하지만 `listen()`만으로는 장사할 수 없습니다. 손님이 문을 열고 들어오면(`connect`), 서버는 그 손님만 전담해서 챙겨줄 **"새로운 새끼 소켓"**을 만들어야 합니다.

- **서버**: "어이구, 손님 오셨네요! 여기 안내원(새 소켓) 붙여드릴 테니 이분하고 대화하세요." (`accept`)
- **클라이언트**: "7777번 사장님 계신가요? 저 들어가고 싶습니다!" (`connect`)

---

## 👂 1. 서버의 안내원 배정: `accept()`

서버 입장에서 가장 중요한 개념은 **소켓이 2개가 된다**는 점입니다.

1.  **리스닝 소켓(Listening Socket)**: 문 앞에 서서 "손님 오나?"만 체크하는 문지기입니다. (가이드 전용)
2.  **클라이언트 전용 소켓(Client Socket)**: `accept()`가 성공하면 반환되는 소켓입니다. 이 소켓이 있어야만 비로소 특정 손님과 1:1로 데이터를 주고받을 수 있습니다.

```cpp
sockaddr_in clientAddr{};
int addrLen = sizeof(clientAddr);

// 손님이 올 때까지 여기서 잠시 멈춥니다 (Blocking)
SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &addrLen);
```

---

## 📞 2. 클라이언트의 접속 요청: `connect()`

클라이언트는 서버보다 훨씬 간단합니다. 서버의 집 주소(IP)와 방 번호(Port)만 알면 됩니다.

```cpp
sockaddr_in serverAddr{};
serverAddr.sin_family = AF_INET;
inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // 문자열 IP를 숫자로 변환
serverAddr.sin_port = htons(7777);

// 서버에게 접속을 요청합니다.
connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
```

> [!NOTE]
> 클라이언트는 서버처럼 `bind()`를 할 필요가 없습니다. `connect()`를 호출하는 순간, 운영체제가 남는 포트 번호 중 하나를 무작위로 골라 자동으로 주소를 입혀주기 때문입니다. (임시 포트)

---

## 🔍 3. 하드웨어 시각화 (Socket Duo on Server)

서버가 손님 한 명을 받으면, 메모리 상에는 아래와 같은 모습이 나타납니다.

```cpp
// 1. 문지기 소켓 (Listen 전용)
SOCKET listenSocket = socket(...); // 예: ID 284

// 2. accept 성공 후 (통신 전용)
SOCKET clientSocket = accept(listenSocket, ...); // 예: ID 312 (새로운 번호!)

std::cout << "Listening Socket ID: " << listenSocket << "\n";
std::cout << "Active Client Socket ID: " << clientSocket << "\n";
```
이처럼 **두 소켓의 번호가 다르다**는 점을 이해하는 것이 네트워크 프로그래밍 중급자로 가는 핵심입니다.

---

## ❓ Q&A (자가 진단)

1.  **왜 서버는 소켓을 하나 더 만드나요?**
    - 정답: 문지기(`listenSocket`)가 손님 한 명하고 수다 떨기 시작하면, 다른 손님이 왔을 때 문을 열어줄 사람이 없어지기 때문입니다. 문지기는 계속 문 앞에 있어야 하고, 손님은 안내원에게 맡겨야 합니다.
2.  **`inet_pton`은 무슨 뜻인가요?**
    - 정답: **P**resentation(문자열 IP) **to** **N**etwork(숫자 IP)의 약자입니다. "127.0.0.1" 같은 글자를 컴퓨터가 읽기 편한 숫자로 바꿔줍니다.

---

## 🔗 다음 단계
이제 서버와 클라이언트라는 두 개의 프로그램을 동시에 띄워놓고, 실제로 둘이 서로의 존재를 인식하는 **"역사적인 첫 접속"** 실습을 진행하겠습니다.
