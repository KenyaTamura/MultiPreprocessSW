#include"Writer.h"

#include<fstream>

using namespace std;

Writer::Writer() {
	
}

Writer::~Writer(){}

void Writer::writing_score(const char* fname, const int* data, int size) {
	ofstream ofs(fname, ios::trunc);
	if (!ofs) { return; }
	for (int i = 0; i < size; ++i) {
		ofs << data[i] << ',';
	}
}

void Writer::writing_score(const char* fname, const int* data, int size, int interval, int timer) {
	ofstream ofs(fname, ios::trunc);
	if (!ofs) { return; }
	int times = 0;
	for (int i = 0; i < size; i++) {
		ofs << data[i] << ',';
		if (i % interval == 0 && i != 0) {
			ofs << '\n';
			++times;
		}
		if (times == timer) { break; }
	}
}