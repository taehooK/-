#include "SquaredLoss.h"

SquaredLoss::SquaredLoss() {

}

SquaredLoss::SquaredLoss(const SquaredLoss& source) {

}

SquaredLoss::~SquaredLoss() {

}

double SquaredLoss::Foward(Array<double> results, Array<double> targets) {
	double loss = -1;
	double sum = 0;
	Long i = 0;
	while (i < results.GetLength()) {
		sum += (results[i] - targets[i])*(results[i] - targets[i]);
		i++;
	}
	
	if (sum != 0) {
		loss = sum * 0.5;
	}
	return loss;
}

Array<double> SquaredLoss::Backward(Array<double> results, Array<double> targets) {
	Array<double> backSignals(results.GetLength());
	Long i = 0;
	while (i < results.GetLength()) {
		backSignals.Store(i, results[i] - targets[i]);
		i++;
	}

	return backSignals;
}

SquaredLoss& SquaredLoss::operator=(const SquaredLoss& source) {
	return *this;
}