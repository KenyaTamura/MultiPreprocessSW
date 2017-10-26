#ifndef PREPROCESSSINGLE_H
#define PREPROCESSSINGLE_H

#include"PreprocessBase.h"

class Data;

class PreprocessSingle : public PreprocessBase{
public:
	PreprocessSingle(const Data& txt, const Data& ptn, const int threshold);
	PreprocessSingle(const Data& txt, const Data& ptn, const char* fname);	// research score
	~PreprocessSingle();
private:
	// The number of each acid
	struct Hash {
		int ADE = 0;
		int GUA = 0;
		int CYT = 0;
		int TYM = 0;
	};
	// Hash plus
	inline void plus_hash(Hash& h, char acid) const;
	// Hash minus
	inline void minus_hash(Hash& h, char acid) const;
	// Main process
	/// txt = long sequence, ptn = short sequence, threshold = Border of OK
	void get_range(const Data& txt, const Data& ptn, const int threshold);
	// Get hash
	Hash get_hash(const Data& data, int length) const;
	// Compare hash
	int get_score(const Hash& hash1, const Hash& hash2) const;
	// Check score
	void check_score(const Data& txt, const Data& ptn, int* range);
};


#endif
