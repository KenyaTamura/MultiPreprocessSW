#ifndef COST_H
#define COST_H

struct Cost {
	int Begin = -10;
	int Extend = -3;
	int Match = 1;
	int Miss = -3;
};

static Cost cost;

#endif
