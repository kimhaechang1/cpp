# Session Checkpoint (2026-04-02)

## Current Goal
- **Module 10 (프로페셔널 도구/Toolchain)** 진입: CMake 빌드 시스템의 기본 원리 및 멀티 소스 프로젝트 구성법 학습.

## Completed Tasks
- ✅ **Module 09 (모던 C++) 마침**: RAII, 스마트 포인터, 이동 시맨틱, 현대적 타입(optional/variant/expected) 마스터.
- ✅ **Q&A 정리(Move Semantics)**: 
  - 이동 생성자(초기화) vs 이동 대입 연산자(대입)의 동작 차이 이론 정립.
  - `std::map::operator[]`의 반환 형태에 따른 이동 시맨틱 연쇄 동작 분석 완료.
- ✅ **환경 설정**: `.vscode/` 폴더의 `.gitignore` 제외 및 Git 캐시 처리 가이드 완료.

## Next Steps
1. **Module 10 폴더(`cpp_basics/module_10/`)** 생성.
2. **`CMakeLists.txt`** 파일 작성 및 `cmake` 명령어로 빌드 환경(Build directory) 독립 구성 연습.
3. g++ 직접 호출 명령어 방식에서 벗어나, 추상화된 빌드 자동화 도구에 익숙해지기.

## Key Constraints & Context
- **컴파일 스펙**: C++20/23 기능을 계속 사용할 예정이므로 `set(CMAKE_CXX_STANDARD 23)` 설정 필요.
- **학습 철학**: 단순 도구 사용법이 아니라 "왜 소스 코드와 빌드 결과물을 분리해야 하는가(Out-of-source build)"라는 철학적 관점에서 접근할 것.
