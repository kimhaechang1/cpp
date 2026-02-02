#include<iostream>
#include<string>
#include<vector>
#include<sstream>

using namespace std;

int main() {
	int N;
	cin >> N;
	vector<vector<string>> map(N, vector<string>(2, ""));
	for (int i = 0; i < N; i++) {
		cin >> map[i][0] >> map[i][1];
	}
	stringstream stream;
	string impossible = "Impossible";
	string possible = "Possible";
	for (int i = 0; i < N; i++) {
		bool can = true;
		if (map[i][0].size() != map[i][1].size()) {
			can = false;
		}
		int isalpha[26] = { 0, };
		for (char c : map[i][0]) {
			isalpha[c - 'a']++;
		}
		for (char c : map[i][1]) {
			int idx = c - 'a';
			isalpha[idx]--;
		}
		for (int i = 0; i < 26; i++) {
			if (isalpha[i] < 0 || isalpha[i] > 0) {
				can = false;
				break;
			}
		}

		stream << (can ? possible : impossible) << endl;
	}
	cout << stream.str();
	
}