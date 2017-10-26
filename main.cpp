#include<iostream>
#include<string>
#include<string.h>

#include"Data.h"
#include"PreprocessSingle.h"
#include"PreprocessQuad.h"
#include"PreprocessBase.h"
#include"SimpleSW.h"
#include"PreprocessSW.h"
#include"SWBase.h"
#include"FileConverter.h"
#include"Timer.h"
#include"Writer.h"

using namespace std;

namespace {
	Data* db = nullptr;
	Data* q = nullptr;
	int threshold = 0;
	string ofname;
	string ofname_pre;
	string type = "quad";
	bool cmp_flag = false;
}

void arg_branch(int argc, char* argv[]) {
	auto cmp = [&](int i, const char* str) -> bool {return (strcmp(argv[i], str) == 0); };
	for (int i = 1; i < argc; ++i) {
		if (cmp(i,"-convert")) {
			if (i + 2 < argc) { FileConverter f(argv[i+1],argv[i+2]); }
			i += 2;
		}
		if (cmp(i, "-db")) {
			if (++i < argc && !db) { db = new Data(argv[i]); }
		}
		if (cmp(i, "-q")) {
			if (++i < argc && !q) { q = new Data(argv[i]); }
		}
		if (cmp(i, "-t")) {
			if (++i < argc) { threshold = atoi(argv[i]); }
		}
		if (cmp(i, "-o")) {
			if (++i < argc) { ofname = argv[i]; }
		}
		if (cmp(i, "-predebug")) {
			if (++i < argc) { ofname_pre = argv[i]; }
		}
		if (cmp(i, "-type")) {
			if (++i < argc) { type = argv[i]; }
		}
		if (cmp(i, "-cmp")) {
			cmp_flag = true;	++i;
		}
	}
}

int main(int argc, char* argv[]) {
	arg_branch(argc, argv);
	auto type_check = [&](const char* str) -> bool {return (strcmp(type.c_str(), str) == 0); };
	Timer t;
	if (db != nullptr && q != nullptr) {
		// Compare execution time
		if (cmp_flag) {
			t.start();
			PreprocessSW(*db, *q, PreprocessSingle(*db, *q, threshold), threshold);
			cout << t.get_millsec() << endl;
			t.start();
			PreprocessSW(*db, *q, PreprocessQuad(*db, *q, threshold), threshold);
			cout << t.get_millsec() << endl;
			t.start();
			SimpleSW(*db, *q, threshold);
			cout << t.get_millsec() << endl;
		}
		else if (!ofname_pre.empty()) {
			if (type_check("single")) { PreprocessSingle(*db, *q, ofname_pre.c_str()); }
			else if (type_check("quad")) { PreprocessQuad(*db, *q, ofname_pre.c_str()); }
		}
		// All of result at preprocess
		else {
			if (type_check("simple")) { 
				SimpleSW sw(*db, *q, threshold);
				// All of result
				if (!ofname.empty()) {
					Writer w;
					w.writing_score(ofname.c_str(), sw.all_score(), db->size(), db->size() / 100);
				}
			}
			else {
				if (type_check("single")) {
					PreprocessSW(*db, *q, PreprocessSingle(*db, *q, threshold), threshold);
				}
				else {
					PreprocessSW(*db, *q, PreprocessQuad(*db, *q, threshold), threshold);
				}
			}
		}
	}
	cout << t.get_millsec() << endl;
	if (db) {
		delete db;
	}
	if (q) {
		delete q;
	}
}
