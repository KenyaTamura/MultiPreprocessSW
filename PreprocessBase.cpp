#include"PreprocessBase.h"
#include<iostream>

using namespace std;

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
