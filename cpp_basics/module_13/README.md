# Module 13: 서버 아키텍처 기초

이 폴더는 진짜 게임 서버의 기틀이 되는 동기식 스레드풀과 멀티플렉싱 비동기 I/O 모델을 학습하는 공간입니다.

## 📋 목차 (Table of Contents)

1. [Thread Pool 모델 (동기식 다중 처리)](concept_thread_pool.md)
   - 일꾼(Thread)들을 미리 만들어두고 클라이언트 요청을 Queue를 통해 효율적으로 처리하기
2. [Selector 모델 (비동기 처리 기초)](concept_selector_model.md)
   - `select()` 함수와 `fd_set`을 활용한 Non-blocking 다중 I/O 처리의 기본
3. [writefds 활용 및 논블로킹 소켓 완벽 이해](concept_writefds_and_nonblocking.md)
   - 논블로킹 소켓의 필요성, TCP 송수신 버퍼 파이프라인, 그리고 `writefds`의 실무적 활용법
