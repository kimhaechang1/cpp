#include<iostream>
#include<vector>

using namespace std;

int main() {
	int n, x;
	cin >> n;
	vector<int> arr(n, 0);
	for (int i = 0; i < n; i++) {
		cin >> arr[i];
	}
	cin >> x;
	bool visit[1000001] = { false };
	
	for (int i = 0; i < n; i++) {
		visit[arr[i]] = true;
	}
	
	int cnt = 0;
	for (int i = 0; i < n; i++) {
		if (x - arr[i] > 0 && x - arr[i] < 1000001 && visit[x - arr[i]]) {
			cnt++;
		}
	}
	cout << cnt / 2;
	
}