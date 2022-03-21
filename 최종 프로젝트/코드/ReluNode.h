#ifndef _RELUNODE_H
#define _RELUNODE_H

#include "Node.h"

class Neuron;
class ReLUNode :public Node {
public:
	ReLUNode();
	ReLUNode(const ReLUNode& source);
	virtual ~ReLUNode();
	
	virtual double Flow(double signal); //�Է� �ϳ��� ��� �ϳ�
	virtual double BackFlow(double signal, double input);

	virtual Neuron* Clone();

	ReLUNode& operator =(const ReLUNode& source);
};

#endif // !_RELUNODE_H
