# MODULE 12: 네트워크 프로토콜 기초 (Winsock2)

네트워크 프로그래밍은 단순히 데이터를 보내고 받는 것을 넘어, 물리적으로 떨어진 두 컴퓨터가 어떻게 서로의 존재를 확인하고 신뢰할 수 있는 데이터를 교환하는지 이해하는 과정입니다.

## 📋 학습 목차
1. **Winsock2 환경 세팅**: [concept_winsock_setup.md](concept_winsock_setup.md)
   - Windows API 네트워크 초기화 및 라이브러리 링크 방법
2. **WinAPI 자료형과 비트 연산**: [concept_winapi_types.md](concept_winapi_types.md)
   - WORD, MAKEWORD 및 0xFF 비트 마스크 원리 정리
3. **[부록] 싱글톤 패턴 구현 가이드**: [appendix_singleton.md](appendix_singleton.md)
   - 스마트 포인터 vs Raw Pointer vs Meyers 방식 비교
4. **신뢰성의 TCP (Transmission Control Protocol)**: (대기 중)
   - 스트림 기반 연결 및 3-Way Handshake 이해
3. **속도의 UDP (User Datagram Protocol)**: (대기 중)
   - 비연결형 데이터 전송 및 브로드캐스트 기초
4. **누적 실습**: (대기 중)
   - TCP/UDP 기반 게임 데이터 직렬화 및 전송

---
> 본 모듈은 Windows 환경에서 `g++` 및 `CMake`를 활용하여 진행됩니다.
