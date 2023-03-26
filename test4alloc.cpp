#include "xin_alloc.h"
#include<vector>
#include<iostream>
using namespace std;

int main() {
	int ia[5] = { 0,1,2,3,4 };
	unsigned int i;

	vector<int, XinSTL::xin_alloc> iv(ia, ia + 5);
	for (int i = 0; i < iv.size(); i++) {
		cout << iv[i] << ' ';
	}
	cout << endl;
}