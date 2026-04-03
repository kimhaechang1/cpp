#include "Calculator.h"
#include <fmt/core.h> // vcpkg에서 가져온 라이브러리!
#include <iostream>

/**
 * [MODULE 10] Multi-file Project main
 *
 * 직접 만든 Calculator 클래스를 사용해봅시다.
 */

int main() {
  Calculator myCalc;

  int sum = myCalc.Add(50, 100);
  int cha = myCalc.Subtract(100, 50);

  // fmt 라이브러리를 사용한 화려한 출력!
  fmt::print("🚀 [Module 10] Vcpkg + CMake success!\n");
  fmt::print("SUM: 50 + 100 = {}\n", sum);
  fmt::print("SUB: 100 - 50 = {}\n", cha);

  return 0;
}