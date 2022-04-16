#include "Softmax.h"
#include "SoftmaxNode.h"
#include "Array.h"
#include <cmath>
#include "Layer.h"

Softmax::Softmax(Long capacity)
	: Layer(capacity) {

}

Softmax::Softmax(const Softmax& source)
	: Layer(source) {

}

Softmax::~Softmax() {

}

Array<double> Softmax::Forward(Array<double>& signals) {
	this->inputs = signals;
	this->inputCount = signals.GetLength();
	Array<double> results(this->length);
	Array<double> numerators(this->length);
	double sum = 0.0;
	double max = signals[signals.Max(Compare)];


	Long i = 0;
	while (i < this->length) {
		numerators.Store(i, exp(const_cast<Array<double>&>(signals)[i] - max));
		sum += numerators[i];
		i++;
	}

	i = 0;
	while (i < this->length) {
		double result = dynamic_cast<Node*>(this->neurons[i])->Flow(numerators[i], sum); //��� �ϳ��� ��� = ����/sum
		results.Store(i, result);
		i++;
	}

	return results;
}

Array<Array<double>> Softmax::BatchForward(Array<Array<double>>& signals) {
	this->batchInputs = signals;
	this->inputCount = signals.GetAt(0).GetLength();
	Long batch = signals.GetLength();
	Array<Array<double>> results(signals);
	Array<double> sums(batch);
	Long i = 0;
	while (i < sums.GetCapacity()) {
		sums.Store(i, 0.0);
		i++;
	}
	Array<Array<double>> numerators(batch);
	i = 0;
	while (i < batch) {
		Array<double> signalsPerBatch = signals.GetAt(i);
		double max = signalsPerBatch[signalsPerBatch.Max(Compare)];
		Array<double> numeratorsPerBatch(this->length);

		Long j = 0;
		while (j < this->length) {
			numeratorsPerBatch.Store(j, exp(const_cast<Array<double>&>(signalsPerBatch)[j] - max));
			sums.Modify(i, sums[i] + numeratorsPerBatch[j]);
			j++;
		}
		numerators.Store(i, numeratorsPerBatch);
		i++;
	}

	Array<double> currentResults(batch);
	i = 0;
	while (i < this->length) {
		Array<double> currentNumerators(batch);
		Long j = 0;
		while (j < batch) {
			Array<double> numeratorsPerBatch = numerators.GetAt(j);
			currentNumerators.Store(j, numeratorsPerBatch[i]);
			j++;
		}
		currentResults = dynamic_cast<Node*>(this->neurons[i])->BatchFlow(currentNumerators, sums); //��� �ϳ��� ��� = ����/sum
		j = 0;
		while (j < batch) {
			results[j].Modify(i, currentResults[j]);
			j++;
		}
		i++;
	}

	return results;
}

Array<double> Softmax::Backward(Array<double>& signals) {
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

Array<Array<double>> Softmax::BatchBackward(Array<Array<double>>& signals) {
	Array<Array<double>> batchInputGradients(signals);

	Long i = 0;
	// 1. this->length ��ŭ �ݺ��Ѵ�. .. i
	while (i < this->length) {
		Array<double> signalValues(signals.GetLength());
		Long j = 0;
		//  1.1 �ñ׳ν� ������ŭ �ݺ��Ѵ�. .. j
		while (j < signals.GetLength()) {
			//   1.1.1. �ñ׳��� �����´�.
			//   1.1.2. i��° ��Ҹ� �����´�
			//   1.1.3. �迭�� �����Ѵ�.
			signalValues.Store(j, signals[j][i]);
			j++;
		}
		//  1.2. ��Ʈ�ƽ���忡�� ��ġ ����� �Ѵ�.
		signalValues = dynamic_cast<Node*>(this->neurons[i])->BatchBackFlow(signalValues);
		//  1.3. ������� ������� �����Ѵ�. [j][i]
		j = 0;
		while (j < batchInputGradients.GetLength()) {
			batchInputGradients[j][i] = signalValues[j]/ signals.GetLength();
			j++;
		}
		i++;
	}
	
	return batchInputGradients;
}

void Softmax::MakeNodes(Long inputCount, Long outputCount) {
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
	while (i < outputCount) {
		node = new SoftmaxNode;
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

Long Softmax::Save(fstream *fs) {
	*fs << this->length << endl;

	return this->length;
}

Long Softmax::Load(fstream *fs) {
	// 1. ��� ���� �д´�.
	Long nodeCount;
	*fs >> nodeCount;
	this->neurons.Resize(nodeCount);
	this->capacity = nodeCount;

	Long i = 0;
	while (i < nodeCount) {	// 2. ��� ����ŭ �ݺ��Ѵ�.
		Node *node = new SoftmaxNode; 	//  2.1. ��带 �����.
		this->Add(node); //  2.2. �����Ѵ�.
		i++;
	}

	return this->length; // 3. ��� ���� ����Ѵ�.
}

string Softmax::GetType() {
	return "SoftMax";
}

Neuron* Softmax::Clone() {
	return new Softmax(*this);
}

Softmax& Softmax::operator =(const Softmax& source) {
	Layer::operator =(source);

	return *this;
}

int Compare(void *one, void *other) {
	double *one_ = static_cast<double*>(one);
	double *other_ = static_cast<double*>(other);
	int ret;

	if (*one_ < *other_) {
		ret = -1;
	}
	else if (*one_ == *other_) {
		ret = 0;
	}
	else if (*one_ > *other_) {
		ret = 1;
	}
	return ret;
}