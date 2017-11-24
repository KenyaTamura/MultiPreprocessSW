#include"PreprocessBase.h"
#include"Data.h"
#include<iostream>

using namespace std;

void PreprocessBase::start(const Data& db, const Data& query, const int threshold, const char* id) {
	if (db.size() < query.size()) {
		cout << "Reverse db and query" << endl;
		return;
	}
	cout << "Preprocess" << id << " start" << endl;
	// Check the range
	process(db, query, threshold);
	cout << "Block = " << mBlock << endl;
	int newrange = 0;
	for (int i = 0; i < mBlock; ++i) {
		newrange += mRange[i * 2 + 1] - mRange[i * 2] + 1 + query.size();
	}
	cout << "New length is " << 100 * (double)(newrange) / (double)(db.size()) << "%" << endl;
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
