# Module 10: CMake 빌드 시스템 기초 (Part 1)

## 🏗️ 1. 빌드 시스템(Build System)이란?
C++ 소스 코드(`.cpp`)가 실행 파일(`.exe`)이 되기까지는 다음과 같은 **4단계의 정밀 공정**을 거칩니다:

1. **전처리 (Preprocessing)**: `#include`, `#define` 등을 처리하여 텍스트 소스를 확장합니다.
2. **컴파일 (Compiling)**: C++ 코드를 저수준의 어셈블리어(`.s`)로 번역하고 최적화합니다.
3. **어셈블 (Assembling)**: 어셈블리어를 CPU가 이해하는 기계어(이진수)인 목적 파일(`.obj`)로 변환합니다.
4. **링크 (Linking)**: 여러 목적 파일과 라이브러리를 하나로 묶어 최종 실행 파일(`.exe`)을 완성합니다.
   - **중요**: `#include`는 '헤더 파일(메뉴판)'만 가져올 뿐, 실제 기능을 담은 '라이브러리(요리)'는 링크 단계에서 링커가 직접 찾아 연결해 줍니다. 

---

## 💎 2. CMake의 역할: Meta-Build System
CMake는 그 자체로 컴파일러가 아니라, **'빌드 설계도를 그리는 도구(Generator)'**입니다. 

### 🎯 핵심 설계 규칙 (CMakeLists.txt)
1. **버전 명시**: `cmake_minimum_required(VERSION 3.10)` - 최소 사양 정의
2. **프로젝트명**: `project(MyProject)` - 프로젝트 이름 설정
3. **표준 설정**: `set(CMAKE_CXX_STANDARD 23)` - 최신 C++ 규격 사용 (우리의 원칙!)
4. **타겟 생성**: `add_executable(MyGame main.cpp)` - `main.cpp`를 컴파일해서 `MyGame.exe`를 만들라는 명세

---

## 🧹 3. "Out-of-Source Build" 철학
가장 중요한 포인트입니다! 소스 코드가 있는 곳에 빌드 결과물(목적 파일, 실행 파일)이 뒤섞이면 관리가 매우 힘들어집니다. 

- **In-Source Build (지양)**: 소스 폴더(`src/`) 내부에 `.obj`, `.exe` 등이 생성됨.
- **Out-of-Source Build (지향)**: 별도의 `build/` 폴더를 만들어 모든 찌꺼기와 결과물을 그 안에 격리시킵니다. 

---

## 📋 4. CMakeLists.txt 기본 템플릿 (단일 프로젝트)
순수 C++ 소스로만 구성된 프로젝트를 시작할 때의 최소 템플릿입니다.

```cmake
# 1. 최소 CMake 버전 명시
cmake_minimum_required(VERSION 3.10)

# 2. 프로젝트 이름 및 언어 설정
project(MySimpleProject)

# 3. C++ 표준 설정 (C++23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. 실행 파일 생성 (Target 명시)
add_executable(App main.cpp Registry.cpp)
```

---

## ⚙️ 5. 필수 명령어 (PowerShell 기준)

1. **구성 (설계도 생성)**
   ```powershell
   cmake -S . -B build
   ```
2. **빌드 (실제 실행 파일 생성)**
   ```powershell
   cmake --build build
   ```

---

## 🙋 6. 자주 묻는 질문 (Q&A)

### Q1. `find_package()`는 왜 여기 없나요?
- **답변**: `find_package`는 Vcpkg 등을 통해 설치한 **외부 라이브러리**를 가져올 때 사용합니다. 라이브러리 연동에 대한 자세한 내용은 `concept_vcpkg_basics.md` 문서를 참고하세요!

### Q2. 왜 `target_link_libraries`가 필요한가요?
- **답변**: 여러 소스 파일이 서로를 참조하거나, 외부 라이브러리 기능을 끌어다 쓸 때 링커에게 "이 타겟은 저 녀석을 필요로 해!"라고 알려주는 필수 과정입니다.

---

### 🤔 궁금한 점
더 궁금한 점이 있다면 언제든 알려주세요!
