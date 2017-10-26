#ifndef PREPROCESSSW_H
#define PREPROCESSSW_H

#include"SWBase.h"
class PreprocessBase;
class Data;

class PreprocessSW : public SWBase{
public:
	PreprocessSW(const Data& db, const Data& query, const PreprocessBase& pre, int threshold);
	~PreprocessSW();
private:
	PreprocessSW() {};
	// Culculate the score by Dynamic Programing 
	void DP(const Data& db, const Data& query, int start, int end);
};



#endif
