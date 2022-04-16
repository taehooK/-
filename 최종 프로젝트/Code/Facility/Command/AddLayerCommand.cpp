#include "AddLayerCommand.h"
#include "NeuralNetworkForm.h"
#include "NeuronForm.h"
#include "AddLayerDialog.h"

AddLayerCommand::AddLayerCommand(NeuronForm *neuronForm)
	: Command(neuronForm) {

}

AddLayerCommand::AddLayerCommand(const AddLayerCommand& source)
	: Command(source) {

}

AddLayerCommand::~AddLayerCommand() {

}

void AddLayerCommand::Execute() {
	AddLayerDialog addLayerDialog((CWnd*)this->neuronForm->neuralNetworkForm);
	addLayerDialog.DoModal();
}

AddLayerCommand& AddLayerCommand::operator =(const AddLayerCommand& source) {
	this->neuronForm = source.neuronForm;

	return *this;
}