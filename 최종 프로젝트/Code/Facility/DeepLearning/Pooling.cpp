#include "Pooling.h"
#include "Matrix.h"
#include "PoolingFilter.h"

Pooling::Pooling(Long capacity, Long size, Long stride)
	: Layer(capacity) {
	this->size = size;
	this->stride = stride;
}

Pooling::Pooling(const Pooling& source)
	: Layer(source) {
	this->size = source.size;
	this->stride = source.stride;
}

Pooling::~Pooling() {

}

Array<double> Pooling::Forward(Array<double>& signals) {
	Long inputSize = sqrt(signals.GetLength() / this->length);
	this->inputs = signals;
	this->inputCount = signals.GetLength();
	Long resultsSize = (inputSize - this->size) / this->stride + 1;
	Array<double> results(resultsSize*resultsSize*this->length);
	Long indexOfResults = 0;
	Long indexOfSignals = 0;
	Long i = 0;
	Long j;

	while (i < this->length) {
		Matrix<double> signalMatrix(inputSize, inputSize);
		j = 0;
		while (j < signalMatrix.GetCapacity()) {
			signalMatrix.Store(j, signals[indexOfSignals++]);
			j++;
		}
		Matrix<double> outputs(dynamic_cast<PoolingFilter*>(this->neurons[i])->Flow(signalMatrix, this->stride));

		j = 0;
		while (j < outputs.GetLength()) {
			results.Store(indexOfResults++, outputs.GetAt(j));
			j++;
		}
		i++;
	}

	return results;
}

Array<Array<double>> Pooling::BatchForward(Array<Array<double>>& signals) {
	this->batchInputs = signals;
	this->inputCount = signals.GetAt(0).GetLength();
	Long inputSize = sqrt(this->inputCount / this->length);
	Long resultsSize = (inputSize - this->size) / this->stride + 1;

	Long batch = signals.GetLength();
	Array<Array<double>> results(batch);

	Long indexOfBatch = 0;
	while (indexOfBatch < batch) {
		Array<double> signalsPerBatch = signals.GetAt(indexOfBatch);
		Array<double> resultsPerBatch(resultsSize*resultsSize*this->length);

		Long indexOfResults = 0;
		Long indexOfSignals = 0;
		Long j;

		Long i = 0;
		while (i < this->length) {
			Matrix<double> signalMatrix(inputSize, inputSize);
			j = 0;
			while (j < signalMatrix.GetCapacity()) {
				signalMatrix.Store(j, signalsPerBatch[indexOfSignals++]);
				j++;
			}
			Matrix<double> outputs(dynamic_cast<PoolingFilter*>(this->neurons[i])->Flow(signalMatrix, this->stride));

			j = 0;
			while (j < outputs.GetLength()) {
				resultsPerBatch.Store(indexOfResults++, outputs.GetAt(j));
				j++;
			}
			i++;
		}
		results.Store(indexOfBatch, resultsPerBatch);
		indexOfBatch++;
	}

	return results;
}

Array<double> Pooling::Backward(Array<double>& signals) {
	Long inputSize = sqrt(this->inputCount / this->length); //inputMatrix의 size는 inputs배열의길이 / 채널수의 루트값
	Long signalSize = (inputSize - this->size) / this->stride + 1; //signalMatrix의 size는 inputSize - 필터사이즈 / 스트라이드(필터사이즈) +1

	this->inputGradients = Array<double>(this->inputCount);
	Long indexOfInputGradients = 0;
	Matrix<double> resultMatrix(inputSize, inputSize);

	Long j;
	Long k;
	Long l = 0;
	Long m = 0;
	Long i = 0;
	while (i < this->length) { //채널수만큼 반복한다.
		Matrix<double> inputMatrix(inputSize, inputSize);
		j = 0;
		while (j < inputMatrix.GetRow()) { //inputMatrix의 size만큼 반복한다. (행)
			k = 0;
			while (k < inputMatrix.GetColumn()) { //inputMatrix의 size만큼 반복한다. (열)
				inputMatrix.Store(j, k, this->inputs[l++]); //inputMatrix에 inputs를 차례로 넣는다.
				k++;
			}
			j++;
		}

		Matrix<double> signalMatrix(signalSize, signalSize);
		j = 0;
		while (j < signalMatrix.GetRow()) {
			k = 0;
			while (k < signalMatrix.GetColumn()) {
				signalMatrix.Store(j, k, signals[m++]);
				k++;
			}
			j++;
		}

		//채널별로 잘리고 행렬로 변경된 input과 signal을 BackFlow의 매개변수로 넣는다. 필터별로 BackFlow
		resultMatrix = dynamic_cast<PoolingFilter*>(this->neurons[i])->BackFlow(inputMatrix, signalMatrix, this->stride);
		j = 0;
		while (j < resultMatrix.GetLength()) { //기울기행렬의 길이만큼 반복한다.
			this->inputGradients.Store(indexOfInputGradients++, resultMatrix.GetAt(j)); //반환할 기울기 배열에 다 넣는다.
			j++;
		}
		i++;
	}



	return this->inputGradients;
}

Array<Array<double>> Pooling::BatchBackward(Array<Array<double>>& signals) {
	Long inputSize = sqrt(this->inputCount / this->length); //inputMatrix의 size는 inputs배열의길이 / 채널수의 루트값

	Long signalSize = (inputSize - this->size) / this->stride + 1; //signalMatrix의 size는 inputSize - 필터사이즈 / 스트라이드(필터사이즈) +1

	Long batch = signals.GetLength();
	Array<Array<double>> batchInputGradients(batch);

	Long indexOfBatch = 0;
	while (indexOfBatch < batch) {
		Array<double> inputsPerBatch = this->batchInputs.GetAt(indexOfBatch);
		Array<double> signalsPerBatch = signals.GetAt(indexOfBatch);
		Array<double> batchInputGradientsPerBatch(this->inputCount);
		Long indexOfInputGradients = 0;
		Matrix<double> resultMatrix(inputSize, inputSize);

		Long j;
		Long k;
		Long l = 0;
		Long m = 0;
		Long i = 0;
		while (i < this->length) { //채널수만큼 반복한다.
			Matrix<double> inputMatrix(inputSize, inputSize);
			j = 0;
			while (j < inputMatrix.GetRow()) { //inputMatrix의 size만큼 반복한다. (행)
				k = 0;
				while (k < inputMatrix.GetColumn()) { //inputMatrix의 size만큼 반복한다. (열)
					inputMatrix.Store(j, k, inputsPerBatch[l++]); //inputMatrix에 inputs를 차례로 넣는다.
					k++;
				}
				j++;
			}
			Matrix<double> signalMatrix(signalSize, signalSize);
			j = 0;
			while (j < signalMatrix.GetRow()) {
				k = 0;
				while (k < signalMatrix.GetColumn()) {
					signalMatrix.Store(j, k, signalsPerBatch[m++]);
					k++;
				}
				j++;
			}
			//채널별로 잘리고 행렬로 변경된 input과 signal을 BackFlow의 매개변수로 넣는다. 필터별로 BackFlow
			resultMatrix = dynamic_cast<PoolingFilter*>(this->neurons[i])->BackFlow(inputMatrix, signalMatrix, this->stride);
			j = 0;
			while (j < resultMatrix.GetLength()) { //기울기행렬의 길이만큼 반복한다.
				batchInputGradientsPerBatch.Store(indexOfInputGradients++, resultMatrix.GetAt(j)); //반환할 기울기 배열에 다 넣는다.
				j++;
			}
			i++;
		}
		batchInputGradients.Store(indexOfBatch, batchInputGradientsPerBatch);
		indexOfBatch++;
	}


	return batchInputGradients;
}

Long Pooling::Save(fstream *fs) {
	*fs << this->length << endl;
	*fs << size << endl;
	*fs << stride << endl;

	return this->length;
}

Long Pooling::Load(fstream *fs) {

	Long filterCount;//1. 필터 수를 읽는다.
	*fs >> filterCount;
	this->neurons.Resize(filterCount);
	this->capacity = filterCount;
	//2. 필터 사이즈를 읽는다.
	*fs >> this->size;
	//3. 스트라이드를 읽는다.
	*fs >> this->stride;
	//4. 필터 수만큼 반복한다
	Long i = 0;
	while (i < filterCount) {
		PoolingFilter *filter = new PoolingFilter(this->size);// 4.1. 필터를 만든다.
		this->Add(filter);// 4.2. 폴링에 넣는다.
		i++;
	}

	return this->length;
}

void Pooling::MakeFilters(Long channel, Long filterCount) {
	Neuron *neuron;
	Long i = 0;
	while (i < this->length) { //neurons의 요소들이 포인터 : 할당 해제
		neuron = this->neurons[i];
		if (neuron != 0) {
			delete neuron;
		}
		i++;
	}
	this->neurons.Clear();
	this->neurons.Resize(filterCount);
	this->capacity = filterCount;
	this->length = 0;

	i = 0;
	while (i < channel) {
		Neuron *poolingFilter = new PoolingFilter(this->size);
		this->Add(poolingFilter);
		i++;
	}
}

string Pooling::GetType() {
	return "Pooling";
}

Neuron* Pooling::Clone() {
	return new Pooling(*this);
}

Pooling& Pooling::operator =(const Pooling& source) {
	Layer::operator=(source);
	this->size = source.size;
	this->stride = source.stride;
	return *this;
}