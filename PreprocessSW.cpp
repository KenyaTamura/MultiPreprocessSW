#include"PreprocessSW.h"
#include"PreprocessBase.h"
#include"Data.h"
#include"Cost.h"

#include<algorithm>
#include<iostream>

using namespace std;

PreprocessSW::PreprocessSW(const Data& db, const Data& query, const PreprocessBase& pre, int threshold) : SWBase{ threshold } {
	cout << "PreprocessSW start" << endl;
	mScore = new int[db.size()]{ 0 };	// Be able to cut size
	mSize = db.size();	
	for (int i = 0; i < pre.block(); ++i) {
		DP(db, query, pre.get(i * 2), pre.get(i * 2 + 1) + query.size());
	}
	cout << "Max score is " << max_score() << ", max position is " << max_position() << endl;
	traceback(db, query);
	cout << "PreprocessSW end" << endl;
}

PreprocessSW::~PreprocessSW() {
	if (mScore) {
		delete[] mScore;
		mScore = nullptr;
	}
}

void PreprocessSW::DP(const Data& db, const Data& query, int start, int end) {
	int tsize = db.size();
	int psize = query.size();
	int* FScore = new int[psize] {0};
	int* MainScore = new int[psize + 1]{ 0 };
	int EScore = 0;
	int score = 0;
	int FBeg = 0;
	int FExt = 0;
	int EBeg = 0;
	int EExt = 0;
	int match = cost.Match;
	int miss = cost.Miss;
	int ext = cost.Extend;
	int beg = cost.Begin;
	for (int t = start; t < end; ++t) {
		int EScore = 0;
		int score = 0;
		int prevScore = 0;
		int FBeg = 0;
		int FExt = 0;
		int EBeg = 0;
		int EExt = 0;
		char db_char = db[t];
		for (int p = 0; p < psize; ++p) {
			// Match or miss match
			if (db_char == query[p]) {
				score = MainScore[p] + match;
			}
			else {
				score = MainScore[p] + miss;
			}
			// Gap begin
			FBeg = MainScore[p + 1] + beg;
			EBeg = prevScore + beg;
			// Gap extend
			FExt = FScore[p] + ext;
			EExt = EScore + ext;
			// set value
			MainScore[p] = prevScore;
			FScore[p] = max({ FBeg, FExt, 0 });
			EScore = max({ EBeg, EExt, 0 });
			prevScore = max({ FScore[p],EScore,score });
			if (mScore[t] < prevScore) {
				mScore[t] = prevScore;
			}
		}
	}
	delete[] FScore;
	delete[] MainScore;
}


