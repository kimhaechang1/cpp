#include<iostream>
#include<vector>

using namespace std;

int main() {
	int N, K, S, Y;
	cin >> N >> K;
	vector<vector<int>> map(N, vector<int>(2, 0));
	vector<vector<int>> cnt(7, vector<int>(2, 0));
	for (int i = 0; i < N; i++) {
		cin >> S >> Y;
		map[i][0] = S;
		map[i][1] = Y;
	}
	// 여학생 0, 남학생 1
	// 남학생은 남학생끼리
	// 여학생은 여학생끼리
	// 그리고 같은 학년이어야 함
	// 학년별로 남학생이 몇명인지 여학생이 몇명인지 파악하면 K 로 나누면 끝
	// 몫과 나머지 주의
	for (int i = 0; i < N; i++) {
		cnt[map[i][1]][map[i][0]]++;
	}
	int result = 0;
	for (int i = 1; i <= 6; i++) {
		result += (cnt[i][0] / K + (cnt[i][0] % K == 0 ? 0 : 1) + cnt[i][1] / K + (cnt[i][1] % K == 0 ? 0 : 1));
	}
	cout << result;
}