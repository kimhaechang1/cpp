# Session Checkpoint

## Current Goal
- [Module 13] 서버 아키텍처 기초 학습 (동기식 다중 처리 구현)
- `ThreadPool` 모델 설계를 통한 멀티스레딩 및 동기화 기술 숙달
- **[진행 중]** Half-Sync/Half-Async (하이브리드 아키텍처) 기반 서버 통신 구축

## Completed Tasks
- [x] **ThreadPool 기본 구조 구현 완료**: `std::vector<std::thread>`와 `std::queue<std::function<void()>>`를 이용한 작업 큐잉 시스템 설계 완료.
- [x] **동기화 기법 심화 학습**: `std::unique_lock`, Spurious Wakeup 방어, OS Wait Queue 분석.
- [x] **가시성(Visibility)과 상호 배제**: Memory Barrier 메커니즘 검증 및 문서화 완료.
- [x] **Windows 환경 테스트 디버깅**: Windows Timer Resolution 분석 및 RAII 소멸자 안전 보장.
- [x] **Selector 모델 (I/O Multiplexing) 학습**: `select()`와 `fd_set`을 통한 Non-blocking 소켓 처리.
- [x] **Thread & Selector 연동 미니 실습**: Thread Pool과 `select()` 결합, 람다 값 캡처(`[=]`), Master-Replica `fd_set` 설계 패턴 등 아키텍처 이론 확립 및 실습 성공 (`practice_hybrid_server.cpp`).

## Next Steps
- **[Cumulative Practice 2] Hybrid Architecture Server**: Thread Pool(동기 다중 처리)과 Select(비동기 I/O 감시) 모델을 완전히 결합하고, 방금 문서에 추가한 **`writefds` 비동기 송신 큐(SendQueue) 기법**까지 도입한 실전적인 게임 서버 아키텍처 구현 (SessionManager 도입 예상).

## Key Constraints
- 에이전트는 C++17 및 최신 2025 표준 아키텍처를 기반으로 설명할 것.
- 모든 답변 시 "왜 이렇게 동작하는가?"에 대한 OS 및 아키텍처 레벨의 샌드위치 교육법 유지할 것.
- 하드웨어 검증이나 로우레벨 작동 방식이 궁금한 Q&A는 무조건 문서(`concept_*.md`)에 기록할 것.
