#include <iostream>
#include<queue>
using namespace std;

int N;
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    queue<int> q;
    cin >> N;
    for (int i = 1; i <= N; i++) {
        q.push(i);
    }
    while (q.size() > 1) {
        q.pop();
        int f = q.front();
        q.pop();
        q.push(f);
    }
    cout << q.front();
    return 0;
}
