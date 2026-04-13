# Module 11: Concurrency & Multithreading

> C++23 멀티스레드 프로그래밍의 핵심 개념과 실습을 다룹니다.

---

## 개념 문서 (Concepts)

| 순서 | 파일 | 내용 |
|------|------|------|
| 1 | [concept_thread_basics.md](concept_thread_basics.md) | 스레드 기초: 프로세스 vs 스레드, `std::thread`, join/detach |
| 2 | [concept_mutex.md](concept_mutex.md) | 뮤텍스와 경쟁 상태: `std::mutex`, `std::lock_guard`, Race Condition |
| 3 | [concept_jthread.md](concept_jthread.md) | `std::jthread`, `std::stop_token`, 협력적 중단, `yield()` |

## 실습 파일 (Practice)

| 파일 | 유형 | 내용 |
|------|------|------|
| [practice_threads.cpp](practice_threads.cpp) | 단독 실습 | 스레드 생성, 인자 전달, `std::ref` |
| [practice_mutex.cpp](practice_mutex.cpp) | 단독 실습 | 경쟁 상태 재현 및 `lock_guard`로 해결 |
| [practice_jthread.cpp](practice_jthread.cpp) | 단독 실습 | `jthread` 자동 조인, `stop_token` 중단 패턴 |

## 누적 실습 (Cumulative Practice - TDD)

| 파일 | 역할 |
|------|------|
| [test_dungeon_raid.cpp](test_dungeon_raid.cpp) | GTest 테스트 명세 (10개 테스트 케이스) |
| [Monster.h](Monster.h) / [Monster.cpp](Monster.cpp) | 다형성 몬스터 (추상 클래스 + Goblin/Dragon) |
| [DungeonRaid.h](DungeonRaid.h) / [DungeonRaid.cpp](DungeonRaid.cpp) | 스레드 안전 던전 공략 시스템 |
| [CMakeLists.txt](CMakeLists.txt) | 빌드 설정 (GTest + vcpkg 연동) |
