# [Module 13] writefds 활용 및 논블로킹(Non-blocking) 소켓 완벽 이해

## 1. 블로킹 소켓의 치명적 단점과 논블로킹의 필요성
OS가 처음 `accept()`로 만들어주는 소켓은 기본적으로 **블로킹(Blocking)** 모드입니다.
만약 블로킹 상태의 에코 서버에 악의적인 클라이언트가 접속해서, 데이터 수신(recv)은 절대 하지 않고 송신(send)만 무한정 한다면 어떻게 될까요?
1. 클라이언트의 수신 버퍼가 가득 참
2. 서버의 송신 버퍼도 뒤이어 가득 참
3. 서버가 `send()`를 호출하는 순간 **메인 스레드가 영원히 기절(Sleep)함**
4. 단 하나의 스레드로 돌아가는 `select()` 서버 전체가 프리징(Freezing)되어 다른 정상 유저들까지 모두 멈추는 대참사 발생!

이러한 문제를 막기 위해, 실무에서는 **반드시 모든 소켓을 논블로킹(Non-blocking) 모드로 강제 전환**하여, 버퍼가 꽉 차더라도 스레드가 기절하지 않고 즉시 에러(`WSAEWOULDBLOCK`)를 반환하도록 설계해야 합니다.

## 2. TCP 파이프라인과 버퍼의 실체 (64KB vs 192KB)
실습에서 서버의 OS 송신 버퍼 크기는 64KB였는데, 어떻게 190KB 이상의 데이터를 보내야만 버퍼가 꽉 찼다는 에러가 떴을까요?
이것은 `send()`의 성공이 "상대방이 데이터를 완벽히 읽어갔다"는 뜻이 아니라, **"TCP 파이프라인에 데이터를 안전하게 밀어 넣었다"**는 뜻이기 때문입니다.

TCP 통신은 다음과 같은 거대한 파이프 구조를 가집니다.
`[서버 측 송신 버퍼(약 64KB)]  ->  [네트워크 망]  ->  [클라이언트 측 수신 버퍼(약 128KB)]`

서버가 냅다 데이터를 쏘면 이 데이터들은 랜선을 타고 넘어가 클라이언트의 수신 버퍼부터 차곡차곡 쌓입니다. 클라이언트 수신 버퍼가 가득 차면 그제서야 서버의 송신 버퍼에 데이터 고이기 시작합니다. 결과적으로 **양쪽 버퍼가 모두 100% 꽉 찼을 때에야 비로소 `send()`가 통로가 막혔다는 `WSAEWOULDBLOCK` 에러를 뿜어냅니다.**

## 3. writefds의 올바른 활용법 (코드 분석)
`writefds`는 OS에게 "내 전용 송신 버퍼에 빈 공간이 생기면 즉시 알려줘!"라고 부탁하는 알람 기능입니다. 
평소에 빈 공간이 널널할 때 소켓을 넣으면 `select()`가 무한정 즉시 리턴되어 CPU 점유율이 100%로 치솟게 됩니다. 따라서 **오직 `WSAEWOULDBLOCK` 에러가 발생하여 통로가 막혔을 때만 제한적으로 소켓을 `writefds`에 넣어야 합니다.**

### 실습 코드 요약: 송신 버퍼 폭파 및 복구 루틴
```cpp
// 1. 소켓을 논블로킹 모드로 강제 전환 (accept 직후 가장 먼저 해야 할 일)
u_long mode = 1;
ioctlsocket(clientSock, FIONBIO, &mode);

// 2. 버퍼가 꽉 찰 때까지 무한 송신 (악의적 클라이언트가 안 읽어가는 상황 가정)
while(true) {
    int ret = send(clientSock, dummyData, sizeof(dummyData), 0);
    if (ret == SOCKET_ERROR) {
        if (WSAGetLastError() == WSAEWOULDBLOCK) {
            // 🚨 TCP 파이프라인(양쪽 OS 버퍼)이 완전히 꽉 차서 더 이상 못 보냄!
            break; 
        }
    }
}

// 3. 통로가 막혔으니 해당 소켓을 writefds에 등록하고 대기
fd_set writefds;
FD_ZERO(&writefds);
FD_SET(clientSock, &writefds);

// 4. 클라이언트가 데이터를 꿀꺽 소화해서 서버 송신 버퍼에 빈 공간이 생기면 select() 리턴
int selRet = select(0, nullptr, &writefds, nullptr, nullptr);
if (selRet > 0 && FD_ISSET(clientSock, &writefds)) {
    // 🎉 빈 공간 확보! 다시 남은 데이터 전송 재개
    send(clientSock, dummyData, sizeof(dummyData), 0);
}
```

## 4. 핵심 요약
1. `readfds`는 **항상** 감시합니다. (언제 누가 접속할지, 언제 데이터를 보낼지 모르니까)
2. `writefds`는 **막혔을 때만(`WSAEWOULDBLOCK`)** 감시합니다. (평소엔 그냥 바로 쏘면 되니까)
3. 모든 소켓은 `accept` 직후 무조건 **논블로킹(`FIONBIO`)**으로 바꿔주어 단일 스레드를 보호해야 합니다.
