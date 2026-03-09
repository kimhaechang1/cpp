# 0x07: 덱 (Deque) 문제집

| 문제 분류 | 문제 번호 | 문제 제목 |
| :--: | :--: | :--: |
| 기본 문제 | 10866 | [덱](https://www.acmicpc.net/problem/10866) |
| 기본 문제 | 1021 | [회전하는 큐](https://www.acmicpc.net/problem/1021) |
| 응용 문제 | 5430 | [AC](https://www.acmicpc.net/problem/5430) |

> 출처: [바킹독 실전 알고리즘 0x07](https://github.com/encrypted-def/basic-algo-lecture/blob/master/workbook/0x07.md)

---

## 💡 덱(Deque) 학습 포인트
1. **Double Ended Queue**: 덱은 큐와 스택의 결합형입니다. 맨 앞과 맨 뒤, **양쪽 양 끝**에서 데이터의 삽입과 삭제가 모두 허용되는 매우 유연한 선형 자료구조입니다.
2. **Data-Oriented 배열 구조 설계 (핵심!)**: 
   - 덱을 1차원 정적 배열(`Array`) 하나로 최적화하여 구현할 때는, `head`와 `tail` 인덱스의 시작점이 `0`이 되면 안 됩니다. (그럼 앞으로 뻗어나갈 공간이 없기 때문이죠!)
   - 따라서 `head = MX`, `tail = MX` 와 같이 **배열의 정중앙 지점**에서부터 양옆으로 뻗어나갈 수 있도록 넉넉한 2배 크기의 배열 공간을 잡아두는 것이 팁입니다.
3. STL `std::deque` 역시 내부적으로 매우 정밀하게 최적화되어 있지만, 데이터를 `chunk` 단위로 관리하기 때문에 완전한 연속형 메모리는 아닙니다. 따라서 정적 배열 덱이 `std::deque`보다 캐시 미스가 훨씬 적습니다.
