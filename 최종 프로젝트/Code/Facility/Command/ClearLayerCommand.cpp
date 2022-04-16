#include "ClearLayerCommand.h"
#include "NeuronForm.h"
#include "NeuralNetwork.h"
#include "ScrollControllers.h"
#include "NeuralNetworkForm.h"
#include "GraphMaker.h"

ClearLayerCommand::ClearLayerCommand(NeuronForm *neuronForm) {
	this->neuronForm = neuronForm;
}

ClearLayerCommand::ClearLayerCommand(const ClearLayerCommand& source) {
	this->neuronForm = source.neuronForm;
}

ClearLayerCommand::~ClearLayerCommand() {

}

void ClearLayerCommand::Execute() {
	int ret = this->neuronForm->MessageBox("������ �ʱ�ȭ�Ͻðڽ��ϱ�?", "���� �ʱ�ȭ", MB_OKCANCEL);
	if (ret == IDOK) {
		if (this->neuronForm->neuralNetwork != 0) {
			delete this->neuronForm->neuralNetwork;
		}
		this->neuronForm->neuralNetwork = new NeuralNetwork;
		GraphMaker graphMaker;
		this->neuronForm->neuralNetworkForm->layerBoxControl = graphMaker.Make(dynamic_cast<NeuralNetwork*>(this->neuronForm->neuralNetwork), 20, 30, false);
	}
	this->neuronForm->neuralNetworkForm->scrollController->Update();
	this->neuronForm->neuralNetworkForm->Invalidate();
}

ClearLayerCommand& ClearLayerCommand::operator=(const ClearLayerCommand& source) {
	this->neuronForm = source.neuronForm;

	return *this;
}