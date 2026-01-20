#include <iostream>
#include <string>
#include <vector>

// [Gemini Agent]
// std::vector API 기초 연습용 파일입니다.
// 주석에 적힌 미션을 하나씩 해결하며 vector 사용법을 익혀봅시다.
using namespace std;

int main() {
  std::cout << "=== Vector API Practice ===" << std::endl;

  // TODO 1: int형 데이터를 담을 수 있는 빈 벡터 'numbers'를 선언하세요.
  vector<int> numbers;

  // TODO 2: 'numbers'에 10, 20, 30, 40, 50을 순서대로 추가하세요. (push_back
  // 사용)
  numbers.push_back(10);
  numbers.push_back(20);
  numbers.push_back(30);
  numbers.push_back(40);
  numbers.push_back(50);

  // TODO 3: 벡터의 현재 크기(size)를 출력하세요.
  cout << numbers.size() << endl;

  // TODO 4: 벡터의 세 번째 요소(인덱스 2)를 출력하세요. (예상 출력: 30)
  cout << numbers[2] << endl;

  // TODO 5: 벡터의 첫 번째 요소를 99로 변경하세요.
  numbers[0] = 99;

  // TODO 6: 범위 기반 for문(Range-based for loop)을 사용하여 모든 요소를
  // 공백으로 구분해 출력하세요. 출력 예시: 99 20 30 40 50
  for (auto &val : numbers) {
    cout << val << " ";
  }
  cout << endl;

  // TODO 7: 벡터의 마지막 요소를 제거하세요. (pop_back 사용)
  numbers.pop_back();

  // TODO 8: 반복자(iterator)를 사용하여 모든 요소를 출력하세요. (begin(), end()
  // 사용) 힌트: for (auto it = numbers.begin(); it != numbers.end(); ++it) {
  // ... }
  // 반복자를 사용하여 출력할때에는 포인터를 앞에 붙이면 된다.
  for (auto it = numbers.begin(); it != numbers.end(); ++it) {
    cout << *it << " ";
  }
  cout << endl;

  // TODO 9: 벡터의 모든 요소를 삭제하세요. (clear 사용)
  numbers.clear();

  // TODO 10: 벡터가 비어있는지 확인하고(empty 사용), 결과를 bool
  // 알파벳(true/false)으로 출력하세요. 힌트: std::boolalpha 사용
  cout << (numbers.empty() ? "true" : "false") << endl;

  return 0;
}
