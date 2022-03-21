#include "Affine.h"
#include "AffineNode.h"
#include "Node.h"
#include <random> //���� ����
#include <chrono> //���� ����
#include <iostream>

using namespace std; //���� ����

Affine::Affine(Long capacity, Long inputCount, double deviation)
	: Layer(capacity) {
	this->inputCount = inputCount;
	this->deviation = deviation;
}

Affine::Affine(const Affine& source)
	: Layer(source) {
	this->deviation = source.deviation;
}

Affine::~Affine() {

}

Array<double> Affine::Forward(Array<double>& signals) {
	this->inputs = signals;
	this->inputCount = signals.GetLength();
	Array<double> outputs(this->length); //�������� ��� �迭
	double output;

	Long i = 0;
	while (i < this->length) {
		output = dynamic_cast<Node*>(this->neurons[i])->Flow(signals);
		//�Է°� ��ο� ��� �ϳ��� ����ġ �迭�� ���� ��İ�
		outputs.Store(i, output);
		i++;
	}

	return outputs;
}

Array<Array<double>> Affine::BatchForward(Array<Array<double>>& signals) {
	this->batchInputs = signals;
	this->inputCount = signals.GetAt(0).GetLength();
	Array<Array<double>> outputs(signals.GetLength()); //�������� ��� �迭
	double output;

	Long i = 0;
	//��ġ ũ�⸸ŭ �ݺ��Ѵ�.
	while(i < signals.GetLength()) {
		Array<double> outputsPerBatch(this->length);
		Array<double> signalsPerBatch = signals.GetAt(i);
		Long j = 0;
		while (j < this->length) { //��� ������ŭ �ݺ��Ѵ�.
			output = dynamic_cast<Node*>(this->neurons[j])->Flow(signalsPerBatch);
			outputsPerBatch.Store(j, output);
			j++;
		}
		outputs.Store(i, outputsPerBatch);
		i++;
	}


	return outputs;
}

Array<double> Affine::Backward(Array<double>& signals) {
	//Array<double> weights(this->length); //������ n��° ����ġ �迭
	this->inputGradients = Array<double>(this->inputCount);
	double weight;
	Neuron *node = 0;
	Long i = 0;
	while (i < this->inputCount) { //�Է°��� ���� ������ ���� = �Է°� ����
		//n��° ����ġ�� ��� ��忡�� ��������
		Array<double> weights(this->length);
		Long j = 0;
		while (j < this->length) {
			node = this->neurons[j];
			weight = dynamic_cast<AffineNode*>(node)->GetWeight(i); //���� ����� k��° ����ġ
			weights.Store(j, weight);
			j++;
		}
		//���⸦ ������ ����ġ�� signal�� �̿��� ��İ��ϱ�
		double gradient = 0.0;
		j = 0;
		while (j < this->length) {
			gradient += signals[j] * weights[j];
			j++;
		}
		//�Է°��� ���� ���� ���ϱ�
		this->inputGradients.Store(i, gradient);
		i++;
	}
	//����ġ�� ���� ����, ���⿡ ���� ���� �����ϱ�
	i = 0;
	while (i < this->length) {
		node = this->neurons[i];
		//�����İ���, k��°����ġ��, �Է°���� ���� �ε���
		dynamic_cast<Node*>(node)->BackFlow(signals[i], this->inputs);
		i++;
	}

	return this->inputGradients;
}

Array<Array<double>> Affine::BatchBackward(Array<Array<double>>& signals) {
	double weight;
	Neuron *node = 0;
	Long batch = signals.GetLength();
	Array<Array<double>> batchInputGradients(batch);

	Array<Array<double>> allWeights(this->inputCount);
	Long i = 0;
	while (i < this->inputCount) {
		Array<double> weights(this->length);
		Long j = 0;
		while (j < this->length) {
			node = this->neurons[j];
			weight = dynamic_cast<AffineNode*>(node)->GetWeight(i); //���� ����� k��° ����ġ
			weights.Store(j, weight);
			j++;
		}
		allWeights.Store(i, weights);
		i++;
	}

	i = 0;
	//��ġ ũ�⸸ŭ �ݺ��Ѵ�.
	while (i < batch) {
		Array<double> batchInputGradientsPerBatch(this->inputCount);
		//Array<double> signalsPerBatch = signals.GetAt(i);
		Long j = 0;
		//�Է� ������ŭ �ݺ��Ѵ�.
		while (j < this->inputCount) { //�Է°��� ���� ������ ���� = �Է°� ����
			double gradient = 0.0;
			Long k = 0;
			while (k < this->length) {
				gradient += signals[i][k] * allWeights[j][k];
				k++;
			}
			batchInputGradientsPerBatch.Store(j, gradient);
			j++;
		}
		batchInputGradients.Store(i, batchInputGradientsPerBatch);
		i++;
	}

	i = 0;
	//��� ������ŭ �ݺ��ϸ鼭 batchbackflow�� ���� ����ġ�� ���� ���⸦ ���Ѵ�.
	while (i < this->length) {
		Array<double> backFlowSignals(batch); //BatchBackFlow �� ��ȣ�� : n��° �����
		Long j = 0;
		while (j < batch) { //��ġ ũ�⸸ŭ �ݺ��ϸ鼭 j��° ����鸸 ����Ѵ�.
			Array<double> signalsPerBatch = signals.GetAt(j);
			backFlowSignals.Store(j, signalsPerBatch[i]);
			j++;
		}
		dynamic_cast<Node*>(this->neurons[i])->BatchBackFlow(backFlowSignals, this->batchInputs);
		i++;
	}

	return batchInputGradients;
}

void Affine::Update(double learningRate) {
	Long i = 0;
	while (i < this->GetLength()) {
		Neuron *affineNode = this->neurons[i];
		dynamic_cast<Node*>(affineNode)->Update(learningRate);
		i++;
	}
}

void Affine::Update(double updatedLearningRate, double beta1, double beta2) {
	Long i = 0;
	while (i < this->length) {
		Neuron *affineNode = this->neurons[i];
		dynamic_cast<Node*>(affineNode)->Update(updatedLearningRate, beta1, beta2);
		i++;
	}
}

void Affine::MakeNodes(Long inputCount, Long outputCount, double deviation) {
	unsigned seed = chrono::steady_clock::now().time_since_epoch().count();
	default_random_engine e(seed);
	this->deviation = deviation;
	normal_distribution<double> distrN(0.0, deviation);
	Neuron *node;

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

	i = 0;
	while (i < outputCount) { //��� ������ŭ �ݺ��Ѵ�.
		Array<double> temp(inputCount); //��� �� ���� �Ӽ��� ����ġ �迭
		Array<double> weightMomentums(inputCount);
		Array<double> weightVelocities(inputCount);

		Long j = 0;
		while (j < inputCount) {
			temp.Store(j, distrN(e));//������ �迭�� �ֱ�
			weightMomentums.Store(j, 0.0);
			weightVelocities.Store(j, 0.0);
			j++;
		}

		node = new AffineNode(temp, 0.0, weightMomentums, weightVelocities, 0.0, 0.0); //AffineNode�� �Ű������� ���� ������ : ����ġ�迭, ����

		if (this->length < this->capacity) {
			this->neurons.Store(this->length, node); //neurons �迭�� �߰�
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

Long Affine::Save(fstream *fs) {
	string content;

	(*fs) << to_string(this->length) << "\n"; // ��� ����
	(*fs) << to_string(this->inputCount) << "\n"; // ����ġ ����
	(*fs) << to_string(this->deviation) << "\n"; //����
	double value;
	double bias;
	double biasMomentum;
	double biasVelocity;
	Long j;
	Long i = 0;
	while (i < this->length) {
		Neuron *node = this->neurons[i];
		j = 0;
		while (j < node->GetLength()) {
			value = dynamic_cast<Node*>(node)->GetWeight(j);
			(*fs) << value << " ";
			j++;
		}
		(*fs) << "\n";

		bias = dynamic_cast<Node*>(node)->GetBias();
		(*fs) << bias << "\n";
		biasMomentum = dynamic_cast<Node*>(node)->GetBiasMomentum();
		(*fs) << biasMomentum << "\n";
		biasVelocity = dynamic_cast<Node*>(node)->GetBiasVelocity();
		(*fs) << biasVelocity << "\n";

		j = 0;
		while (j < node->GetLength()) { // ����� ����
			value = dynamic_cast<Node*>(node)->GetWeightMomentum(j);
			(*fs) << value << " ";
			j++;
		}
		(*fs) << "\n";
		j = 0;
		while (j < node->GetLength()) { // �ӵ� ����
			value = dynamic_cast<Node*>(node)->GetWeightVelocity(j);
			(*fs) << value << " ";
			j++;
		}
		(*fs) << "\n";

		i++;
	}

	return this->length;
}

Long Affine::Load(fstream *fs) {
	Long nodeCount;
	Long weightCount;
	double value;
	double bias;
	Neuron *node;
	// 1. ��� ������ �д´�.
	// 2. ����ġ ������ �д´�.
	*fs >> nodeCount;
	this->neurons.Resize(nodeCount);
	this->capacity = nodeCount;

	*fs >> weightCount;

	*fs >> this->deviation;
	double biasMomentum;
	double biasVelocity;
	Long i = 0;
	Long j;

	// 3. ��� ������ŭ �ݺ��Ѵ�.
	while (i < nodeCount) {
		//  3.1. ����ġ ������ŭ �ݺ��Ѵ�.
		Array<double> weights(weightCount);
		Array<double> weightMomentums(weightCount);
		Array<double> weightVelocities(weightCount);
		j = 0;
		while (j < weightCount) {
			//   3.1.1. ����ġ�� �д�.
			*fs >> value;
			weights.Store(j, value);
			j++;
		}
		//  3.2. ������ �д�.
		*fs >> bias;
		*fs >> biasMomentum;
		*fs >> biasVelocity;
		//  3.3. ��带 �����.
		// ������� �д�.
		j = 0;
		while (j < weightCount) {
			*fs >> value;
			weightMomentums.Store(j, value);
			j++;
		}
		// �ӵ��� �д�.
		j = 0;
		while (j < weightCount) {
			//   3.1.1. ����ġ�� �д�.
			*fs >> value;
			weightVelocities.Store(j, value);
			j++;
		}

		node = new AffineNode(weights, bias, weightMomentums, weightVelocities, biasMomentum, biasVelocity);
		//  3.4. Affine�� �ִ´�.
		this->Add(node);
		i++;
	}

	// 4. ��� ������ ����ϴ�.
	return this->length;
}

Array<double> Affine::GetWeights() {
	Array<double> weights(this->length * this->inputCount); //���� = ��� ���� * ��� �ϳ��� ����ġ �迭 ����
	Long k = 0;
	Neuron* node;
	double weight;
	Long j;
	Long i = 0;

	while (i < this->length) { //��� ������ŭ �ݺ�
		node = this->neurons[i]; //���� ��� ��������
		j = 0;
		while (j < this->inputCount) { //����� ����ġ ������ŭ �ݺ�
			weight = dynamic_cast<AffineNode*>(node)->GetWeight(j);
			weights.Store(k, weight);
			k++; //weigths �迭�� ÷��
			j++;
		}
		i++;
	}

	return weights;
}

Array<double> Affine::GetWeightGradients() {
	Array<double> weights(this->length * this->inputCount);
	Long k = 0;
	Neuron* node;
	double weight;
	Long j;
	Long i = 0;

	while (i < this->length) {
		node = this->neurons[i];
		j = 0;
		while (j < this->inputCount) {
			weight = dynamic_cast<AffineNode*>(node)->GetWeightGradient(j);
			weights.Store(k, weight);
			k++;
			j++;
		}
		i++;
	}

	return weights;
}

string Affine::GetType() {
	return "Affine";
}

Neuron* Affine::Clone() {
	return new Affine(*this);
}

Affine& Affine::operator=(const Affine& source) {
	Layer::operator =(source);
	this->deviation = source.deviation;
	return *this;
}