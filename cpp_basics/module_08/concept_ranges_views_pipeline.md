# Module 8 보충: C++20 Ranges와 Views 파이프라인

## 📌 파이프라인(Pipeline) 스타일의 데이터 필터링

C++20에 도입된 `std::views` 계열의 함수들은 데이터 컬렉션을 함수형(Functional) 스타일로 가공하는 강력한 도구들입니다. 이것들의 각각의 역할을 전용 예시로 분석해 봅시다.

가령, 우리가 `std::map<int, std::unique_ptr<Monster>>` 라는 몬스터 관리 맵을 가지고 있다고 가정해 봅시다.

```cpp
auto GetBossMonsters(const std::map<int, std::unique_ptr<Monster>>& map) {
    return map 
         | std::views::values 
         | std::views::transform([](const auto& ptr) { return ptr.get(); }) 
         | std::views::filter([](const Monster* m) { return m->IsBoss(); }) 
         | std::ranges::to<std::vector<Monster*>>();
}
```

### 1. `std::views::values`
*   **역할**: `std::map<Key, Value>` 구조에서 Key(몬스터 ID)를 무시하고 뜯어내어 Value(몬스터 객체)만 남기는 나이프(View)입니다.
*   **원리**: 해시맵의 순회 흐름 속에서 불필요한 데이터를 가지치기합니다.

### 2. `std::views::transform`
*   **역할**: 들어온 각각의 데이터를 특정한 조작(Lambda)을 거쳐 완전히 다른 형태로 **변신(Transform)** 시킵니다.
*   **원리**: 데이터 소유권 래퍼인 `std::unique_ptr<Monster>`를 통째로 전달하면 소유권 구조가 꼬이므로, `.get()` 함수만 호출하여 **투명하게 들여다볼 수 있는 Raw Pointer**(`Monster*`)로 즉석에서 변신시킵니다.

### 3. `std::views::filter`
*   **역할**: 데이터들의 흐름 속에서, 람다가 지정한 조건(boolean)을 통과한(True) 녀석들만 살려둡니다.
*   **원리**: 무수한 몬스터 오브젝트들 중에서 `IsBoss() == true` 인 **보스 몬스터만** 남깁니다.

### 4. `std::ranges::to<컨테이너 타입>()` (C++23)
*   **역할**: 지금까지 파이프라인(`|`)을 타고 나불나불 떠들어왔던 **지연된 계산식(View)**들을, 종착역인 실제 메모리(컨테이너)로 꽝꽝 얼려 담아냅니다.
*   **원리**: 공중에 떠 있던 필터링 결과물(View)들을 최종적으로 `std::vector<Monster*>`라는 구체화된 동적 배열에 쓸어 담아 밖으로 반환합니다.

---

> **💡 핵심 철학: Lazy Evaluation (지연 평가)**
> 파이프라인 중간에 위치한 `views`들은 당장 for문을 돌며 연산을 수행하지 않고 계속 **나중으로 미룹니다**. 데이터 전체를 백만 번 훑고 변신시키고 필터링을 반복하는 게 아니라, 맨 마지막에 `ranges::to<>` 결합 로봇이나 `for`문을 만날 때 비로소 **최소한의 횟수(단 한 번의 순회)**만으로 압축하여 데이터를 한 큐에 처리합니다. 이것이 C++20 파이프라인의 엄청난 위력입니다!
