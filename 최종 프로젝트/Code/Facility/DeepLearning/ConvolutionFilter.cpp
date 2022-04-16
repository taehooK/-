#include "ConvolutionFilter.h"

ConvolutionFilter::ConvolutionFilter() {
	this->size = 3;
	this->resultsSize = 0;
}

ConvolutionFilter::ConvolutionFilter(Long size, Matrix<double>& weights, Matrix<double>& weightMomentums, Matrix<double>& weightVelocities)
	: weights(weights), weightGradients(weights), weightMomentums(weightMomentums), weightVelocities(weightVelocities) {
	this->size = size;
	this->resultsSize = 0;
}

ConvolutionFilter::ConvolutionFilter(const ConvolutionFilter& source)
	: weights(source.weights), results(source.results), weightGradients(source.weightGradients),
	weightMomentums(source.weightMomentums), weightVelocities(source.weightVelocities) {
	this->size = source.size;
	this->resultsSize = source.resultsSize;
}

ConvolutionFilter::~ConvolutionFilter() {

}

Matrix<double> ConvolutionFilter::Flow(Matrix<double>& inputs, Long stride) {
	Long i;
	Long j;
	Long k;
	Long l;

	Long resultRow = (inputs.GetRow() - this->size) / stride + 1;
	Long resultColumn = (inputs.GetColumn() - this->size) / stride + 1;
	this->results = Matrix<double>(resultRow, resultColumn);
	this->resultsSize = this->results.GetRow();
	double output;

	// 1. ���͸� �� ��ġ�� �Է���� �� ������ �´µ��� �ݺ��Ѵ�.
	i = 0;
	while (i < results.GetRow()) {
		j = 0;
		//  1.1. ���͸� �� ��ġ�� �Է���� �� ������ �´� ���� �ݺ��Ѵ�.
		while (j < results.GetColumn()) {
			//   1.1.1. ���� �� ����ŭ �ݺ��Ѵ�. 
			output = 0.0;
			k = 0;
			while (k < this->size) {
				//    1.1.1.1 ���� �� ����ŭ �ݺ��Ѵ�.
				l = 0;
				while (l < this->size) {
					// 1.1.1.1.1. �� ���� ���� ���Ѵ�.
					// 1.1.1.1.2. ���� ���Ѵ�.
					output += this->weights.GetAt(k, l) * inputs.GetAt(k + i, l + j);
					l++;
				}
				//   1.1.2. ��� ��Ŀ� ���´�.
				k++;
			}
			//   1.1.3. ��Ʈ���̵常ŭ �������� �̵��Ѵ�.
			results.Store(i, j, output);
			j += stride;
		}
		//  1.2. ��Ʈ���̵常ŭ �Ʒ��� �̵��Ѵ�.
		i += stride;
	}

	return this->results;
}

Matrix<double> ConvolutionFilter::BackFlow(Matrix<double>& inputs, Matrix<double>& gradients, Long stride, Long pad) {
	Long i;
	Long j;
	Long k;
	Long l;
	Matrix<double> processingInputs(inputs.AddPadding(pad));

	//���� ����ġ ���� ���ϱ�
	double weightGradient;
	// 1. ���� �����ŭ �ݺ��Ѵ�.
	i = 0;
	while (i < this->size) {
		j = 0;
		//  1.1. ���� �����ŭ �ݺ��Ѵ�.
		while (j < this->size) {
			//   1.1.1. ���� �����ŭ �ݺ��Ѵ�. 
			weightGradient = 0.0;
			k = 0;
			while (k < gradients.GetRow()) {
				//    1.1.1.1 ���� �����ŭ �ݺ��Ѵ�.
				l = 0;
				while (l < gradients.GetColumn()) {
					// 1.1.1.1.1. �� ���� ���� ���Ѵ�.
					// 1.1.1.1.2. ���� ���Ѵ�.
					weightGradient += processingInputs.GetAt(k + i, l + j) * gradients.GetAt(k, l);
					l++;
				}
				k++;
			}
			//   1.1.2. ���� ���� ��Ŀ� ���´�.
			this->weightGradients.Modify(i / stride, j / stride, weightGradient);
			//   1.1.3. ��Ʈ���̵常ŭ �������� �̵��Ѵ�.
			j += stride;
		}
		//  1.2. ��Ʈ���̵常ŭ �Ʒ��� �̵��Ѵ�.
		i += stride;
	}

	//�Է°��� ���� ���� ���ϱ�

	//2. ���� ���� ����ġ�� ������.
	Matrix<double> reversedWeights(this->weights.Reverse());
	Matrix<double> processingMatrix(gradients);

	//3. ������ ��ȣ����� �����ϴ�.
	Long subSize = this->size - 1 - pad;
	if (subSize > 0) {
		processingMatrix = gradients.AddPadding(subSize);
	}
	else if (subSize < 0) {
		processingMatrix = gradients.CutEdge(-subSize);
	}

	Matrix<double> inputGradients(inputs.GetRow(), inputs.GetColumn());
	double inputGradient;
	//3. ���� �����ŭ �ݺ��Ѵ�.
	i = 0;
	while (i < inputGradients.GetRow()) {
		//3.1. ���� �����ŭ �ݺ��Ѵ�.
		j = 0;
		while (j < inputGradients.GetColumn()) {
			//3.1.1. ���� �����ŭ �ݺ��Ѵ�.
			inputGradient = 0.0;
			k = 0;
			while (k < this->size) {
				//3.1.1.1. ���ͻ����ŭ �ݺ��Ѵ�.
				l = 0;
				while (l < this->size) {
					//3.1.1.1.1. �Է±��⸦ ���Ѵ�.
					inputGradient += processingMatrix.GetAt(k + i, l + j) * reversedWeights.GetAt(k, l);
					l++;
				}
				k++;
			}
			//3.1.2. �Է±��� ��Ŀ� ���´�.
			inputGradients.Store(i, j, inputGradient);
			//3.1.3. ��Ʈ���̵常ŭ �̵��Ѵ�.
			j += stride;
		}
		//3.2. ��Ʈ���̵常ŭ �̵��Ѵ�.
		i += stride;
	}
	return inputGradients;
}

Matrix<double> ConvolutionFilter::BatchBackFlow(Matrix<double>& inputs, Matrix<double>& gradients, Long stride, Long pad) {
	Long i;
	Long j;
	Long k;
	Long l;
	Matrix<double> processingInputs(inputs.AddPadding(pad));

	//�Է°��� ���� ���� ���ϱ�

	//2. ���� ���� ����ġ�� ������.
	Matrix<double> reversedWeights(this->weights.Reverse());
	Matrix<double> processingMatrix(gradients);

	//3. ������ ��ȣ����� �����ϴ�.
	Long subSize = this->size - 1 - pad;
	if (subSize > 0) {
		processingMatrix = gradients.AddPadding(subSize);
	}
	else if (subSize < 0) {
		processingMatrix = gradients.CutEdge(-subSize);
	}

	Matrix<double> inputGradients(inputs.GetRow(), inputs.GetColumn());
	double inputGradient;
	//3. ���� �����ŭ �ݺ��Ѵ�.
	i = 0;
	while (i < inputGradients.GetRow()) {
		//3.1. ���� �����ŭ �ݺ��Ѵ�.
		j = 0;
		while (j < inputGradients.GetColumn()) {
			//3.1.1. ���� �����ŭ �ݺ��Ѵ�.
			inputGradient = 0.0;
			k = 0;
			while (k < this->size) {
				//3.1.1.1. ���ͻ����ŭ �ݺ��Ѵ�.
				l = 0;
				while (l < this->size) {
					//3.1.1.1.1. �Է±��⸦ ���Ѵ�.
					inputGradient += processingMatrix.GetAt(k + i, l + j) * reversedWeights.GetAt(k, l);
					l++;
				}
				k++;
			}
			//3.1.2. �Է±��� ��Ŀ� ���´�.
			inputGradients.Store(i, j, inputGradient);
			//3.1.3. ��Ʈ���̵常ŭ �̵��Ѵ�.
			j += stride;
		}
		//3.2. ��Ʈ���̵常ŭ �̵��Ѵ�.
		i += stride;
	}

	return inputGradients;
}

void ConvolutionFilter::BatchBackFlow(Array<Matrix<double>>& inputs, Array<Matrix<double>>& gradients, Long stride, Long pad) {
	//�Ű����� ����
	//inputs : �ش� ���Ϳ� ���� �ִ�(������ ä����) �Է°� ����� ��ġũ�⸸ŭ ���� �迭
	//gradients : �ش� ���Ϳ� ���� �ִ�(���� ��°��) ���� ����� ��ġũ�⸸ŭ ���� �迭

	Long batch = inputs.GetLength(); //��ġ ũ��

	Long i = 0;
	while (i<this->weightGradients.GetLength()) {
		this->weightGradients.Modify(i, 0.0);
		i++;
	}

	Long indexOfBatch = 0;
	while (indexOfBatch < batch) {
		Matrix<double> gradientsPerBatch(gradients.GetAt(indexOfBatch)); //�ش� ���Ϳ� ���� �ִ� n��° ���� ���
		Matrix<double> processingInputs(inputs.GetAt(indexOfBatch).AddPadding(pad)); //�Է°� ��Ŀ� �е�����
		Matrix<double> weightGradientsPerBatch(this->weightGradients); //����ġ���⸦ �������Ѿ� �ϱ� ������ ���� ����ġ������İ� ũ�Ⱑ ������ ���

		//���� ����ġ ���� ���ϱ�
		double weightGradient;
		// 1. ���� �����ŭ �ݺ��Ѵ�.
		Long i = 0;
		while (i < this->size) {
			Long j = 0;
			//  1.1. ���� �����ŭ �ݺ��Ѵ�.
			while (j < this->size) {
				//   1.1.1. ���� �����ŭ �ݺ��Ѵ�. 
				weightGradient = 0.0;
				Long k = 0;
				while (k < gradientsPerBatch.GetRow()) {
					//    1.1.1.1 ���� �����ŭ �ݺ��Ѵ�.
					Long l = 0;
					while (l < gradientsPerBatch.GetColumn()) {
						// 1.1.1.1.1. �� ���� ���� ���Ѵ�.
						// 1.1.1.1.2. ���� ���Ѵ�.
						weightGradient += processingInputs.GetAt(k + i, l + j) * gradientsPerBatch.GetAt(k, l);
						l++;
					}
					k++;
				}
				//   1.1.2. ���� ���� ��Ŀ� ���´�.
				weightGradientsPerBatch.Modify(i / stride, j / stride, weightGradient);
				//   1.1.3. ��Ʈ���̵常ŭ �������� �̵��Ѵ�.
				j += stride;
			}
			//  1.2. ��Ʈ���̵常ŭ �Ʒ��� �̵��Ѵ�.
			i += stride;
		}
		this->weightGradients.Plus(weightGradientsPerBatch); //�Ӽ� ����ġ��������� ��ġ���� ���� ����ġ ���� ����� ������Ų��.
		indexOfBatch++;
	}
}

void ConvolutionFilter::Update(double learningRate) {
	this->weights = this->weights - (this->weightGradients * learningRate);
}

void ConvolutionFilter::Update(double updatedLearningRate, double beta1, double beta2) {
	double weightMomentum;
	double weightVelocity;
	double weight;
	Long length = this->size * this->size;
	Long i = 0;
	while (i < length) {
		weightMomentum = this->weightMomentums[i] + (1 - beta1) * (this->weightGradients[i] - this->weightMomentums[i]);
		this->weightMomentums.Modify(i, weightMomentum);
		weightVelocity = this->weightVelocities[i] + (1 - beta2) * (pow(this->weightGradients[i], 2) - this->weightVelocities[i]);
		this->weightVelocities.Modify(i, weightVelocity);
		weight = this->weights[i] - (updatedLearningRate * this->weightMomentums[i] / (sqrt(this->weightVelocities[i]) + 1e-7));
		this->weights.Modify(i, weight);
		i++;
	}
}

Neuron* ConvolutionFilter::Clone() {
	return new ConvolutionFilter(*this);
}

ConvolutionFilter& ConvolutionFilter::operator =(const ConvolutionFilter& source) {
	Neuron::operator =(source);
	this->weights = source.weights;
	this->size = source.size;
	this->results = source.results;
	this->resultsSize = source.resultsSize;
	this->weightGradients = source.weightGradients;
	this->weightMomentums = source.weightMomentums;
	this->weightVelocities = source.weightVelocities;

	return *this;
}