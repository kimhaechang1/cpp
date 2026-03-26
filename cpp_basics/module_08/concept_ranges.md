# Module 08: Modern STL (Ranges & Views)

최신 C++에서 데이터를 다루는 방식은 더욱 직관적이고 강력해졌습니다. `std::ranges`는 컨테이너의 시작과 끝을 매번 적는 번거로움을 덜어주고, 기능을 마치 파이프라인처럼 연결할 수 있게 해줍니다.

---

## 1. Ranges란 무엇인가?
기존의 STL 알고리즘은 반복자(Iterator) 두 개(`begin`, `end`)를 쌍으로 묶어 전달해야 했습니다. **Range**는 이 반복자 쌍을 하나로 묶어 '범위' 그 자체를 객체로 다루는 개념입니다.

```cpp
// 기존 방식
std::sort(vec.begin(), vec.end());

// Ranges 방식 (C++20)
std::ranges::sort(vec); // 훨씬 간결합니다!
```

---

## 2. Views: 복사 없는 데이터 가공
**View**는 Range의 일종이지만, 데이터를 실제로 소유하거나 복사하지 않고 **'시야(Window)'**만 제공합니다. 
이를 활용하면 큰 데이터를 복사하지 않고도 필터링하거나 변환할 수 있습니다.

1.  **Lazy Evaluation (지연 평가)**: 실제로 데이터가 필요할 때까지 계산을 미룹니다.
2.  **Piping (`|`) 연산자**: 유닉스/쉘 명령어처럼 로직을 연결할 수 있습니다.

---

## 3. 파이프라인 구성 예시

사용자의 점수 목록에서 **짝수만 골라내어(filter)** 그 값을 **제곱(transform)**한 뒤 출력하는 예시입니다.

```cpp
#include <iostream>
#include <vector>
#include <ranges> // 핵심 헤더
#include <algorithm>

int main() {
    std::vector<int> scores = { 1, 2, 3, 4, 5, 6 };

    // 1. 짝수만 필터링 -> 2. 제곱으로 변환
    auto results = scores 
                 | std::views::filter([](int n) { return n % 2 == 0; })
                 | std::views::transform([](int n) { return n * n; });

    for (int v : results) {
        std::cout << v << " "; // 4 16 36 출력
    }
}
```

---

## 4. 왜 View를 쓰는가? (Game Dev 관점)

-   **성능**: `vector`에서 특정 조건의 몬스터만 골라내어 다른 `vector`에 복사하는 비용을 아낄 수 있습니다.
-   **가독성**: 중첩된 `for`문과 `if`문 대신, 데이터의 흐름(Flow)을 한눈에 파악할 수 있습니다.
-   **안전성**: 원본 데이터를 건드리지 않고 '보여지는 모습'만 바꾸기 때문에 부수 효과(Side Effect)가 적습니다.

---

## 5. 하드웨어 검증 (시각화)
View는 실제 데이터를 갖지 않고 포인터와 로직만 담고 있습니다.

```cpp
std::vector<int> v(1000, 7);
auto view = v | std::views::take(5);

std::cout << "Vector size: " << sizeof(v) << " bytes" << std::endl;
std::cout << "View size: " << sizeof(view) << " bytes" << std::endl; 
// 데이터 갯수와 상관없이 View의 크기는 매우 작고 일정합니다.
```

---

### [Q&A] 
내용을 읽으면서 **"이 부분은 게임 서버/엔진에서 어떻게 쓰일까요?"** 라거나 **"코드에서 이 기호(|)의 의미는 정확히 뭔가요?"** 같은 궁금증이 생기면 언제든 질문해 주세요! 질문이 해결되면 바로 실습으로 들어갑니다. 🙏✨
