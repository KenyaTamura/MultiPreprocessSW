#ifndef PREPROCESSQUAD_H
#define PREPROCESSQUAD_H

#include"PreprocessBase.h"

class Data;

class PreprocessQuad : public PreprocessBase{
public:
	PreprocessQuad(const Data& db, const Data& query, const int threshold, const int thread_num = 8);
	~PreprocessQuad();
private:
	// Main process
	void process(const Data& db, const Data& query, const int threshold);
	void get_range(const Data& db, const Data& query, const int threshold, int starrt, int end, int& block, int* buffer);
	// Get hash
	void get_hash(const Data& data, int length, int* hash, int start) const;
	// Compare hash
	int get_score(const int* hash1, const int* hash2) const;
};

#endif
