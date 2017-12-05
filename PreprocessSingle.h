#ifndef PREPROCESSSINGLE_H
#define PREPROCESSSINGLE_H

#include"PreprocessBase.h"

class Data;

class PreprocessSingle : public PreprocessBase{
public:
	PreprocessSingle(const Data& db, const Data& query, const int threshold, const int thread_num = 8);
	~PreprocessSingle();
private:
	// Main process	
	void process(const Data& db, const Data& query, const int threshold);
	// db = long sequence, query = short sequence, threshold = Border of OK	// Get hash
	void get_range(const Data& db, const Data& query, const int threshold, int start, int end, int& block, int* buffer);
	void get_hash(const Data& data, int length, int* hashi, int start) const;
	// Compare hash
	int get_score(const int* hash1, const int* hash2) const;
	// Check score
	void check_score(const Data& db, const Data& query, int* range);
};


#endif
