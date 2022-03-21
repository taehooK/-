#include "InsertLayerCommand.h"
#include "AddLayerDialog.h"
#include "NeuronForm.h"
#include "NeuralNetworkForm.h"

InsertLayerCommand::InsertLayerCommand(NeuronForm *neuronForm)
:Command(neuronForm){
}

InsertLayerCommand::InsertLayerCommand(const InsertLayerCommand& source)
:Command(source){
}

InsertLayerCommand::~InsertLayerCommand(){
}

void InsertLayerCommand::Execute(){
	CString messageText;
	messageText.Format("������ �����ϸ� ����ġ�� ��� �ʱ�ȭ�˴ϴ�. �׷��� ���� �Ͻðڽ��ϱ�?");
	int ret = this->neuronForm->neuralNetworkForm->MessageBox((LPCTSTR)messageText, "���", MB_OKCANCEL | MB_ICONWARNING);

	if (ret == IDOK) {
		AddLayerDialog addLayerDialog((CWnd*)this->neuronForm->neuralNetworkForm);
		addLayerDialog.DoModal();
	}
}

InsertLayerCommand& InsertLayerCommand::operator=(const InsertLayerCommand& source){
	Command::operator=(source);
	return *this;
}