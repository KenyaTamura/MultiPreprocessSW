#ifndef FILECONVERTER_H
#define FILECONVERTER_H

#include"Data.h"
#include<string>

class FileConverter{
public:
	FileConverter(const char* fname);
	FileConverter(const char* fname, const char* outfname);
	~FileConverter();
private:
	void read(const char* fname);
	std::string convert(std::string s);
	std::string mData;
};


#endif
