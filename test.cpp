#include <iostream>
#include <cstdio>
#include <vector>
int i = 0;

using namespace std;

vector<int> v;



void lol(std::string idontkare) {
    cout << i;
    i++;
    return;
}

int main() {
	std::string whatareyoudoing;
	v.push_back(2);
	for (int j = 3; j < 5; j++) {
		cout << j;
	}
	for (auto i : v) {
		cout << i;
	}
    do {
    	lol(whatareyoudoing);
    } while (i < 3);

    while (i < 3) 
    	lol(whatareyoudoing);
    while (i < 3) 
    	while (true) {
    		lol(whatareyoudoing);
    	}
    return 0;
}
