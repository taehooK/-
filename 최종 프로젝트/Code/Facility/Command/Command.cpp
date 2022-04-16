#include "Command.h"

Command::Command(NeuronForm *neuronForm) {
	this->neuronForm = neuronForm;
}

Command::Command(const Command& source) {
	this->neuronForm = source.neuronForm;
}

Command::~Command() {

}

Command& Command::operator =(const Command& source) {
	this->neuronForm = source.neuronForm;

	return *this;
}