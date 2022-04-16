#include "ReLU.h"
#include "ReluNode.h"
#include "CompositeNeuron.h"
#include "Layer.h"

ReLU::ReLU(Long capacity)
	: Layer(capacity) {

}

ReLU::ReLU(const ReLU& source)
	: Layer(source) {

}

ReLU::~ReLU() {

}

Array<double> ReLU::Forward(Array<double>& signals) {
	this->inputs = signals;
	this->inputCount = signals.GetLength();
	double result;
	Array<double> results(this->length); //��°��� ���� = ��� ����
	Neuron *node;
	Long i = 0;
	while (i < this->length) {
		node = this->neurons.GetAt(i);
		result = dynamic_cast<Node*>(node)->Flow(const_cast<Array<double>&>(signals)[i]); //�Է°� �ϳ����� ������
		results.Store(i, result);
		i++;
	}


	return results;
}

Array<Array<double>> ReLU::BatchForward(Array<Array<double>>& signals) {
	this->batchInputs = signals;
	this->inputCount = signals.GetAt(0).GetLength();
	double result;
	Array<Array<double>> results(signals);

	Long i = 0;
	//��ġ ũ�⸸ŭ �ݺ��Ѵ�.
	while (i < signals.GetLength()) {
		Array<double> signalsPerBatch = signals.GetAt(i);
		Long j = 0;
		while (j < this->length) { //��� ������ŭ �ݺ��Ѵ�.
			result = dynamic_cast<Node*>(this->neurons[j])->Flow(signalsPerBatch[j]);
			results[i].Modify(j, result);
			j++;
		}
		i++;
	}

	return results;
}

Array<double> ReLU::Backward(Array<double>& signals) {
	this->inputGradients = Array<double>(this->inputCount);
	Neuron *node;
	double gradient;

	Long i = 0;
	while (i < this->length) {
		node = this->neurons.GetAt(i);
		gradient = dynamic_cast<Node*>(node)->BackFlow(const_cast<Array<double>&>(signals)[i], this->inputs[i]);
		this->inputGradients.Store(i, gradient);
		i++;
	}

	return this->inputGradients;
}

Array<Array<double>> ReLU::BatchBackward(Array<Array<double>>& signals) {
	Array<Array<double>> batchInputGradients(signals.GetLength());
	double gradient;

	Long i = 0;
	while (i < signals.GetLength()) {
		Array<double> batchInputGradientsPerBatch(this->length);
		Array<double> signalsPerBatch = signals.GetAt(i);
		Array<double> inputsPerBatch = this->batchInputs.GetAt(i);
		Long j = 0;
		while (j < this->length) {
			gradient = dynamic_cast<Node*>(this->neurons[j])->BackFlow(signalsPerBatch[j], inputsPerBatch[j]);
			batchInputGradientsPerBatch.Store(j, gradient);
			j++;
		}
		batchInputGradients.Store(i, batchInputGradientsPerBatch);
		i++;
	}

	return batchInputGradients;
}

void ReLU::MakeNodes(Long inputCount, Long outputCount) {
	Neuron *neuron;
	Long i = 0;
	while (i < this->length) { //neurons�� ��ҵ��� ������ : �Ҵ� ����
		neuron = this->neurons[i];
		if (neuron != 0) {
			delete neuron;
		}
		i++;
	}
	this->neurons.Clear();
	this->neurons.Resize(outputCount);
	this->capacity = outputCount;
	this->length = 0;

	Neuron *node = 0;
	i = 0;
	while (i < outputCount) { //��� ������ŭ �ݺ� ReLU������ �Է°��� = ���(���)����
		node = new ReLUNode;
		if (this->length < this->capacity) {
			this->neurons.Store(i, node);
		}
		else {
			this->neurons.AppendFromRear(node);
			this->capacity++;
		}
		this->length++;
		i++;
	}

	this->inputCount = inputCount;
}

Long ReLU::Save(fstream *fs) {
	*fs << this->length << endl;

	return this->length;
}

Long ReLU::Load(fstream *fs) {
	// 1. ��� ���� �д´�.
	Long nodeCount;
	*fs >> nodeCount;
	this->neurons.Resize(nodeCount);
	this->capacity = nodeCount;

	Long i = 0;
	while (i < nodeCount) {	// 2. ��� ����ŭ �ݺ��Ѵ�.
		Node *node = new ReLUNode; 	//  2.1. ��带 �����.
		this->Add(node); //  2.2. �����Ѵ�.
		i++;
	}

	return this->length; // 3. ��� ���� ����Ѵ�.
}

string ReLU::GetType() {
	return "ReLU";
}

Neuron* ReLU::Clone() {
	return new ReLU(*this);
}

ReLU& ReLU::operator =(const ReLU& source) {
	Layer::operator =(source);

	return *this;
}