#ifndef _SQUAREDLOSS_H
#define _SQUAREDLOSS_H

#include "Array.h"

class SquaredLoss {
public:
	SquaredLoss();
	SquaredLoss(const SquaredLoss& source);
	~SquaredLoss();
	double Foward(Array<double> results, Array<double> targets);
	Array<double> Backward(Array<double> results, Array<double> targets);
	SquaredLoss& operator=(const SquaredLoss& source);
};




#endif