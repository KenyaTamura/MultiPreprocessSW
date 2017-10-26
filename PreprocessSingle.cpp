#include"PreprocessSingle.h"
#include"Data.h"

#include<algorithm>
#include<iostream>

#include"Writer.h"

using namespace std;

PreprocessSingle::PreprocessSingle(const Data& txt, const Data& ptn, const int threshold) {
	if (txt.size() < ptn.size()) {
		cout << "Reverse txt and ptn" << endl;
		return;
	}
	cout << "PreprocessSingle start" << endl;
	// Check the range
	get_range(txt, ptn, threshold);
	cout << "Block = " << mBlock << endl;
	int newrange = ptn.size();
	for (int i = 0; i < mBlock; ++i) {
		newrange += mRange[i * 2 + 1] - mRange[i * 2] + 1;
	}
	cout << "New length is " << 100 * (double)(newrange) / (double)(txt.size()) << "%" << endl;
	cout << "PreprocessSingle end" << endl;
}

PreprocessSingle::PreprocessSingle(const Data& txt, const Data& ptn, const char* fname) {
	if (txt.size() < ptn.size()) {
		cout << "Reverse txt and ptn" << endl;
		return;
	}
	cout << "PreprocessSingle start" << endl;
	// The search range of origin
	int* score = new int[txt.size() - ptn.size()];
	// Check the range
	check_score(txt, ptn, score);
	Writer w;
	w.writing_score(fname, score, txt.size() - ptn.size(), txt.size() / 100, 100);
	cout << "PreprocessSingle end" << endl;
}

PreprocessSingle::~PreprocessSingle() {
	if (mRange) {
		delete[] mRange;
	}
}

void PreprocessSingle::get_range(const Data& txt, const Data& ptn, const int threshold) {
	// Buffer
	int* buffer = new int[txt.size() / ptn.size()];
	// Get hash, the length is ptn size
	Hash hashT = get_hash(txt, ptn.size());
	Hash hashP = get_hash(ptn, ptn.size());
	int size = txt.size() - ptn.size();
	int psize = ptn.size();
	int block = 0;
	for (int i = 0; i < size; ++i) {
		if (get_score(hashT, hashP) >= threshold) {
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
		// Minus i and plus i + ptn.size()
		minus_hash(hashT, txt[i]);
		plus_hash(hashT, txt[i + psize]);
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

void PreprocessSingle::check_score(const Data& txt, const Data& ptn, int* score) {
	// Get hash, the length is ptn size
	Hash hashT = get_hash(txt, ptn.size());
	Hash hashP = get_hash(ptn, ptn.size());
	int size = txt.size() - ptn.size();
	int psize = ptn.size();
	int j = 0;
	for (int i = 0; i < size; ++i) {
		score[i] = get_score(hashT, hashP);
		// Minus i and plus i + ptn.size()
		minus_hash(hashT, txt[i]);
		plus_hash(hashT, txt[i + psize]);
	}
}

PreprocessSingle::Hash PreprocessSingle::get_hash(const Data& data, int size) const {
	Hash h;
	for (int i = 0; i < size; ++i) {
		plus_hash(h, data[i]);
	}
	return h;
}

int PreprocessSingle::get_score(const Hash& hash1, const Hash& hash2) const {
	int score = 0;
	score += std::min(hash1.ADE, hash2.ADE);
	score += std::min(hash1.CYT, hash2.CYT);
	score += std::min(hash1.GUA, hash2.GUA);
	score += std::min(hash1.TYM, hash2.TYM);
	return score;
}

void PreprocessSingle::plus_hash(PreprocessSingle::Hash& h, char acid) const {
	if (acid == 'A') { ++h.ADE; }
	else if (acid == 'C') { ++h.CYT; }
	else if (acid == 'G') { ++h.GUA; }
	else { ++h.TYM; }
}

void PreprocessSingle::minus_hash(PreprocessSingle::Hash& h, char acid) const {
	if (acid == 'A') { --h.ADE; }
	else if (acid == 'C') { --h.CYT; }
	else if (acid == 'G') { --h.GUA; }
	else { --h.TYM; }
}
