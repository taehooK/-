#include "ReLUNode.h"

ReLUNode::ReLUNode() {

}

ReLUNode::ReLUNode(const ReLUNode& source)
	:Node(source) {

}

ReLUNode::~ReLUNode() {

}

double ReLUNode::Flow(double signal) {
	if (signal > 0.0f) { //�Է��� 0���� ũ�� �Է°� �״�� ���
		this->result = signal;
	}
	else { //�Է��� 0���� ������ 0 ���
		this->result = 0;
	}

	return this->result;
}

double ReLUNode::BackFlow(double signal, double input) {
	double gradient = 0.0;
	if (input > 0) {
		gradient = signal;
	}

	return gradient;
}

Neuron* ReLUNode::Clone() {
	return new ReLUNode(*this);
}

ReLUNode& ReLUNode::operator =(const ReLUNode& source) {
	Node::operator =(source);

	return *this;
}