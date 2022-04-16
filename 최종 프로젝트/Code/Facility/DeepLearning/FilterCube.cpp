#include "FilterCube.h"
#include "ConvolutionFilter.h"
#include "Matrix.h"
#include <cmath>

FilterCube::FilterCube(Long capacity, double bias, double biasMomentum, double biasVelocity)
	: CompositeNeuron(capacity) {
	this->bias = bias;
	this->biasGradient = 0;
	this->biasMomentum = biasMomentum;
	this->biasVelocity = biasVelocity;
}

FilterCube::FilterCube(const FilterCube& source)
	: CompositeNeuron(source) {
	this->bias = source.bias;
	this->biasGradient = source.biasGradient;
	this->biasMomentum = source.biasMomentum;
	this->biasVelocity = source.biasVelocity;
}

FilterCube::~FilterCube() {

}

Array<double> FilterCube::Flow(Array<double>& inputs, Long inputsSize, Long stride, Long pad) {
	Long inputChannel = this->length;
	Long inputWidth = sqrt((double)inputs.GetLength() / inputChannel);
	Long length = pow((double)((inputsSize + pad * 2) - dynamic_cast<ConvolutionFilter*>(this->GetAt(0))->GetSize()) / stride + 1, 2);

	Array<double> results(length);
	Long i = 0;
	while (i < length) {
		results.Store(i, 0.0);
		i++;
	}

	Long k = 0;
	i = 0;
	while (i < inputChannel) {
		Matrix<double> inputsMatrix(inputWidth + pad * 2, inputWidth + pad * 2);
		Long j = 0;
		while (j < inputsMatrix.GetRow())
		{
			Long l = 0;
			while (l < inputsMatrix.GetColumn()) {
				if ((j >= pad && j < pad + inputWidth) && (l >= pad && l < pad + inputWidth)) {
					inputsMatrix.Store(j, l, inputs[k++]);
				}
				else {
					inputsMatrix.Store(j, l, 0);
				}
				l++;
			}
			j++;
		}

		Matrix<double> outputs((dynamic_cast<ConvolutionFilter*>(this->neurons[i]))->Flow(inputsMatrix, stride));
		j = 0;
		while (j < outputs.GetLength()) {
			results[j] = results[j] + outputs[j];
			j++;
		}
		i++;
	}
	i = 0;
	while (i < results.GetLength()) {
		results[i] += this->bias;
		i++;
	}

	return results;
}

Array<double> FilterCube::BackFlow(Array<double>& inputs, Array<double>& signals, Long stride, Long pad) {
	Array<double> results(inputs.GetLength());
	Matrix<double> resultMatrix;
	Long inputWidth = static_cast<Long>(sqrt(inputs.GetLength() / this->length));
	// ��ǲ�� �ʺ� = ��Ʈ(inputs����/ä�� ��) 
	Long indexOfInputs = 0;
	Long indexOfReulsts = 0;

	Long signalWidth = static_cast<Long>(sqrt(signals.GetLength()));
	Matrix<double>signalMatrix(signalWidth, signalWidth);


	//1. inputs, ���� �Ϻ� �Է¹޴´�.
	Long i = 0;
	//2. ������ ��ȣ�� ��ķ� ��ȯ�ϴ�.
	while (i < signals.GetLength()) {
		signalMatrix.Store(i, signals[i]);
		i++;
	}

	i = 0;
	Long j;
	while (i < this->length) {//3. ���� ������ŭ �ݺ��Ѵ�.
		 //3.1. inputs�� ������( ���� ������ŭ)
		 //3.2. ���� input�� ��ķ� �����.
		Matrix<double> inputMatrix(inputWidth, inputWidth);
		j = 0;
		while (j < inputWidth* inputWidth) {
			inputMatrix.Store(j, inputs[indexOfInputs++]);
			j++;
		}

		//3.3. Filter ���� BackFlow�ϴ�.
		resultMatrix = dynamic_cast<ConvolutionFilter*>(this->neurons[i])->BackFlow(inputMatrix, signalMatrix, stride, pad);
		j = 0;
		while (j < resultMatrix.GetLength()) {
			results.Store(indexOfReulsts++, resultMatrix.GetAt(j));
			j++;
		}
		//3.4. ����迭�� ����ϴ�.
		i++;
	}

	//4. ���� ���⸦ ���ϴ�.
	this->biasGradient = 0;
	i = 0;
	while (i < signals.GetLength()) {
		this->biasGradient += signals[i];
		i++;
	}
	//4. ����迭�� ����ϴ�.
	return results;
	//5. ������.
}

Array<double> FilterCube::BatchBackFlow(Array<double>& inputs, Array<double>& signals, Long stride, Long pad) {
	Array<double> results(inputs.GetLength());
	Matrix<double> resultMatrix;
	Long inputWidth = static_cast<Long>(sqrt(inputs.GetLength() / this->length));
	// ��ǲ�� �ʺ� = ��Ʈ(inputs����/ä�� ��) 
	Long indexOfInputs = 0;
	Long indexOfReulsts = 0;

	Long signalWidth = static_cast<Long>(sqrt(signals.GetLength()));
	Matrix<double>signalMatrix(signalWidth, signalWidth);


	//1. inputs, ���� �Ϻ� �Է¹޴´�.
	Long i = 0;
	//2. ������ ��ȣ�� ��ķ� ��ȯ�ϴ�.
	while (i < signals.GetLength()) {
		signalMatrix.Store(i, signals[i]);
		i++;
	}

	i = 0;
	Long j;
	while (i < this->length) {//3. ���� ������ŭ �ݺ��Ѵ�.
		 //3.1. inputs�� ������( ���� ������ŭ)
		 //3.2. ���� input�� ��ķ� �����.
		Matrix<double> inputMatrix(inputWidth, inputWidth);
		j = 0;
		while (j < inputWidth* inputWidth) {
			inputMatrix.Store(j, inputs[indexOfInputs++]);
			j++;
		}

		//3.3. Filter ���� BackFlow�ϴ�.
		resultMatrix = dynamic_cast<ConvolutionFilter*>(this->neurons[i])->BatchBackFlow(inputMatrix, signalMatrix, stride, pad);
		j = 0;
		while (j < resultMatrix.GetLength()) {
			results.Store(indexOfReulsts++, resultMatrix.GetAt(j));
			j++;
		}
		//3.4. ����迭�� ����ϴ�.
		i++;
	}

	//4. ����迭�� ����ϴ�.
	return results;
	//5. ������.
}

void FilterCube::BatchBackFlow(Array<Array<double>>& inputs, Array<Array<double>>& signals, Long stride, Long pad) {
	Long batch = inputs.GetLength(); //��ġ ũ��

	//�迭�� �迭 signals�� ����� �迭�� �����
	Array<Matrix<double>> signalsOfFilter(batch); //������ BatchBackFlow�� �Ű����� ����� : �迭�� �迭 -> ����� �迭
	Long i = 0;
	while (i < signals.GetLength()) { //��ġ ũ�⸸ŭ �ݺ��Ѵ�.
		Array<double> signalsPerBatch = signals.GetAt(i); //�ش� ����ť��� ���� �ִ�(������ ��°��) ���� �迭
		Matrix<double> signalsMatrixPerBatch(sqrt(signalsPerBatch.GetLength()), sqrt(signalsPerBatch.GetLength())); //���� ���� �迭�� ��ȯ�� ���
		Long j = 0;
		while (j < signalsPerBatch.GetLength()) { //�迭�� ũ�⸸ŭ �ݺ��Ѵ�.
			signalsMatrixPerBatch.Store(j, signalsPerBatch[j]); //��Ŀ� �迭�� ��ҵ��� ��� �ִ´�.
			j++;
		}
		signalsOfFilter.Store(i, signalsMatrixPerBatch); //����� �迭�� ������.
		i++;
	}

	//���� ������ŭ �ݺ��Ѵ�.
	i = 0;
	while (i < this->length) {
		//���� ��° inputs�� �߷� ���� ��ķ� ���� �� �迭�� ����
		Array<Matrix<double>> inputsOfFilter(batch);
		Long j = 0;
		while (j < batch) { //��ġ��ŭ �ݺ��Ѵ�.
			Array<double> inputsArrayPerBatch = inputs.GetAt(j); //��ġ���� �������� ä�κ��δ� ���յ�����
			Long matrixLength = inputsArrayPerBatch.GetLength() / this->length; //��� ���� = ��ġ���� ���� �Է°� �迭�� ũ�� / ���Ͱ���(ä�� ��)
			Matrix<double> inputsMatrixPerBatch(sqrt(matrixLength), sqrt(matrixLength)); //��ȯ�� ���
			//���� ��° inputs�� �߸��� ���� ÷�� : ��ġ���� �������� ������ ä�κ��δ� ���յǾ��ִ� �迭���� ä�κ��� ���� ������ �� �ְ� ���������� ����
			Long l = i * matrixLength;
			Long k = 0;
			while (k < matrixLength) {
				inputsMatrixPerBatch.Store(k, inputsArrayPerBatch[l++]); //ä�κ� ���յ� �迭���� ä�κ��� ������ ���ÿ� ��ķ� ��ȯ�Ѵ�.
				k++;
			}
			inputsOfFilter.Store(j, inputsMatrixPerBatch); //����� �迭�� ������.
			j++;
		}
		//���Ϳ��� ������ : ����ġ ���⸸ ����
		dynamic_cast<ConvolutionFilter*>(this->neurons[i])->BatchBackFlow(inputsOfFilter, signalsOfFilter, stride, pad);

		i++;
	}

	// ���� ���⸦ ���ϴ�. : �Ű����� signals�� ��ġ���� �ش� ����ť��� ��°�� �����Ƿ� �װ��� ��� ���� ���� �ش� ����ť���� ���� ���Ⱑ �ȴ�.
	this->biasGradient = 0;
	i = 0;
	while (i < signals.GetLength()) {
		Array<double> signalsPerBatch = signals.GetAt(i);
		Long j = 0;
		while (j < signalsPerBatch.GetLength()) {
			this->biasGradient += signalsPerBatch[j];
			j++;
		}
		i++;
	}
}

void FilterCube::Update(double learningRate) {
	Long i = 0;
	while (i < this->length) {
		this->neurons[i]->Update(learningRate);
		i++;
	}
	this->bias -= learningRate * this->biasGradient;
}

void FilterCube::Update(double updatedLearningRate, double beta1, double beta2) {
	Long i = 0;
	while (i < this->length) {
		this->neurons[i]->Update(updatedLearningRate, beta1, beta2);
		i++;
	}
	this->biasMomentum += (1 - beta1) * (this->biasGradient - this->biasMomentum);
	this->biasVelocity += (1 - beta2) * (pow(this->biasGradient, 2) - this->biasVelocity);
	this->bias -= (updatedLearningRate * this->biasMomentum / (sqrt(this->biasVelocity) + 1e-7));
}

FilterCube& FilterCube::operator =(const FilterCube& source) {
	CompositeNeuron::operator =(source);
	this->bias = source.bias;
	this->biasGradient = source.biasGradient;
	this->biasMomentum = source.biasMomentum;
	this->biasVelocity = source.biasVelocity;

	return *this;
}

Neuron* FilterCube::Clone() {
	return new FilterCube(*this);
}