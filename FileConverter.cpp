#include"FileConverter.h"

#include<fstream>
#include<iostream>

using namespace std;

FileConverter::FileConverter(const char* fname) : mData{} {
	read(fname);
	ofstream ofs(fname);
	ofs.write(mData.c_str(), mData.size());
}

FileConverter::FileConverter(const char* fname, const char* outfname) : mData{} {
	read(fname);
	ofstream ofs(outfname, ios::trunc);
	if (ofs.fail()) {
		cerr << "Not found file" << endl;
		return;
	}
	else {
		cout << "Writing " << outfname << endl;
	}
	ofs.write(mData.c_str(), mData.size());
}

FileConverter::~FileConverter() {

}

void FileConverter::read(const char* fname) {
	ifstream ifs(fname);
	if (ifs.fail()) {
		cerr << "Not found file" << endl;
		exit(0);
	}
	else {
		cout << "Loading " << fname << endl;
	}
	string buf;
	while (getline(ifs, buf)) {
		if (buf[0] == '>') {}	// File information
		else {
			mData += convert(buf);
		}
	}
}

string FileConverter::convert(string s) {
	if (s[0] > 'T') {
		for (int i = 0; i < static_cast<int>(s.size());++i) {
			s[i] -= 'a' - 'A';
		}
	}
	return s;
}