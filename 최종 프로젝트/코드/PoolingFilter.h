#ifndef _POOLINGFILTER_H
#define _POOLINGFILTER_H

#include "Neuron.h"
#include "Matrix.h"

class PoolingFilter : public Neuron {
public:
	PoolingFilter();
	PoolingFilter(Long size);
	PoolingFilter(const PoolingFilter& source);
	virtual ~PoolingFilter();

	Matrix<double> Flow(Matrix<double>& signals, Long stride);
	Matrix<double> BackFlow(Matrix<double>& inputs, Matrix<double>& signals, Long stride);

	virtual Neuron* Clone();

	double GetResult(Long row, Long column) const;
	Long GetSize() const;
	Long GetResultSize() const;

	PoolingFilter& operator =(const PoolingFilter& source);
private:
	Matrix<double> results;
	Long resultSize;
	Long size;
};

inline double PoolingFilter::GetResult(Long row, Long column) const {
	return const_cast<PoolingFilter*>(this)->results.GetAt(row, column);
}

inline Long PoolingFilter::GetSize() const {
	return this->size;
}

inline Long PoolingFilter::GetResultSize() const {
	return this->results.GetRow();
}

#endif //_POOLINGFILTER_H`