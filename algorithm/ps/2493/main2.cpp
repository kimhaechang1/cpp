#include <iostream>
#include <stack>
#include <vector>

// -------------------------------------------------------------
// [Algorithm 0x05] 2493: 탑 (Optimized Solution)
// -------------------------------------------------------------
// Time Complexity: O(N)
// - 각 탑은 스택에 최대 1번 들어가고(push), 최대 1번 나옵니다(pop).
// - 따라서 N개의 탑에 대해 2N번 연산 = O(N)입니다.
// -------------------------------------------------------------

using namespace std;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n;
    cin >> n;

    // stack에는 {height, index} 쌍을 저장합니다.
    stack<pair<int, int>> s;

    for (int i = 1; i <= n; i++) {
        int height;
        cin >> height;

        // Monotone Stack Logic (내림차순 유지)
        // 스택의 top이 '나(Current)'보다 작다면? -> 걔는 이제 가려져서 쓸모 없음 -> pop!
        while (!s.empty()) {
            if (s.top().first > height) { 
                // 나보다 큰 녀석 발견! -> 걔가 내 신호를 수신함
                cout << s.top().second << " "; 
                break; 
            }
            s.pop(); // 나보다 작은 녀석은 버림 (내가 가리니까)
        }

        // 스택이 비었다면 -> 내 신호를 받을 수 있는 더 큰 탑이 왼쪽에 없음
        if (s.empty()) {
            cout << "0 ";
        }

        // '나'를 스택에 등록 (오른쪽 탑들이 나를 볼 수도 있으니까)
        s.push({height, i}); 
    }

    return 0;
}
