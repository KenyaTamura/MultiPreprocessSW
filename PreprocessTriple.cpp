#include"PreprocessTriple.h"
#include"Data.h"

#include<iostream>
#include<algorithm>

#include"Writer.h"

using namespace std;

namespace {
	constexpr int Triple = 2;
	constexpr int Type = 64;
	auto convert = [](char acid) {
		if (acid == 'A') return 0;
		else if (acid == 'C') return 1;
		else if (acid == 'G') return 2;
		else if (acid == 'T') return 3;
		else return 0;
	};
}

PreprocessTriple::PreprocessTriple(const Data& txt, const Data& ptn, const int threshold) {
	process(txt, ptn, threshold, "Triple");
}

PreprocessTriple::~PreprocessTriple() {
	if (mRange) {
		delete[] mRange;
	}
}

void PreprocessTriple::check_score(const Data& txt, const Data& ptn, int* range) {
	// Get hash, the length is ptn size
	int hashT[Type]{ 0 };
	int hashP[Type]{ 0 };
	get_hash(txt, ptn.size(), hashT);
	get_hash(ptn, ptn.size(), hashP);
	int size = txt.size() - ptn.size();
	int psize = ptn.size();
	auto sum = [&](int i) {
		return (convert(txt[i]) << 4) + (convert(txt[i + 1]) << 2) + convert(txt[i + 2]);
	};
	for (int i = 0; i < size; ++i) {
		range[i] = get_score(hashT, hashP);
		// Minus i and plus i + ptn.size()
		--hashT[sum(i)];
		++hashT[sum(i + psize - Triple)];
	}
}

void PreprocessTriple::get_range(const Data& txt, const Data& ptn, const int threshold) {
	// Buffer
	int* buffer = new int[txt.size() / ptn.size()];
	// Get hash, the length is ptn size
	int hashT[Type]{ 0 };
	int hashP[Type]{ 0 };
	get_hash(txt, ptn.size(), hashT);
	get_hash(ptn, ptn.size(), hashP);
	int size = txt.size() - ptn.size();
	int psize = ptn.size();
	int block = 0;
	int score = get_score(hashT, hashP);
	for (int i = 0; i < size; ++i) {
		if (score >= threshold) {
			if (block % 2 == 0) {
				buffer[block++] = i;
				buffer[block] = i + 1;
			}
			else {
				buffer[block] = i;
			}
		}
		else {
			if (block % 2 != 0 && i - buffer[block] > psize) {
				++block;
			}
		}
		// Minus hash i~i+1 and plus hash i + ptn.size()-1 ~ i+ptn.size()
		int dec = (convert(txt[i]) << 4) + (convert(txt[i + 1]) << 2) + convert(txt[i + 2]);
		int offset = i + psize;
		int inc = (convert(txt[offset - 2]) << 4) + (convert(txt[offset - 1]) << 2) + convert(txt[offset]);
		if (hashT[dec] <= hashP[dec]) {
			--score;
		}
		--hashT[dec];
		if (hashT[inc] < hashP[inc]) {
			++score;
		}
		++hashT[inc];
	}
	if (block % 2 == 1) {
		++block;
	}
	mBlock = block / 2;
	mRange = new int[block];
	for (int i = 0; i < block; ++i) {
		mRange[i] = buffer[i];
	}
	delete[] buffer;
}

int PreprocessTriple::get_score(const int* hash1, const int* hash2) const {
	int score = 0;
	for (int i = 0; i < Type; ++i) {
		score += min(hash1[i], hash2[i]);
	}
	if (score == 0) { return 0; }
	else { return score + Triple; }
}

void PreprocessTriple::get_hash(const Data& data, int size, int* hash) const {
	for (int i = 0; i < size - Triple; ++i) {
		++hash[(convert(data[i]) << 4) + (convert(data[i + 1]) << 2) + convert(data[i + 2])];
	}
	/*for (int i = 0; i < Type; ++i) {
	cout << static_cast<int>(hash[i]) << endl;
	}*/
}
