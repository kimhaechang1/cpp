# 🎮 DirectX 게임 개발 커리큘럼

## 📘 개요 (Overview)
DirectX API 암기가 아닌, **"픽셀의 원리"**부터 **"엔진 아키텍처"**까지 단계별로 정복하는 과정입니다.
기초 멘토(dx_mentor)와 심화 아키텍트(graphics_architect)가 단계별로 지도합니다.

---

## 🎨 Phase 1: Math & Pixels (The Foundation)
**담당 Agent**: ✨ **dx_mentor**
**목표**: 그래픽스 API 없이 C++과 수학만으로 화면을 그려본다. (Software Rendering)

- [ ] **Software Rasterizer Basics**:
    - Win32 API로 창 띄우기 & 픽셀 버퍼 제어.
    - 점(Point) -> 선(Line, Bresenham) -> 면(Triangle) 그리기.
- [ ] **3D Math from Scratch**:
    - `Vector3`, `Matrix4x4` 클래스 직접 구현하기.
    - 정점 변환(Vertex Transformation): Local -> World -> View -> Projection.
    - 카메라 시스템(LookAt) 수학적 구현.

---

## 🎨 Phase 2: Modern API Basics
**담당 Agent**: ✨ **dx_mentor**
**목표**: DirectX 11을 통해 현대적인 GPU 파이프라인의 구조를 익힌다.

- [ ] **Pipeline Setup**: Device, SwapChain, RenderTarget 개념 잡기.
- [ ] **Programmable Shader**: HLSL 기초 (Vertex Shader, Pixel Shader).
- [ ] **Buffers**: Vertex Buffer, Index Buffer로 육면체 그리기.
- [ ] **Texture & Sampler**: 텍스처 매핑과 UV 좌표 이해.

---

## 🏗️ Phase 3: High-Performance Graphics (2025 Trend)
**담당 Agent**: 🏗️ **graphics_architect**
**목표**: DirectX 12의 하드웨어 친화적 제어와 최신 렌더링 기술을 도입한다.

- [ ] **DirectX 12 Core**:
    - Command List/Queue/Allocator 구조 이해.
    - Descriptor Heap & Root Signature 설계.
    - Fence & Barrier를 이용한 CPU-GPU 동기화.
- [ ] **Optimization Techniques**:
    - **GPU Instancing**: 대량의 오브젝트 최적화 렌더링.
    - **Compute Shader**: GPGPU를 활용한 입자 시스템.
- [ ] **Next-Gen Rendering**:
    - **Ray Tracing (DXR)**: 그림자 및 반사 효과 구현.
    - Hybrid Rendering 파이프라인 구축.
