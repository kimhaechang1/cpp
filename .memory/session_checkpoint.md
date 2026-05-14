# Session Checkpoint

## Current Goal
- [Module 13] 서버 아키텍처 기초 학습 (동기식 다중 처리 구현)
- `ThreadPool` 모델 설계를 통한 멀티스레딩 및 동기화 기술 숙달

## Completed Tasks
- [x] **ThreadPool 기본 구조 구현 완료**: `std::vector<std::thread>`와 `std::queue<std::function<void()>>`를 이용한 작업 큐잉 시스템 설계 완료.
- [x] **동기화 기법 심화 학습**:
    - `std::unique_lock`을 사용하는 이유 (`cv.wait` 내부의 unlock 로직 지원).
    - `condition_variable`의 두 번째 인자(조건식)를 통한 **Spurious Wakeup(가짜 깨어남)** 방어.
    - OS의 대기실(Wait Queue)과 `notify_one()` / `notify_all()`의 정확한 동작 메커니즘 차이 분석.
- [x] **가시성(Visibility)과 상호 배제**:
    - `std::mutex`의 진짜 역할(Mutual Exclusion + Memory Barrier)에 대한 깊이 있는 질의응답 문서화 완료.
    - 하드웨어 CPU 캐시 동기화 지연 문제 및 컴파일러 최적화 방어 원리 숙지 완료.
- [x] **Windows 환경 테스트 디버깅**:
    - Windows Timer Resolution(`sleep_for(2ms)`가 약 15.6ms로 동작하는 OS 한계) 이슈 확인 및 해결.
    - 소멸자 안에서의 변수 조작(`bStopPool = true`) 및 RAII를 활용한 안전한 테스트 종료(Scope) 구현 완료.

## Next Steps
- **Selector 모델 도입**: 블로킹(Blocking) I/O의 한계를 극복하기 위해 `select()` 기반 Non-blocking 모델과 다중 I/O(Multiplexing) 모델 학습 예정.
- 하이브리드 아키텍처 서버 구현 시도 (이전에 만든 네트워크 시스템과 Thread Pool의 통합).

## Key Constraints
- 에이전트는 C++17 및 최신 2025 표준 아키텍처를 기반으로 설명할 것.
- 모든 답변 시 "왜 이렇게 동작하는가?"에 대한 OS 및 아키텍처 레벨의 샌드위치 교육법 유지할 것.
- 하드웨어 검증이나 로우레벨 작동 방식이 궁금한 Q&A는 무조건 문서(`concept_*.md`)에 기록할 것.
