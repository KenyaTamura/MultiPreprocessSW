#ifndef PREPROCESSBASE_H
#define PREPROCESSBASE_H

class Data;

class PreprocessBase {
public:
	PreprocessBase() : mRange{ nullptr }, mBlock{ 0 } {};
	virtual ~PreprocessBase() {};
	int get(int i) const;
	int* getAll() const;
	int block() const;
protected:
	// 0,13,25,50 = 0~13 and 25~50
	int* mRange;
	// The number of range blocks
	int mBlock;
	// First call
	void process(const Data& txt, const Data& ptn, const int threshold, const char* id);
	// Main process
	virtual void get_range(const Data& txt, const Data& ptn, const int threshold) {};
};


#endif
