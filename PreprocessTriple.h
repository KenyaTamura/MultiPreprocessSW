#ifndef PREPROCESSTRIPLE_H
#define PREPROCESSTRIPLE_H

#include"PreprocessBase.h"

class Data;

class PreprocessTriple : public PreprocessBase {
public:
	PreprocessTriple(const Data& db, const Data& query, const int threshold, const int thread_num = 8);
	~PreprocessTriple();
private:
	// Main process
	void process(const Data& db, const Data& query, const int threshold);
	void get_range(const Data& db, const Data& query, const int threshold, int start, int end, int& block, int* buffer);
	// Get hash
	void get_hash(const Data& data, int length, int* hash, int start) const;
	// Compare hash
	int get_score(const int* hash1, const int* hash2) const;
	// Check score
	void check_score(const Data& db, const Data& query, int* range);
};


#endif
