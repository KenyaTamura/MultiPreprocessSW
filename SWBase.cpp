#include"SWBase.h"

SWBase::SWBase(int threshold) :
	mScore{ nullptr },
	mThreshold{ threshold },
	mSize{ 0 },
	mMaxScore{ -1 },
	mMaxPos{ -1 } {}

int SWBase::max_score() {
	if (mMaxScore == -1) {
		search_max();
	}
	return mMaxScore;
}

int SWBase::max_position() {
	if (mMaxScore == -1) {
		search_max();
	}
	return mMaxPos;
}

int* SWBase::all_score() {
	if (!mScore) {
		return mScore;
	}
	else {
		return nullptr;
	}
}

void SWBase::search_max() {
	for (int i = 0; i < mSize; ++i) {
		if (mScore[i] >= mThreshold && mScore[i] > mMaxScore) {
			mMaxScore = mScore[i];
			mMaxPos = i;
		}
	}
	if (mMaxScore == -1) {
		mMaxScore = 0;
		mMaxPos = 0;
	}
}

