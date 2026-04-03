# Module 10: 패키지 매니저 (vcpkg) 기초

## 🛒 1. vcpkg란 무엇인가?
**vcpkg**는 Microsoft에서 만든 C++용 **오픈 소스 라이브러리 관리 도구**입니다. 

C++ 개발자들을 가장 괴롭히는 것 중 하나가 바로 "외부 라이브러리 설치"입니다. `{fmt}`, `nlohmann_json`, `OpenCV` 같은 유명한 라이브러리를 쓰려면 예전에는 소스를 다운로드받아 직접 빌드하고, 환경 변수를 잡고, 프로젝트 설정에서 인클루드 경로를 하나하나 입력해야 했습니다. 

vcpkg는 이 모든 과정을 **명령어 한 줄**로 해결해 줍니다. 

### 🌟 주요 특징
- **의존성 자동 해결**: 라이브러리 A가 라이브러리 B를 필요로 하면, vcpkg가 알아서 B까지 설치해 줍니다.
- **CMake 친화적**: CMake와 완벽하게 연동되어, 설치한 라이브러리를 바로 프로젝트에 가져올 수 있습니다.
- **격리된 빌드**: 시스템 전역이 아닌, 특정 폴더나 프로젝트 단위로 라이브러리를 관리할 수 있습니다.

## 🏗️ 2. vcpkg 설치 방법 (Installation)
vcpkg는 별도의 설치파일(`.exe`)이 아닌, **Git 리포지토리를 복제하여 직접 빌드**하는 독특한 방식을 사용합니다.

### 📜 설치 순서 (Windows 기준)
1. **Repository 복제**: 작업 공간의 상위 폴더(예: `C:\cpp\`)에서 Git을 통해 가져옵니다.
   ```powershell
   git clone https://github.com/microsoft/vcpkg.git
   ```
2. **부트스트랩(Bootstrap) 실행**: vcpkg 도구를 직접 빌드하여 실행 파일(`vcpkg.exe`)을 생성합니다.
   ```powershell
   cd vcpkg
   ./bootstrap-vcpkg.bat
   ```
3. **통합 설치 (선택 사항)**: Visual Studio 등에서 전역적으로 사용하고 싶을 때 실행합니다.
   ```powershell
   ./vcpkg.exe integrate install
   ```

---

## 🛠️ 3. vcpkg 기본 사용법

### 🔍 라이브러리 검색
내가 원하는 라이브러리가 있는지 확인합니다.
```powershell
./vcpkg.exe search fmt
```

### 📥 라이브러리 설치
가장 기본이 되는 설치 명령어입니다.
```powershell
./vcpkg.exe install fmt
```
- **Triplet(트리플렛)**: 특정 환경에 맞춤 빌드하고 싶을 때는 `:삼중항`을 붙입니 다.
  - 예: `./vcpkg.exe install fmt:x64-mingw-static` (우리 환경의 기본!)

### 📜 설치된 목록 확인
```powershell
./vcpkg.exe list
```

---

## 🔗 4. CMake와 vcpkg 연동하기

vcpkg로 설치한 라이브러리를 CMake 프로젝트에서 사용하려면 구성(Configure) 시점에 툴체인 파일을 지정해야 합니다.

### 📋 CMakeLists.txt 연동 템플릿
여러 개의 외부 라이브러리를 한꺼번에 가져와 프로젝트에 연결하는 표준 방식입니다.

```cmake
# 1. 외부 패키지 찾기 (config 모드로 필수 검색)
# find_package([패키지이름] CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# 2. 실행객체(Target) 생성
add_executable(App main.cpp)

# 3. 라이브러리 연결 (Imported Target 사용)
# target_link_libraries([타겟이름] PRIVATE [라이브러리::타겟])
target_link_libraries(App PRIVATE 
    fmt::fmt 
    nlohmann_json::nlohmann_json
)
```

---

## 🔍 5. 패키지 이름과 CMake 타겟 찾는 법
설치는 `vcpkg install`로 했는데, `find_package`에서 쓸 **정확한 이름**과 `target_link_libraries`의 **`::` 타겟명**을 모를 때 확인하는 방법입니다.

### 방법 1: 설치 완료 메시지 확인 (가장 확실함)
라이브러리 설치가 끝나면 터미널에 친절하게 안내 메시지가 뜹니다.
> "The package fmt provides CMake targets: **fmt::fmt**"  
> "find_package(**fmt** CONFIG REQUIRED)"

### 방법 2: `share/` 폴더 직접 확인
설치된 경로(`vcpkg/installed/[triplet]/share/`)를 보면 라이브러리 폴더들이 있습니다.
- **폴더 이름**: 거의 항상 `find_package()`에서 찾는 이름과 일치합니다.
- **내부의 `*Config.cmake` 파일**: 이 파일을 열어보면 `add_library(... INTERFACE IMPORTED)` 부분 등에 명시된 정식 타겟(`::`) 이름을 찾을 수 있습니다.

---

## ⚙️ 6. 빌드 시 툴체인 지정
Vs Code 외부의 터미널에서 직접 빌드할 때는 vcpkg의 스크립트 파일 위치를 수동으로 알려주어야 합니다.
```powershell
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="C:/cpp/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

## 🏗️ 7. Antigravity 플랫폼 연동 (VS Code 설정)
우리의 학습 환경(Antigravity)에서는 매번 긴 명령어를 칠 필요 없이 가세화된 Tasks 기능을 사용합니다.

### 📁 1) .vscode/tasks.json (빌드 자동화)
구성(Configure) 태스크에 vcpkg 경로를 미리 주입해 두었습니다.

### 📁 2) .vscode/c_cpp_properties.json (IntelliSense)
코드에 빨간 줄이 뜨지 않게 하려면 에디터가 vcpkg의 헤더 파일을 읽어야 합니다. 

---

## 🤔 궁금한 점
vcpkg 명령어나 CMake 연동 과정에서 이해가 안 가는 부분이 있다면 언제든 질문해 주세요! 질문이 없으시다면 다음 단계인 패키지 연동 실습으로 넘어가겠습니다.
