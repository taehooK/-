#include "PoolingFilter.h"

PoolingFilter::PoolingFilter() {
	this->size = 0;
	this->resultSize = 0;
}

PoolingFilter::PoolingFilter(Long size) {
	this->size = size;
	this->resultSize = 0;
}

PoolingFilter::PoolingFilter(const PoolingFilter& source)
	: results(source.results) {
	this->size = source.size;
	this->resultSize = source.resultSize;
}

PoolingFilter::~PoolingFilter() {

}

Matrix<double> PoolingFilter::Flow(Matrix<double>& signals, Long stride) {
	this->resultSize = (signals.GetRow() - this->size) / stride + 1;
	this->results = Matrix<double>(this->resultSize, this->resultSize);
	Long i = 0;
	Long j;
	Long k;
	Long l;
	double value;
	double max;

	while (i < resultSize) {
		j = 0;
		while (j < resultSize) {
			k = 0;
			while (k < this->size) {
				l = 0;
				while (l < this->size) {
					value = signals.GetAt(k + (i*stride), l + (j*stride));
					if ((k == 0 && l == 0) || max < value) {
						max = value;
					}
					l++;
				}
				k++;
			}
			this->results.Store(i, j, max);
			j++;
		}
		i++;
	}

	return this->results;
}

Matrix<double> PoolingFilter::BackFlow(Matrix<double>& inputs, Matrix<double>& signals, Long stride) {
	Long i;
	Long j;
	Long k;
	Long l;
	Long m = 0;
	double max;
	
	double inputGradient;
	Matrix<double> inputGradients(inputs.GetRow(), inputs.GetColumn());
	i = 0;
	while (i < inputGradients.GetRow()) {
		j = 0;
		while (j < inputGradients.GetColumn()) {
			// 필터 사이즈 영역의 max 구하기 
			k = 0;
			while (k < this->size) {
				l = 0;
				while (l < this->size) {
					if ((k == 0 && l == 0 ) || inputs.GetAt(k + i, l + j) > max) {
						max = inputs.GetAt(k + i, l + j);
					}
					l++;
				}
				k++;
			}

			k = 0;
			while (k < this->size) {
				l = 0;
				while (l < this->size) {
					if (inputs.GetAt(k + i, l + j) == max) {
						inputGradient = signals.GetAt(m);
					}
					else {
						inputGradient = 0.0;
					}
					inputGradients.Store(k + i, l + j, inputGradient);
					l++;
				}
				k++;
			}
			m++;
			j += stride;
		}
		i += stride;
	}

	return inputGradients;
}

Neuron* PoolingFilter::Clone() {
	return new PoolingFilter(*this);
}

PoolingFilter& PoolingFilter::operator=(const PoolingFilter& source) {
	this->results = source.results;
	this->size = source.size;
	this->resultSize = source.resultSize;
	return *this;
}