# C++ 시스템 프로그래밍 & 네트워크 완전 정복

## 🎓 개요
**"기초(Basic)가 아닌, 원리(Deep Dive)를 파헤칩니다."**
이 커리큘럼은 수박 겉핥기 식 이론 공부를 거부합니다. 
운영체제의 **커널(Kernel) 동작 원리**와 **네트워크 프로토콜의 바닥(Packet)**까지, C++로 직접 구현하며 **완벽하게 이해**하는 과정을 담았습니다.

---

## 🗺️ 학습 로드맵 (총 4단계)

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                 🖥️ LEVEL 1: OS ARCHITECTURE (운영체제 해부)                 │
├─────────────────────────────────────────────────────────────────────────────┤
│  MODULE 1: 컴퓨터 구조와 OS (CPU, Register, Interrupt, System Call)         │
│  MODULE 2: 프로세스 스케줄링 (Context Switch, Scheduling Algorithms)        │
│  MODULE 3: 메모리 가상화 (Paging, Segmentation, TLB, Cache)                 │
│  🔥 PROJECT: "나만의 작은 쉘(Shell) 만들기 & 메모리 할당기(Malloc) 구현"    │
├─────────────────────────────────────────────────────────────────────────────┤
│                 🧵 LEVEL 2: CONCURRENCY (동시성의 깊은 세계)                │
├─────────────────────────────────────────────────────────────────────────────┤
│  MODULE 4: 멀티스레딩 심화 (User vs Kernel Thread, Thread Pool)             │
│  MODULE 5: 동기화와 IPC (Mutex, Semaphore, Monitor, Pipe, Shared Memory)    │
│  MODULE 6: Deadlock과 Lock-free (ABA Problem, Atomics, Memory Model)        │
│  🔥 PROJECT: "멀티스레드 유저모드 스케줄러 (Goroutine 흉내내기)"            │
├─────────────────────────────────────────────────────────────────────────────┤
│                 🌐 LEVEL 3: NETWORK PROTOCOLS (네트워크 해부)               │
├─────────────────────────────────────────────────────────────────────────────┤
│  MODULE 7: 네트워크 계층 (OSI 7 Layer, TCP/IP Stack, Packet Analysis)       │
│  MODULE 8: TCP 심화 (Flow Control, Congestion Control, Handshake)           │
│  MODULE 9: HTTP & DNS (Application Layer 동작 원리)                         │
│  🔥 PROJECT: "Raw Socket으로 TCP 패킷 캡쳐 및 분석기(Sniffer) 만들기"       │
├─────────────────────────────────────────────────────────────────────────────┤
│                 🚀 LEVEL 4: HIGH PERFORMANCE I/O (고성능 통신)              │
├─────────────────────────────────────────────────────────────────────────────┤
│  MODULE 10: I/O 모델 진화 (Blocking -> Non-Blocking -> Select -> IOCP)      │
│  MODULE 11: 비동기 서버 설계 (Proactor vs Reactor Pattern)                  │
│  MODULE 12: 실시간 패킷 처리 (Serialization, Protobuf, RUDP)                │
│  🔥 PROJECT: "IOCP 기반 고성능 채팅 서버 (C10K 문제 해결)"                  │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## 📋 상세 상세 커리큘럼

### 🖥️ LEVEL 1: OS ARCHITECTURE
> "운영체제는 마법이 아닙니다. C언어로 짜여진 프로그램일 뿐입니다."

- [ ] **System Call**: `User Mode`와 `Kernel Mode`의 벽, `int 0x80`의 이해
- [ ] **Process Image**: ELF 파일 포맷, Loader가 프로그램을 메모리에 올리는 과정
- [ ] **Virtual Memory**: MMU가 주소를 변환하는 과정 시뮬레이션
- [ ] **Cache Coherence**: 하드웨어 캐시가 성능에 미치는 영향 확인

### 🧵 LEVEL 2: CONCURRENCY
> "동시성은 어렵지만, 정복하면 신세계가 열립니다."

- [ ] **Thread Pool**: 스레드 생성/파괴 비용 아끼기 (직접 구현)
- [ ] **IPC**: 프로세스 간 통신 (파이프로 데이터 보내기, 공유 메모리로 맵 공유하기)
- [ ] **Condition Variable**: "일이 끝날 때까지 자고 있어" (효율적인 대기)
- [ ] **Memory Order**: 컴파일러와 CPU의 명령어 재배치 막기 (`std::memory_order`)

### 🌐 LEVEL 3: NETWORK PROTOCOLS
> "소켓 함수만 쓰는 것은 네트워크를 아는 것이 아닙니다."

- [ ] **Wireshark 활용**: 내 코드가 보낸 패킷 눈으로 확인하기
- [ ] **TCP Retransmission**: 줄을 끊으면(패킷 유실) 정말 다시 보내는지 테스트
- [ ] **Nagle Algorithm**: 패킷 뭉쳐 보내기의 장단점 (게임에선 왜 끄는가?)
- [ ] **Endianness**: `ntohl`, `htonl`이 왜 필요한가?

### 🚀 LEVEL 4: HIGH PERFORMANCE I/O
> "게임 서버 개발자로 가는 마지막 관문입니다."

- [ ] **C10K Problem**: 접속자 1만 명을 동시에 처리하려면?
- [ ] **Event-Driven**: 이벤트를 기다리는 효율적인 방법
- [ ] **IOCP (Windows)**: 입출력 완료 포트의 원리
- [ ] **Serialization**: 객체를 바이트 배열로 직렬화하여 전송하기

---

## 💡 학습 방식 가이드
이 과정은 이론 서적(운영체제, 컴퓨터 네트워크)을 옆에 두고, **코드로 검증하는 방식**으로 진행됩니다.

1.  **이론**: 책을 통해 "TCP 3-way Handshake"를 읽는다.
2.  **검증**: Wireshark를 켜고, 내가 짠 서버-클라이언트가 실제로 SYN, SYN-ACK, ACK를 주고받는지 확인한다.
3.  **구현**: 소켓 라이브러리 없이, Raw Socket으로 직접 Handshake 패킷을 만들어 보내본다.

이 정도 침투력이면 "기초"라는 말은 쏙 들어갈 것입니다. 준비되셨나요?
