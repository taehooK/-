#ifndef _SIGMOIDNODE_H
#define _SIGMOIDNODE_H

#include "Node.h"

class SigmoidNode : public Node {
public:
	SigmoidNode();
	SigmoidNode(const SigmoidNode& source);
	virtual ~SigmoidNode();

	virtual double Flow(double signal); //�Է� �ϳ��� ��� �ϳ�
	virtual Array<double> BatchFlow(Array<double>& signals);

	virtual double BackFlow(double signal);
	virtual Array<double> BatchBackFlow(Array<double>& signal);


	virtual Neuron* Clone();

	SigmoidNode& operator =(const SigmoidNode& source);
private:
	Array<double> batchResult;

};

#endif //_SIGMOIDNODE_H