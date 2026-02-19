#include <iostream>
#include <stack>
#include <vector>

// -------------------------------------------------------------
// [Algorithm 0x05] 2493: 탑
// https://www.acmicpc.net/problem/2493
//
// [문제 설명]
// 일직선 위에 N개의 높이가 서로 다른 탑을 수평 직선의 왼쪽부터 오른쪽 방향으로 차례로 세웠다.
// 각 탑의 꼭대기에 레이저 송신기를 설치하였다.
// 모든 탑의 레이저 송신기는 레이저 신호를 지표면과 평행하게 수평 직선의 왼쪽 방향으로 발사하고,
// 탑의 기둥 모두에는 레이저 신호를 수신하는 장치가 설치되어 있다.
// 하나의 탑에서 발사된 레이저 신호는 가장 먼저 만나는 단 하나의 탑에서만 수신이 가능하다.
//
// (예: 높이가 6, 9, 5, 7, 4인 다섯 개의 탑이 일렬로 서 있다고 하자.
//  5번째 탑(4)의 신호 -> 4번째 탑(7) 수신
//  4번째 탑(7)의 신호 -> 2번째 탑(9) 수신
//  3번째 탑(5)의 신호 -> 2번째 탑(9) 수신
//  2번째 탑(9)의 신호 -> 수신 없음 (0)
//  1번째 탑(6)의 신호 -> 수신 없음 (0)
//
// [입력]
// 첫째 줄에 탑의 수를 나타내는 정수 N이 주어진다. N은 1 이상 500,000 이하이다.
// 둘째 줄에는 N개의 탑들의 높이가 직선상에 놓인 순서대로 하나의 빈칸을 사이에 두고 주어진다.
// 탑들의 높이는 1 이상 100,000,000 이하의 정수이다.
//
// [출력]
// 첫째 줄에 주어진 탑들의 순서대로 각각의 탑에서 발사한 레이저 신호를 수신한 탑들의 번호를
// 하나의 빈칸을 사이에 두고 출력한다. 만약 레이저 신호를 수신하는 탑이 존재하지 않으면 0을 출력한다.
// -------------------------------------------------------------

using namespace std;
vector<int> input;
int N;

int main() {
    // 1. 입출력 속도 향상 (백준 필수)
    ios::sync_with_stdio(0);
    cin.tie(0);

    // 2. 입력 받기
    cin >> N;
    for (int i = 0; i < N; i++) {
        int t;
        cin >> t;
        input.push_back(t);
    }
    // 항상 arr[0] 의 타워는 높이에 상관없이 무조건 0이다.
    // 높이가 같은 탑은 없다.
    // 자신의 바로앞이 어떤지에 따라 달라진다.
    // 특정 idx 번째의 높이가 K 라고 두었을 때, 그것보다 index-N 번째의 역순의 숫자들중에서
    // 처음으로 자신보다 큰 숫자가 나왔을 때 그때 수신하게 된다.
    // 해당 큰 숫자의 idx 가 곧 수신받는 것이 된다.
    // 예를들어 5 3 4 이런식이라고 가정해보자.
    // 그러면 정답은 0 1 1 이 된다.
    // 4부터 시작해서 자신보다 앞전에 더 큰 높이가 있는지 찾게된다. 4는 3번째 탑이다.
    // 앞의 타워인 3은 높이가 낮아서 수신하지 못한다. 3은 2번째 탑이다.
    // 앞의 타워인 5는 3과 4를 모두 수신할 수 있기 때문에 5의 등장으로 앞전에 나왔었던 타워들의 수신타워가 결정된다.
    stack<int> indexStack;
    stack<int> waitStack;
    vector<int> result(N, 0);
    for (int i = 0; i < N; i++) indexStack.push(i);
    int top = indexStack.top();
    int topValue = input[top];
    indexStack.pop();
    waitStack.push(top);
    while (!indexStack.empty()) {
        //cout << "size=" << indexStack.size() << endl;
        int idx = indexStack.top();
        int val = input[idx];
        //cout << "현재 선택된 idx=" << idx << ", val=" << val << endl;
        while (!waitStack.empty()) {
            int wTop = waitStack.top();
            //cout << "wait 에 대기중인 값 idx=" << wTop << ", val=" << input[wTop] << endl;
            if (input[wTop] > val) {
                break;
            }
            //cout << "결정: idx=" << wTop << ", 순번=" << idx + 1 << endl;
            result[wTop] = idx + 1;
            waitStack.pop();
        }
        waitStack.push(idx);
        indexStack.pop();
    }
    for (int i = 0; i < result.size(); i++) {
        cout << result[i] << " ";
    }

    return 0;
}
