#include "Dropout.h"
#include "DropoutNode.h"
#include "Node.h"
#include <random>
#include <chrono>

Dropout::Dropout(Long capacity, double ratio)
	: Layer(capacity) {
	this->ratio = ratio;

}

Dropout::Dropout(const Dropout& source)
	: Layer(source) {
	this->ratio = source.ratio;
}

Dropout::~Dropout() {

}

Array<double> Dropout::Forward(Array<double>& signals, bool isTraining) {
	unsigned seed = chrono::steady_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	uniform_real_distribution<double> distribution(0.0, 1.0);

	this->inputs = signals;
	this->inputCount = signals.GetLength();

	Array<double> results(this->length);
	Neuron *node;
	Long i = 0;
	while (i < this->length) {
		node = this->neurons.GetAt(i);
		double randomNumber = distribution(generator);
		double result;
		if (randomNumber > this->ratio || isTraining == false) {
			result = dynamic_cast<Node*>(node)->Flow(signals[i], true);
		}
		else {
			result = dynamic_cast<Node*>(node)->Flow(0.0, false);
		}
		results.Store(i, result);
		i++;
	}

	return results;
}

Array<Array<double>> Dropout::BatchForward(Array<Array<double>>& signals, bool isTraining) {
	unsigned seed = chrono::steady_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	uniform_real_distribution<double> distribution(0.0, 1.0);

	Long batch = signals.GetLength();
	this->batchInputs = signals;
	this->inputCount = signals.GetAt(0).GetLength();
	Array<Array<double>> results(signals);

	Long i = 0;
	while (i < this->length) { // ��� ������ŭ �ݺ�
		Array<double> signalsPerBatch(signals.GetLength());
		Long j = 0;
		while (j < signals.GetLength()) { // ��ġ ������ŭ �ݺ�, �ñ׳ν����� i��°�� ������
			signalsPerBatch.Store(j, signals[j][i]);
			j++;
		}

		Array<bool> batchIsActive(signals.GetLength());
		j = 0;
		while (j < signals.GetLength()) { // ������ ������ ����
			double randomNumber = distribution(generator);
			if (randomNumber > this->ratio || isTraining == false) {
				batchIsActive.Store(j, true);
			}
			else {
				batchIsActive.Store(j, false);
				signalsPerBatch[j] = 0.0;
			}
			j++;
		}

		Neuron *node = this->neurons.GetAt(i);
		signalsPerBatch = dynamic_cast<Node*>(node)->BatchFlow(signalsPerBatch, batchIsActive);

		// results�� ����� ���´�. 
		j = 0;
		while (j < signalsPerBatch.GetLength()) {
			results[j][i] = signalsPerBatch[j];
			j++;
		}
		i++;
	}

	return results;
}

Array<double> Dropout::Backward(Array<double>& signals) {
	this->inputGradients = Array<double>(this->inputCount);
	Neuron *node;
	double gradient;

	Long i = 0;
	while (i < this->length) {
		node = this->neurons.GetAt(i);
		gradient = dynamic_cast<Node*>(node)->BackFlow(signals[i]);
		this->inputGradients.Store(i, gradient);
		i++;
	}

	return this->inputGradients;
}

Array<Array<double>> Dropout::BatchBackward(Array<Array<double>>& signals) {
	Long batch = signals.GetLength();
	Array<Array<double>> batchInputGradients(signals);

	Long i = 0;
	while (i < this->length) { // ��� ������ŭ �ݺ�
		Array<double> signalsPerBatch(signals.GetLength());
		Long j = 0;
		while (j < signals.GetLength()) { // ��ġ ������ŭ �ݺ�, �ñ׳ν����� i��°�� ������
			signalsPerBatch.Store(j, signals[j][i]);
			j++;
		}

		Neuron *node = this->neurons.GetAt(i);
		signalsPerBatch = dynamic_cast<Node*>(node)->BatchBackFlow(signalsPerBatch);

		// results�� ����� ���´�. 
		j = 0;
		while (j < signalsPerBatch.GetLength()) {
			batchInputGradients[j][i] = signalsPerBatch[j];
			j++;
		}
		i++;
	}

	return batchInputGradients;
}

void Dropout::MakeNodes(Long inputCount, Long outputCount) {
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
		node = new DropoutNode;
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
}

Long Dropout::Save(fstream *fs) {
	*fs << this->length << endl;
	*fs << this->ratio << endl;

	return this->length;
}

Long Dropout::Load(fstream *fs) {
	// ��� ���� �д�.
	Long nodeCount;
	*fs >> nodeCount;
	this->neurons.Resize(nodeCount);
	this->capacity = nodeCount;
	// ������ �д�.
	*fs >> this->ratio;

	// ��� ����ŭ �ݺ��ϴ�.
	Long i = 0;
	while (i < nodeCount) {
		Node *node = new DropoutNode;//  ��带 �����.
		this->Add(node);//  �����Ѵ�.
		i++;
	}
	return this->length;
}

string Dropout::GetType() {
	return "Dropout";
}

Neuron* Dropout::Clone() {
	return new Dropout(*this);
}

Dropout& Dropout::operator =(const Dropout& source) {
	Layer::operator=(source);
	this->ratio = source.ratio;


	return *this;
}
