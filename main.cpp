#include<iostream>
#include<string>
#include<string.h>

#include"Data.h"
#include"PreprocessSingle.h"
#include"PreprocessDouble.h"
#include"PreprocessTriple.h"
#include"PreprocessQuad.h"
#include"PreprocessBase.h"
#include"SimpleSW.h"
#include"PreprocessSW.h"
#include"PreprocessSWGPU.h"
#include"SWBase.h"
#include"FileConverter.h"
#include"Timer.h"
#include"Writer.h"

using namespace std;

namespace {
	Data* db = nullptr;
	Data* q = nullptr;
	int threshold = 0;
	int thread = 8;
	int gpu_block = 8;
	string ofname;
	string type = "quad";
	bool cmp_flag = false;
	bool gpu_flag = true;
}

void arg_branch(int argc, char* argv[]) {
	auto cmp = [&](int i, const char* str) -> bool {return (strcmp(argv[i], str) == 0); };
	for (int i = 1; i < argc; ++i) {
		if (cmp(i,"-convert")) {
			if (i + 2 < argc) { FileConverter f(argv[i+1], argv[i+2]); }
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
		if (cmp(i, "-type")) {
			if (++i < argc) { type = argv[i]; }
		}
		if (cmp(i, "-cmp")) {
			cmp_flag = true;
		}
		if (cmp(i, "-cpu")){
			gpu_flag = false;
		}
		if(cmp(i, "-time")){
			if(++i < argc) { ofname = argv[i]; }
		}
		if(cmp(i, "-thread")){
			if(++i < argc && atoi(argv[i]) > 0) { thread = atoi(argv[i]); }
		}
	}
}

void mode_select(){
	auto type_check = [&](const char* str) -> bool {return (strcmp(type.c_str(), str) == 0); };
	if (db != nullptr && q != nullptr && db->data() != nullptr && q->data() != nullptr) {
		// Compare execution time
		if (cmp_flag) {
			Timer t;
			PreprocessSWGPU(*db, *q, PreprocessSingle(*db, *q, threshold, thread), threshold);
			cout << t.get_millsec() << endl;
			t.start();
			PreprocessSWGPU(*db, *q, PreprocessDouble(*db, *q, threshold, thread), threshold);
			cout << t.get_millsec() << endl;
			t.start();
			PreprocessSWGPU(*db, *q, PreprocessTriple(*db, *q, threshold, thread), threshold);
			cout << t.get_millsec() << endl;
			t.start();
			PreprocessSWGPU(*db, *q, PreprocessQuad(*db, *q, threshold, thread), threshold);
			cout << t.get_millsec() << endl;
			t.start();
			SimpleSW(*db, *q, threshold, gpu_block);
			cout << t.get_millsec() << endl;
		}
		// All of result at preprocess
		else {
			if (type_check("simple")) { 
				SimpleSW sw(*db, *q, threshold, gpu_block);
				return;
			}
			PreprocessBase* pre;
			if(type_check("single")){
				pre = new PreprocessSingle(*db, *q, threshold, thread);
			}
			else if(type_check("double")){
				pre = new PreprocessDouble(*db, *q, threshold, thread);
			}
			else if(type_check("triple")){
				pre = new PreprocessTriple(*db, *q, threshold, thread);
			}
			else{	
				pre = new PreprocessQuad(*db, *q, threshold, thread);
			}
			if(gpu_flag){
				PreprocessSWGPU(*db, *q, *pre, threshold);
			}
			else {
				PreprocessSW(*db, *q, *pre, threshold);
			}
			delete pre;
		}
	}
	else{
		cout << "File error" << endl;	
	}
}

int main(int argc, char* argv[]) {
	arg_branch(argc, argv);
	Timer t;
	mode_select();
	cout << t.get_millsec() << endl;
	if(!ofname.empty()){
		Writer w;
		w.writing_time(ofname.c_str(), t.get_millsec());
	}
	if (db) {
		delete db;
	}
	if (q) {
		delete q;
	}
}
