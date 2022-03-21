//Pooling.h

#ifndef _POOLING_H
#define _POOLING_H

#include "Layer.h"
#include "Array.h"


typedef signed long int Long;

class Neuron;
class Pooling : public Layer {
public:
	Pooling(Long capacity = 16, Long size = 2, Long stride = 2);
	Pooling(const Pooling& source);
	virtual ~Pooling();
	virtual Array<double> Forward(Array<double>& signals);
	virtual Array<Array<double>> BatchForward(Array<Array<double>>& signals);
	virtual Array<double> Backward(Array<double>& signals);
	virtual Array<Array<double>> BatchBackward(Array<Array<double>>& signals);
	virtual void MakeFilters(Long channelCount, Long filterCount);
	virtual Long Save(fstream *fs);
	virtual Long Load(fstream *fs);
	virtual Neuron* Clone();
	virtual string GetType();

	Long GetSize() const;
	Long GetStride() const;


	Pooling& operator=(const Pooling& source);
private:
	Long size;
	Long stride;
};

inline Long Pooling::GetSize() const {
	return this->size;
}

inline Long Pooling::GetStride() const {
	return this->stride;
}



#endif // !_POOLING_H