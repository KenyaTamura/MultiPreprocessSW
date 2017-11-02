#ifndef WRITER_H
#define WRITER_H

class Writer {
public:
	Writer();
	~Writer();
	void writing_score(const char* fname, const int* data, int size);
	void writing_score(const char* fname, const int* data, int size, int interval, int timer = 3);
	void writing_time(const char* fname, int time);
private:
};


#endif
