#include"Data.h"
#include<fstream>
#include<iostream>
#include<random>

using namespace std;

Data::Data(const char* fname) : mData{ nullptr } {
	ifstream ifs(fname, ios::binary);
	if (ifs.fail()) {
		cerr << "Not found file" << endl;
		return;
	}
	else {
		cout << "Loading " << fname << endl;
	}
	ifs.seekg(0, ifstream::end);
	mSize = static_cast<int>(ifs.tellg());
	ifs.seekg(0, ifstream::beg);
	mData = new char[mSize];
	ifs.read(mData, mSize);
	if (mData[mSize - 1] == '\0' ||
		mData[mSize - 1] == '\n') { --mSize; }
}

Data::~Data() {
	if(mData){
		delete[] mData;
	}
}

char Data::operator[](int i) const {
	if (i < mSize) {
		return mData[i];
	}
	else {
		cerr << "Out of range" << endl;
		return -1;
	}
}

int Data::size() const {
	return mSize;
}

char* Data::data() const{
	return mData;
}
