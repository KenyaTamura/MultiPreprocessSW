#ifndef TIMER_H
#define TIMER_H

#include<chrono>

class Timer {
public:
	Timer() { mStart = std::chrono::system_clock::now(); };
	~Timer() {};
	int get_millsec() {
		return static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - mStart).count());
	};
	int get_microsec() {
		return static_cast<int>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - mStart).count());
	}
	void start() {
		mStart = std::chrono::system_clock::now();
	};
private:
	std::chrono::system_clock::time_point mStart;
};


#endif
