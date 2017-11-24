#ifndef PREPROCESSDOUBLE_H
#define PREPROCESSDOUBLE_H

#include"PreprocessBase.h"

class Data;

class PreprocessDouble : public PreprocessBase {
public:
	PreprocessDouble(const Data& db, const Data& query, const int threshold, int thread_num);
	~PreprocessDouble();
private:
	// Main process
	void process(const Data& db, const Data& query, const int threshold);
	void get_range(const Data& db, const Data& query, const int threshold, int start, int end, int& block, int* buffer);
	// Get hash
	void get_hash(const Data& data, int length, int* hash, int start) const;
	// Compare hash
	int get_score(const int* hash1, const int* hash2) const;
};


#endif
