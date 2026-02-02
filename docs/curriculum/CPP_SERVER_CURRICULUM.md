# ⚡ C++ 게임 서버 & 백엔드 커리큘럼

## 📘 개요 (Overview)
고성능 I/O 처리를 넘어, **대규모 분산 시스템(Distributed System)**과 **보안(Zero Trust)**을 갖춘 상용 레벨의 서버 아키텍처를 학습합니다.
2025년 트렌드인 **Observability(관측 가능성)**를 핵심 역량으로 합니다.

---

## 🏗️ LEVEL 1: High Performance I/O
**목표**: 단일 서버 성능의 극한을 추구한다.

### Module 1: Network Fundamentals
- [ ] **Socket Programming**: Winsock 기초, Blocking vs Non-Blocking.
- [ ] **Protocols**: TCP의 신뢰성 vs UDP의 속도 비교.
- [ ] **Packet Serialization**: Protobuf 또는 커스텀 바이너리 패킷 설계.

### Module 2: Asynchronous Server
- [ ] **IOCP (I/O Completion Port)**: 윈도우 고성능 비동기 모델 마스터.
- [ ] **Multi-threading Architecture**: Worker Thread Pool 관리.
- [ ] **Lock-free Basics**: Interlocked 연산과 Atomic 자료구조.

---

## 🌐 LEVEL 2: Modern Backend Architecture (2025 Trend)
**목표**: 확장 가능한 분산 서버와 클라우드 네이티브 환경을 구축한다.

### Module 3: Scalability & Database
- [ ] **Distributed Architecture**: Monolithic vs Microservices (MSA).
- [ ] **Database Strategy**: Redis(Cache) + RDB(Persistence) 하이브리드 패턴.
- [ ] **Reliable UDP**: **QUIC / KCP** 프로토콜 구현 (TCP의 한계 극복).

### Module 4: Observability & Security (필수)
- [ ] **Observability (관측 가능성)**:
    - **Prometheus**: 서버 메트릭(CPU, 패킷량) 수집.
    - **Jaeger / OpenTelemetry**: 분산 트랜잭션 추적 (Distributed Tracing).
- [ ] **Zero Trust Security**:
    - **mTLS**: 서버 간 상호 인증 및 암호화.
    - 패킷 검증 및 허용된 서비스 간 통신만 인가.
- [ ] **Cloud Native Deployment**: Docker & Kubernetes(Agones) 배포 기초.
