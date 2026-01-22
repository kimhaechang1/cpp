#include <iostream>
#include <string>
#include <vector>
using namespace std;

// 백준 10808: 알파벳 개수 (브론즈 4)
// https://www.acmicpc.net/problem/10808
// 
// 문제 설명:
// 단어 S가 주어졌을 때, 각 알파벳(a~z)이 단어에 몇 개 포함되어 있는지 구하는 프로그램을 작성하시오.
//
// 힌트:
// 1. 알파벳 소문자는 총 26개입니다.
// 2. 개수를 저장할 배열(또는 벡터)을 26칸 만들고 0으로 초기화하세요.
// 3. 문자 'a'는 아스키코드 97입니다. 문자열의 각 문자 c에 대해 c - 'a'를 하면 0~25 인덱스를 얻을 수 있습니다.

int main() {
    // 입출력 속도 향상
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    cin >> s;

    // 여기에 코드를 작성하세요
    int cnt[26] = { 0, };
    for (char ch : S) {
        cnt[ch - 'a']++;
    }
    for (int c : cnt) {
        cout << c << " ";
    }
    return 0;
}
