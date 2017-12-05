#ifndef PREPROCESSMERGE_H
#define PREPROCESSMERGE_H

#include"Data.h"
#include<string>

struct PreprocessMerge {
public:
	std::string operator()(const Data& q, const int threshold){
		int percent = (100 * threshold) / q.size();
		if (q.size() < 350 && percent > 85){
			return "single";
		}
		else if (q.size() < 650 && percent > 80){
			return "double";	
		}
		else if((q.size() < 350 && percent > 50) || (q.size() < 550 && percent > 75)){
			return "triple";
		}
		else{
			return "quad";
		}
	}
};


#endif
