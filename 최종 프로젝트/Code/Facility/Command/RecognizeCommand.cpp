#include "RecognizeCommand.h"
#include "RecognizingForm.h"
#include "NeuronForm.h"
#include "Graph.h"
#include "resource.h"
#include "LearningForm.h"
#include "GraphMaker.h"
#include "Recognizer.h"
#include "Icon.h"
#include "RoundRectangle.h"

RecognizeCommand::RecognizeCommand(NeuronForm *neuronForm) {
	this->neuronForm = neuronForm;
}

RecognizeCommand::RecognizeCommand(const RecognizeCommand& source) {
	this->neuronForm = source.neuronForm;
}

RecognizeCommand::~RecognizeCommand() {

}

void RecognizeCommand::Execute() {
	if (this->neuronForm->recognizingForm->recognizeButton->GetID() == IDB_RECOGNIZE_OFF) { // �߷л��� �������̸�
		*dynamic_cast<Icon*>(this->neuronForm->recognizingForm->recognizeButton)
			= Icon(this->neuronForm->recognizingForm->recognizeButton->GetRect(), this->neuronForm->recognizingForm->recognizeButton->GetColor(),
				IDB_RECOGNIZE_ON, false);

		// Clear��ư�� �����.
		if (this->neuronForm->recognizingForm->clearButton != NULL) {
			delete this->neuronForm->recognizingForm->clearButton;
			this->neuronForm->recognizingForm->clearButton = NULL;
		}
		// �н��ϴ� ���� �׷����� �����.

		if (this->neuronForm->learningForm->accuracyCoordinateSystem != NULL) {
			delete this->neuronForm->learningForm->accuracyCoordinateSystem;
			this->neuronForm->learningForm->accuracyCoordinateSystem = NULL;
		}
		if (this->neuronForm->learningForm->lossCoordinateSystem != NULL) {
			delete this->neuronForm->learningForm->lossCoordinateSystem;
			this->neuronForm->learningForm->lossCoordinateSystem = NULL;
		}
		// �н��ϴ� ���� ���� ��ư�� �����.
		if (this->neuronForm->learningForm->startButton != NULL) {
			delete this->neuronForm->learningForm->startButton;
			this->neuronForm->learningForm->startButton = NULL;
			// �н��ϴ� ���� �����ϴ�.
			this->neuronForm->learningForm->Invalidate();
		}

		GraphMaker graphMaker;
		this->neuronForm->learningForm->accuracyCoordinateSystem = graphMaker.Make("�ݺ�Ƚ��", "��Ȯ��", CRect(100, 350, 700, 530), 10000, 1);
		this->neuronForm->learningForm->lossCoordinateSystem = graphMaker.Make("�ݺ�Ƚ��", "����", CRect(100, 50, 700, 230), 10000, 5);

		// �νı⿡�� �߷��ϴ�.
		this->neuronForm->recognizingForm->recognizer->AutoRecognize();

	}
	else if (this->neuronForm->recognizingForm->recognizeButton->GetID() == IDB_RECOGNIZE_ON) { // �߷оƴԻ��� �������̸�
	   // �νı⿡�� �����.
		this->neuronForm->recognizingForm->recognizer->Stop();

		*dynamic_cast<Icon*>(this->neuronForm->recognizingForm->recognizeButton)
			= Icon(this->neuronForm->recognizingForm->recognizeButton->GetRect(), this->neuronForm->recognizingForm->recognizeButton->GetColor(),
				IDB_RECOGNIZE_OFF, true);

		// Clear ��ư�� �����.
		this->neuronForm->recognizingForm->clearButton = new Icon(CRect(neuronForm->recognizingForm->imageRect.left,
			neuronForm->recognizingForm->imageRect.top - 30, neuronForm->recognizingForm->imageRect.left + 30, neuronForm->recognizingForm->imageRect.top - 10),
			RGB(0, 245, 0), IDB_RESET, false);
		// �н��ϴ� ���� ���� ��ư�� �����.
		this->neuronForm->learningForm->startButton = new RoundRectangle(CRect(600, 740, 700, 780), "�н� ����", RGB(0, 0, 255), false);
		// �н��ϴ� ���� �����ϴ�.
		this->neuronForm->learningForm->Invalidate();
	}
	this->neuronForm->recognizingForm->isPaint = false;
}

RecognizeCommand& RecognizeCommand::operator =(const RecognizeCommand& source) {
	this->neuronForm = source.neuronForm;
	return *this;
}