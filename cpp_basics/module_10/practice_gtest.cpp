#include "Calculator.h"
#include <gtest/gtest.h>

/**
 * [MODULE 10] Google Test (GTest) 실습
 *
 * 1. TEST(TestSuiteName, TestName) 매크로를 사용합니다.
 * 2. EXPECT_EQ(Actual, Expected): 틀려도 멈추지 않고 리포트를 남깁니다.
 * 3. 별도의 main() 함수가 필요 없습니다! (gtest_main 라이브러리가 제공)
 */

// 더하기 기능 테스트
TEST(CalculatorTestSuite, AddTest) {
  Calculator calc;
  EXPECT_EQ(calc.Add(10, 20), 30);
  EXPECT_EQ(calc.Add(-5, 5), 0);
}

// 빼기 기능 테스트
TEST(CalculatorTestSuite, SubtractTest) {
  Calculator calc;

  // 일부러 틀린 기댓값을 넣어보겠습니다.
  // 수동 테스트(assert)와는 달리, 여기서 실패해도 다음 테스트가 실행되는지
  // 확인하세요!
  EXPECT_EQ(calc.Subtract(20, 10), 10);
  EXPECT_EQ(calc.Subtract(1, 0), 1); // 👈 여기서 실패가 발생해도...
}

// 새로운 테스트를 추가할 때, main에 등록할 필요가 없습니다!
// 그냥 이렇게 아래에 추가하면 GTest가 스스로 찾아냅니다.
TEST(CalculatorTestSuite, BonusTest) {
  Calculator calc;
  EXPECT_EQ(calc.Add(100, 200), 300); // 👈 ...이 코드는 무사히 실행됩니다.
}
