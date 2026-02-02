#include<iostream>
#include<vector>

using namespace std;

int main() {
	int N, v;
	cin >> N;
	vector<int> arr(N, 0);
	for (int i = 0; i < N; i++) {
		cin >> arr[i];
	}
	cin >> v;
	int cnt[201] = { 0, };
	for (int n : arr) {
		cnt[n + 100]++;
	}
	cout << cnt[v + 100];
}