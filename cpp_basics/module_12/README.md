# MODULE 12: 네트워크 프로토콜 기초 (Winsock2)

네트워크 프로그래밍은 단순히 데이터를 보내고 받는 것을 넘어, 물리적으로 떨어진 두 컴퓨터가 어떻게 서로의 존재를 확인하고 신뢰할 수 있는 데이터를 교환하는지 이해하는 과정입니다.

## 📋 학습 목차
- [concept_winsock_setup.md](concept_winsock_setup.md): 환경 세팅 및 기초 이론
- [concept_winapi_types.md](concept_winapi_types.md): Windows API 타입 (WORD, LPVOID 등)
- [concept_tcp_protocol.md](concept_tcp_protocol.md): TCP 3-Way Handshake 및 소켓 생성 원리
- [concept_addressing_bind.md](concept_addressing_bind.md): 주소 설정(Endianness) 및 Bind/Listen 원리
- [concept_connect_accept.md](concept_connect_accept.md): 서버의 응답(Accept)과 클라이언트의 접속(Connect)
- [concept_send_recv.md](concept_send_recv.md): 데이터 송수신(Send/Recv)과 스트림의 이해
- [concept_udp_protocol.md](concept_udp_protocol.md): **[NEW]** UDP 프로토콜과 비연결형 통신의 이해
- [appendix_endianness.md](appendix_endianness.md): 엔디언(Big/Little) 및 변환 함수 총정리
- [appendix_singleton.md](appendix_singleton.md): Singleton 패턴 부록
   - 스마트 포인터 vs Raw Pointer vs Meyers 방식 비교

---
## 🏁 진행 상태
- [x] **1. Winsock2 환경 세팅**
- [x] **2. TCP 기초 및 연결 생명주기**
- [x] **3. 신뢰성의 TCP 데이터 송수신**
- [ ] **4. 속도의 UDP (User Datagram Protocol)**: (진행 중 🏃)
- [ ] **5. 누적 실습**: TCP/UDP 기반 게임 데이터 직렬화 및 전송

---
> 본 모듈은 Windows 환경에서 `g++` 및 `CMake`를 활용하여 진행됩니다.
