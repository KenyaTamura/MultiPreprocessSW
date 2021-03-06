#ifndef PREPROCESSSWGPU_CUH
#define PREPROCESSSWGPU_CUH

#include"SWBase.h"
class Data;
class PreprocessBase;

class PreprocessSWGPU : public SWBase {
public:
	PreprocessSWGPU(const Data& txt, const Data& ptn, const PreprocessBase& pre, int threshold);
	~PreprocessSWGPU();
private:
	void call_DP(const Data& txt, const Data& ptn, const PreprocessBase& pre);
	void checkScore(const Data& txt, const Data& ptn);
	void traceback(const char* direction, const Data& txt, int x, int y, int length) const;
};


#endif





