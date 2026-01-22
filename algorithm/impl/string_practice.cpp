#include <iostream>
#include <string>

// [Gemini Agent]
// std::string API 기초 연습용 파일입니다.
// 문자열을 다루는 기본적인 방법을 익혀봅시다.

using namespace std;

int main() {
    cout << "=== String API Practice ===" << endl;

    // TODO 1: "Hello" 라는 값을 가진 문자열 변수 's1'을 선언하세요.
    string s1 = "Hello";

    // TODO 2: ", World!" 라는 값을 가진 문자열 변수 's2'를 선언하세요.
    string s2 = ", World!";

    // TODO 3: s1과 s2를 더해서 새로운 문자열 'result'를 만드세요. (문자열 연결)
    // 힌트: + 연산자 사용
    string result = s1 + s2;

    // TODO 4: 'result'를 출력하세요. (예상 출력: Hello, World!)
    cout << result << endl;

    // TODO 5: 'result' 문자열의 길이(length 혹은 size)를 출력하세요.
    cout << result.length() << endl;

    // TODO 6: 'result'가 비어있는지 확인하여 출력하세요. (empty 사용)
    cout << result.empty() << endl;

    // TODO 7: 'result'의 첫 글자('H')와 마지막 글자('!')를 각각 출력하세요.
    // 힌트: 인덱스 [] 사용. 마지막 글자 인덱스는 size() - 1 입니다. 혹은 back()
    // 사용.
    cout << result[0] << " " << result[result.size() - 1] << endl;

    // TODO 8: 'result'의 내용을 "C++ Coding"으로 완전히 변경하세요. (대입 연산자
    // = 사용)

    result = "C++ Coding";

    // TODO 9: 범위 기반 for문을 사용하여 'result'의 각 문자를 한 줄에 하나씩
    // 출력하세요.
    /*
       출력 예시:
       C
       +
       +
       ...
    */
    for (auto& a : result) {
        cout << a << endl;
    }

    return 0;
}
