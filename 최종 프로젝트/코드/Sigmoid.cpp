#include "Sigmoid.h"
#include "SigmoidNode.h"

Sigmoid::Sigmoid(Long capacity)
	: Layer(capacity) {

}

Sigmoid::Sigmoid(const Sigmoid& source)
	: Layer(source) {

}

Sigmoid::~Sigmoid() {

}

Array<double> Sigmoid::Forward(Array<double>& signals) {
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

Array<Array<double>> Sigmoid::BatchForward(Array<Array<double>>& signals) {
	this->batchInputs = signals;
	this->inputCount = signals.GetAt(0).GetLength();
	Array<Array<double>> results(signals);

	Long i = 0; 
	while (i < this->length) {
		// signals[j][i] ��° ��������
		Array<double> resultsPerBatch(signals.GetLength());
		Array<double> signalsPerBatch(signals.GetLength());
		Long j = 0;
		while (j < signals.GetLength()) {
			signalsPerBatch.Store(j, signals[j][i]);
			j++;
		}
		// �ñ׸��̵� ����� ��ġ���� �θ���
		resultsPerBatch = dynamic_cast<Node*>(this->neurons[i])->BatchFlow(signalsPerBatch);

		j = 0; // ����� results[j][i] �� ���� 
		while (j < signals.GetLength()) {
			results[j][i] = resultsPerBatch[j];
			j++;
		}
		i++;
	}

	return results;
}

Array<double> Sigmoid::Backward(Array<double>& signals) {
	this->inputGradients = Array<double>(this->inputCount);
	Neuron *node;
	double gradient;

	Long i = 0;
	while (i < this->length) {
		node = this->neurons.GetAt(i);
		gradient = dynamic_cast<Node*>(node)->BackFlow(const_cast<Array<double>&>(signals)[i]);
		this->inputGradients.Store(i, gradient);
		i++;
	}

	return this->inputGradients;
}

Array<Array<double>> Sigmoid::BatchBackward(Array<Array<double>>& signals) {
	Array<Array<double>> batchInputGradients(signals);

	Long i = 0;
	while (i < this->length) {
		// signals[j][i] ��° ��������
		Array<double> resultsPerBatch(signals.GetLength());
		Array<double> signalsPerBatch(signals.GetLength());
		Long j = 0;
		while (j < signals.GetLength()) {
			signalsPerBatch.Store(j, signals[j][i]);
			j++;
		}
		// �ñ׸��̵� ����� ��ġ���� �θ���
		resultsPerBatch = dynamic_cast<Node*>(this->neurons[i])->BatchBackFlow(signalsPerBatch);

		j = 0; // ����� results[j][i] �� ���� 
		while (j < signals.GetLength()) {
			batchInputGradients[j][i] = resultsPerBatch[j];
			j++;
		}
		i++;
	}


	return batchInputGradients;
}

void Sigmoid::MakeNodes(Long inputCount, Long outputCount) {
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
		node = new SigmoidNode;
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

Long Sigmoid::Save(fstream *fs) {
	*fs << this->length << endl;

	return this->length;
}

Long Sigmoid::Load(fstream *fs) {
	// 1. ��� ���� �д´�.
	Long nodeCount;
	*fs >> nodeCount;
	this->neurons.Resize(nodeCount);
	this->capacity = nodeCount;

	Long i = 0;
	while (i < nodeCount) {	// 2. ��� ����ŭ �ݺ��Ѵ�.
		Node *node = new SigmoidNode; 	//  2.1. ��带 �����.
		this->Add(node); //  2.2. �����Ѵ�.
		i++;
	}

	return this->length; // 3. ��� ���� ����Ѵ�.
}

string Sigmoid::GetType() {
	return "Sigmoid";
}

Neuron* Sigmoid::Clone() {
	return new Sigmoid(*this);
}

Sigmoid& Sigmoid::operator =(const Sigmoid& source) {
	Layer::operator =(source);

	return *this;
}