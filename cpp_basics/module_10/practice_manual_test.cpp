#include "Calculator.h"
#include <cassert>
#include <iostream>

/**
 * [MODULE 10] 수동 함수 분할 테스트 실습
 *
 * 라이브러리 없이 C++ 표준 <cassert>와 함수 분할만으로
 * 기능을 검증하는 "원시적" 방식을 체험합니다.
 */

// 1. 더하기 기능 테스트 함수
void Test_Add() {
  Calculator calc;
  // AAA 패턴: Arrange(준비) -> Act(실행) -> Assert(검증)
  assert(calc.Add(10, 20) == 30);
  assert(calc.Add(-5, 5) == 0);
  std::cout << "[SUCCESS] Test_Add passed!" << std::endl;
}

// 2. 빼기 기능 테스트 함수
void Test_Subtract() {
  Calculator calc;
  // 일부러 틀린 기댓값을 넣어보거나, Calculator.cpp를 수정해서 실패를
  // 유도해보세요.
  assert(calc.Subtract(20, 10) == 10);
  assert(calc.Subtract(5, 10) == -5);
  assert(calc.Subtract(1, 0) == 1);
  std::cout << "[SUCCESS] Test_Subtract passed!" << std::endl;
}

int main() {
  std::cout << "--- 수동 테스트 시스템 시작 ---" << std::endl;

  // TODO: 새로운 테스트 함수를 만들 때마다 여기에 수동으로 추가해야 합니다.
  // (이것이 "Manual Registration"의 번거로움입니다!)
  Test_Add();
  Test_Subtract();

  std::cout << "--- 모든 테스트 완료! (과연 여기까지 올 수 있을까요?) ---"
            << std::endl;

  return 0;
}
