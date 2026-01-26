#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// 백준 1475: 방 번호 (실버 5)
// https://www.acmicpc.net/problem/1475
//
// 문제 설명:
// 다솜이는 은진이의 옆집에 새로 이사왔다. 다솜이는 자기 방 번호를 플라스틱 숫자로 문에 붙이려고 한다.
// 다솜이의 옆집에서는 플라스틱 숫자를 한 세트로 판다. 한 세트에는 0번부터 9번까지 숫자가 하나씩 들어있다.
// 다솜이의 방 번호가 주어졌을 때, 필요한 세트의 최솟값을 출력하시오.
// (6은 9를 뒤집어서 이용할 수 있고, 9는 6을 뒤집어서 이용할 수 있다.)

int main() {
    // 입출력 속도 향상
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    string N;
    cin >> N;
    int cnt[10] = { 0, };
    // 6과 9는 서로 호환이 된다 했으니까
    // 6과 9의 개수는 한 세트당 2개씩 획득 가능하다.
    // 즉, 6699 이런식으로 숫자가 주어지면 2세트로 해결이 된다는 것
    // 근데 6669 이런식으로 주어진 경우에도 마찬가지임
    for (char n : N) {
        cnt[n - '0']++;
    }
    int result = 0;
    for(int i= 0; i < 10; i++) {
        if (i == 6 or i == 9) continue;
        result = max(cnt[i], result);
    }
    result = max(result, (cnt[6] + cnt[9]) / 2 + ((cnt[6] + cnt[9]) % 2 ? 1 : 0));
    cout << result;
}