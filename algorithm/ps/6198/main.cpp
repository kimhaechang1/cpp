#include<iostream>
#include<vector>
#include<stack>

using namespace std;

int N;
vector<int> hs;
void Debug(vector<int> result) {
	for (int i = 0; i < N; i++) {
		cout << "i: " << i << ", value: " << result[i] << endl;
	}
}

int main() {
	cin >> N;
	for (int i = 0; i < N; i++ ) {
		int x;
		cin >> x;
		hs.push_back(x);
	}
	long long result = 0;
	stack<int> b;
	vector<int> count(N, 0);
	for (int i = N - 1; i > -1; i--) {
		int cnt = 0;
		while (!b.empty() && hs[b.top()] < hs[i]) {
			cnt++;
			count[i] += count[b.top()];
			b.pop();
		}
		count[i] += cnt;
		b.push(i);
	}
	for (int val : count) {
		result += val;
	}
	cout << result;
	return 0;
}