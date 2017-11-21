#include"PreprocessBase.h"
#include"Data.h"
#include<iostream>

using namespace std;

void PreprocessBase::process(const Data& txt, const Data& ptn, const int threshold, const char* id) {
	if (txt.size() < ptn.size()) {
		cout << "Reverse txt and ptn" << endl;
		return;
	}
	cout << "Preprocess" << id << " start" << endl;
	// Check the range
	get_range(txt, ptn, threshold);
	cout << "Block = " << mBlock << endl;
	int newrange = ptn.size();
	for (int i = 0; i < mBlock; ++i) {
		newrange += mRange[i * 2 + 1] - mRange[i * 2] + 1;
	}
	cout << "New length is " << 100 * (double)(newrange) / (double)(txt.size()) << "%" << endl;
	cout << "Preprocess" << id << " end" << endl;
}

int PreprocessBase::get(int i) const {
	if (i < mBlock * 2) {
		return mRange[i];
	}
	else {
		std::cerr << "Out of bounds\n";
		exit(1);
	}
	return -1;
}

int* PreprocessBase::getAll() const {
	return mRange;
}

int PreprocessBase::block() const {
	return mBlock;
}
