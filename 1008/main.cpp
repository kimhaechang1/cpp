// 백준 1008번: A/B
// STEP 0 - C++ 기초 문법
// https://www.acmicpc.net/problem/1008

#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    // TODO: 두 정수 A와 B를 입력받아 A/B를 출력하세요
    // 주의: 소수점 출력이 필요할 수 있습니다!
    cout << setprecision(16);
    double a, b;
    cin >> a >> b;
    double c = a / b;
    cout << c;
    return 0;
}
