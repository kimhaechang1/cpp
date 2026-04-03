# Module 10: 프로페셔널 도구 (Toolchain & CMake)

이 모듈에서는 C++ 소스 코드를 실행 파일로 빌드하기 위해 필요한 상용 수준의 도구 체인(Toolchain)을 다룹니다.

## 📜 학습 주제 (Curriculum Contents)

1. [CMake 빌드 시스템 기초](./concept_cmake_basics.md)
   - 전처리, 컴파일, 어셈블, 링크의 4단계 공정 이해.
   - `CMakeLists.txt` 작성법 및 `Out-of-source build` 철학.
   
2. [패키지 매니저 (vcpkg) 기초](./concept_vcpkg_basics.md)
   - 외부 라이브러리(`fmt` 등) 연동을 위한 패키지 관리 도구 활용.
   - `find_package`와 `target_link_libraries` 기반 의존성 관리.

3. **향후 학습 내용**
   - AddressSanitizer(ASan)를 이용한 메모리 오염 실시간 탐지.
   - Google Test를 활용한 단위 테스트(Unit Test) 도입.
   - .gitignore 작성 및 실전적인 협업을 위한 Git 버전 관리 전략.

---

## 🏗️ 실습 프로젝트 (Practice Projects)

- `main.cpp`: vcpkg로 설치한 `fmt` 라이브러리를 활용한 결과 출력.
- `Calculator.h / .cpp`: 헤더와 소스 코드가 분리된 멀티 파일 프로젝트 빌드 실습.
- `CMakeLists.txt`: 프로젝트 구성 및 외부 라이브러리 연동 설계도.

---

_각 문서의 내용을 숙지한 후, 궁금한 점은 언제든 질문해 주세요!_
