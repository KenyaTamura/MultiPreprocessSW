#ifndef SIMPLESW_CUH
#define SIMPLESW_CUH

#include"SWBase.h"
class Data;

class SimpleSW : public SWBase {
public:
	SimpleSW(const Data& txt, const Data& ptn, int threshold, int block_num = 4);
	~SimpleSW();
private:
	void call_DP(const Data& txt, const Data& ptn);
	void checkScore(const Data& txt, const Data& ptn);
	void traceback(const char* direction, const Data& txt, int x, int y, int length) const;

	const int mBlock;
};


#endif





