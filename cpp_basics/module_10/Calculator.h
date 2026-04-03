#pragma once

/**
 * [MODULE 10] 헤더 파일 기초
 *
 * 1. #pragma once: 이 파일이 여러 번 포함(Include)되어도 하나로 처리하게
 * 합니다. (중복 방지)
 * 2. 선언부: 함수의 이름, 매개변수, 반환 타입만 명시합니다.
 */

class Calculator {
public:
  // 두 정수를 더해 결과를 반환하는 기능의 '이름'만 선언합니다.
  int Add(int a, int b);

  // [미션] 빼기(Subtract) 기능의 선언도 여기에 추가해보세요!
  int Subtract(int a, int b);
};
