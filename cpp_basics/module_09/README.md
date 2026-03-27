# Module 09: 모던 C++ (RAII & 효율성)

## 📌 학습 목표

"Raw Pointer와 `delete`를 영원히 추방한다."

M5에서 배운 `new/delete` 기반 수동 메모리 관리를 졸업하고,
컴파일러가 메모리 해제를 자동으로 보장하는 **RAII** 패턴과
**이동 시맨틱(Move Semantics)** 으로 성능까지 챙기는 모던 C++을 익힙니다.

---

## 📚 소제목 목록

| 순서 | 소제목 | 개념 문서 | 실습 파일 | 상태 |
|------|--------|-----------|-----------|------|
| 1 | 스마트 포인터 (`unique_ptr`, `shared_ptr`, `weak_ptr`) | [concept](concept_smart_pointers.md) | [practice](practice_smart_pointers.cpp) | ⬜ |
| 2 | 이동 시맨틱 (`std::move`, R-value `&&`) | concept_move_semantics.md | practice_move_semantics.cpp | ⬜ |
| 3 | **[Cumulative Practice]** Smart Pointers & Move Semantics | - | practice_cumulative_9_1.cpp | ⬜ |
| 4 | Type Inference (`auto`, `decltype`) | concept_type_inference.md | practice_type_inference.cpp | ⬜ |
| 5 | Structured Binding (`auto [key, val]`) | concept_structured_binding.md | practice_structured_binding.cpp | ⬜ |
| 6 | **[Cumulative Practice]** Type Inference & Structured Binding | - | practice_cumulative_9_2.cpp | ⬜ |
| 7 | Modern Types (`optional`, `variant`, `expected`) | concept_modern_types.md | practice_modern_types.cpp | ⬜ |
| 8 | **[Cumulative Practice]** Modern Error Handling & Types | - | practice_cumulative_9_3.cpp | ⬜ |

---

## 🔑 핵심 키워드

- `std::unique_ptr`, `std::shared_ptr`, `std::weak_ptr`
- `std::make_unique`, `std::make_shared`
- `std::move`, R-value reference (`&&`)
- `auto`, `decltype`
- `auto [a, b]` (Structured Binding)
- `std::optional`, `std::variant`, `std::expected`

---

## ⬅️ 이전 / 다음 모듈

- **이전**: [Module 08 - 템플릿과 STL](../module_08/README.md)
- **다음**: Module 10 - 프로페셔널 도구 (CMake, ASan)
