#include<iostream>
#include<vector>
#include<stack>
#include<string>
#include<sstream>

using namespace std;

int N;
const int MX = 10005;
int queue[MX];
int front = 0;
int back = 0;

int Empty() {
	return front == back ? 1 : 0;
}

void Push(int x) {
	//cout <<"index: " << back % MX << ", value: " << x << endl;
	queue[back % MX] = x;
	back++;
}
int Pop() {
	if (Empty() == 1) {
		return -1;
	}
	int rs = queue[front % MX];
	front++;
	return rs;
}

int Size() {
	return back - front;
}

int Front() {
	if (Empty() == 1) {
		return -1;
	}
	return queue[front % MX];
}

int Back() {
	if (Empty() == 1) {
		return -1;
	}
	return queue[(back - 1) % MX];
}

int main() {
	cin >> N;
	stringstream stream;
	for (int i = 0; i < N; i++) {
		string op;
		cin >> op;
		if (op == "push") {
			int x;
			cin >> x;
			Push(x);
		}
		else if (op == "pop") {
			stream << Pop() << endl;
		}
		else if (op == "size") {
			stream << Size() << endl;
		}
		else if (op == "empty") {
			stream << Empty() << endl;
		}
		else if (op == "front") {
			stream << Front() << endl;
		}
		else if (op == "back") {
			stream << Back() << endl;
		}
	}
	cout << stream.str();

}